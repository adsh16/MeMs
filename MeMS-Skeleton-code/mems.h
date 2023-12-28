/*
All the main functions with respect to the MeMS are inplemented here
read the function discription for more details

NOTE: DO NOT CHANGE THE NAME OR SIGNATURE OF FUNCTIONS ALREADY PROVIDED
you are only allowed to implement the functions
you can also make additional helper functions a you wish

REFER DOCUMENTATION FOR MORE DETAILS ON FUNSTIONS AND THEIR FUNCTIONALITY
*/
// add other headers as required
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
/*
Use this macro where ever you need PAGE_SIZE.
As PAGESIZE can differ system to system we should have flexibility to modify this
macro to make the output of all system same and conduct a fair evaluation.
*/

/*
for this simulation assignment:
we refer to the adress returned by mmap as MeMs physical adress;
and we create an alterante contigious mapping starting from zero and return these Mems virtual adress.
these MeMs virtual adress has no meaning outside of MeMs and if the program want to write at an MeMs virtual adress it first has to request for MeMs physical adress from MeMs and then write at that adress.

*/

#define PAGE_SIZE 4096

// Define a structure to represent nodes in the free list.
struct mainNode
{
    void *start_address;
    size_t start_virtual_adress;
    size_t size;
    // also update MeMs_virtual_adress_counter afer allocating each main node
    struct mainNode *prev;
    struct mainNode *next;
    size_t unused_space;
    struct subNode *next_subNode;
};

struct subNode
{
    int type; // 0 for HOLE, 1 for PROCESS
    void *start_address;
    size_t start_virtual_adress;
    size_t size;
    struct subNode *prev;
    struct subNode *next;
    struct mainNode *mainNode; // main node under which this subNode comes
};

static struct mainNode *free_list_head = NULL;
static size_t MeMs_virtual_adress_counter;

/*
Initializes all the required parameters for the MeMS system. The main parameters to be initialized are:
1. the head of the free list i.e. the pointer that points to the head of the free list
2. the starting MeMS virtual address from which the heap in our MeMS virtual address space will start.
3. any other global variable that you want for the MeMS implementation can be initialized here.
Input Parameter: Nothing
Returns: Nothing
*/
void mems_init()
{
    // initializing the starting MeMs virtual address from zero.
    MeMs_virtual_adress_counter = (size_t)0;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------
// creating function to create mainNode and subNode using chunking.

struct mainNode* create_mainNode(){
    static void* memory_chunk = NULL;
    static size_t remaining_memory = 0;
    static size_t node_count = 0;
    
    // Check if there's enough memory for a new node, otherwise, allocate a new page
    if (remaining_memory < sizeof(struct mainNode)) {
        memory_chunk = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        remaining_memory = PAGE_SIZE;
        node_count = 0;
    }

    struct mainNode* new_Mainnode = (struct mainNode*)((char*)memory_chunk + (node_count * sizeof(struct mainNode)));
    node_count++;
    remaining_memory -= sizeof(struct mainNode);

    return new_Mainnode;
}
struct subNode* create_subNode(){
    static void* memory_chunk = NULL;
    static size_t remaining_memory = 0;
    static size_t node_count = 0;
    
    // Check if there's enough memory for a new node, otherwise, allocate a new page
    if (remaining_memory < sizeof(struct subNode)) {
        memory_chunk = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        remaining_memory = PAGE_SIZE;
        node_count = 0;
    }

    struct subNode* new_subnode = (struct subNode*)((char*)memory_chunk + (node_count * sizeof(struct subNode)));
    node_count++;
    remaining_memory -= sizeof(struct subNode);

    return new_subnode;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------

/*
This function will be called at the end of the MeMS system and its main job is to unmap the
allocated memory using the munmap system call.
Input Parameter: Nothing
Returns: Nothing
*/
void mems_finish()
{
    // Unmap all allocated memory.
    struct mainNode *current = free_list_head;
    while (current != NULL)
    {
        struct mainNode *next = current->next;
        munmap(current->start_address, current->size);
        current = next;
    }
}

/*
Allocates memory of the specified size by reusing a segment from the free list if
a sufficiently large segment is available.

Else, uses the mmap system call to allocate more memory on the heap and updates
the free list accordingly.

Note that while mapping using mmap do not forget to reuse the unused space from mapping
by adding it to the free list.
Parameter: The size of the memory the user program wants
Returns: MeMS Virtual address (that is created by MeMS)
*/
void* mems_malloc(size_t size)
{

    void *MeMs_virtual_address = NULL; // Initialize the return value to NULL

    // this is the first time we are calling mems_malloc thus we have to initlialize free_list_head.
    if (free_list_head == NULL)
    {
        if (size % PAGE_SIZE == 0)
        {
            // requested memory is in multiple of PAGE_SIZE
            free_list_head = create_mainNode();
            free_list_head->start_address = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            free_list_head->start_virtual_adress = MeMs_virtual_adress_counter; // 0
            free_list_head->size = size;

            free_list_head->prev = NULL;
            free_list_head->next = NULL;
            free_list_head->unused_space = 0; // as the whole page is being utilized

            // allocating memory for subnode
            struct subNode *temp_process_subnode = create_subNode();
            temp_process_subnode->type = 1; // for process
            temp_process_subnode->mainNode = free_list_head;

            free_list_head->next_subNode = temp_process_subnode;
            temp_process_subnode->next = NULL;
            temp_process_subnode->prev = NULL;

            temp_process_subnode->start_address = free_list_head->start_address;
            temp_process_subnode->start_virtual_adress = MeMs_virtual_adress_counter;
            temp_process_subnode->size = size;

            // updating MeMs_virtual_adress_counter
            MeMs_virtual_adress_counter += size;

            MeMs_virtual_address = (void *)temp_process_subnode->start_virtual_adress; // FIX adding (void *)
            return MeMs_virtual_address;
        }
        else
        {
            size_t memory_to_be_allocated = (size / PAGE_SIZE + 1) * PAGE_SIZE; // memory in multiple of PAGE_SIZE that will be called by mmap
            free_list_head = create_mainNode();
            free_list_head->start_address = mmap(NULL, memory_to_be_allocated, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            free_list_head->start_virtual_adress = MeMs_virtual_adress_counter;
            free_list_head->size = memory_to_be_allocated;
            free_list_head->prev = NULL;
            free_list_head->next = NULL;
            free_list_head->unused_space = memory_to_be_allocated - size;

            // allocating memory for subnode
            struct subNode *temp_process_subnode = create_subNode();
            temp_process_subnode->type = 1; // for process
            temp_process_subnode->mainNode = free_list_head;
            temp_process_subnode->start_address = free_list_head->start_address;
            temp_process_subnode->size = size;
            temp_process_subnode->start_virtual_adress = MeMs_virtual_adress_counter;

            // updating MeMs_virtual_adress_counter
            MeMs_virtual_adress_counter += memory_to_be_allocated;

            MeMs_virtual_address = (void *)temp_process_subnode->start_virtual_adress; // FIX adding (void *)

            struct subNode *temp_hole_subnode = create_subNode();
            temp_hole_subnode->type = 0; // for hole
            temp_process_subnode->mainNode = free_list_head;
            temp_hole_subnode->start_address = (void *)((char *)free_list_head->start_address + size); // FIX ptr = (void*)((char*)ptr + shift_size); (ptr is void*, size_t shift_size)
            temp_hole_subnode->start_virtual_adress = size;
            temp_hole_subnode->size = memory_to_be_allocated - size;

            free_list_head->next_subNode = temp_process_subnode;
            temp_process_subnode->next = temp_hole_subnode;
            temp_hole_subnode->prev = temp_process_subnode;
            temp_hole_subnode->next = NULL;
            temp_process_subnode->prev = NULL;

            return MeMs_virtual_address;
        }
    }
    else
    {
        // this means that this is NOT the first time mems_malloc has been called, thus we will check for available space.
        // We will transverse through the free list to find a hole of more of equal size than what is requested and return the mems_virtual adress.

        struct mainNode *current_mainNode = free_list_head;
        while (current_mainNode != NULL)
        {
            if (current_mainNode->unused_space >= size) // unused space is more than or equal to the requested memory
            {
                // Now will will iterate through the subnode of main node.
                struct subNode *current_subNode = current_mainNode->next_subNode;
                while (current_subNode != NULL)
                {
                    if (current_subNode->type == 0 && current_subNode->size >= size) // i.e this subnode is a hole and of sufficent size
                    {
                        // alocating memory for node
                        struct subNode *temp_process_subnode = create_subNode();
                        struct subNode *temp_hole_subnode = create_subNode();

                        temp_process_subnode->type = 1;
                        temp_process_subnode->start_address = current_subNode->start_address;
                        temp_process_subnode->start_virtual_adress = current_subNode->start_virtual_adress;
                        temp_process_subnode->size = size;
                        temp_process_subnode->mainNode = current_mainNode;

                        current_mainNode->unused_space -= size;

                        temp_hole_subnode->type = 0;
                        temp_hole_subnode->start_address = (void *)((char *)temp_process_subnode->start_address + size); //  FIX ptr = (void*)((char*)ptr + shift_size); (ptr is void*, size_t shift_size)
                        temp_hole_subnode->start_virtual_adress = temp_process_subnode->start_virtual_adress + size;
                        temp_hole_subnode->size = current_subNode->size - size; // size of original hole_node - requested memory
                        temp_hole_subnode->mainNode = current_mainNode;

                        if (temp_hole_subnode->size == 0)
                        {
                            // reconstructing pointers
                            temp_process_subnode->next = current_subNode->next;
                            temp_process_subnode->prev = current_subNode->prev;

                            if (current_subNode->next != NULL)
                            {
                                // mapping next node's prev to temp_process_subnode
                                current_subNode->next->prev = temp_process_subnode;
                            }
                            if (current_subNode->prev != NULL)
                            {
                                // mapping prev node's next to temp_process_subnode
                                current_subNode->prev->next = temp_process_subnode;
                            }
                            else
                            {
                                // this means this current_subNode was first subnode in subchain. Mapping main chaing first subnode to temp_process_node
                                current_mainNode->next_subNode = temp_process_subnode;
                            }
                            
                        }
                        else
                        {
                            // reconstructing pointers
                            temp_process_subnode->next = temp_hole_subnode;
                            temp_process_subnode->prev = current_subNode->prev;

                            temp_hole_subnode->next = current_subNode->next;
                            temp_hole_subnode->prev = temp_process_subnode;

                            if (current_subNode->next != NULL)
                            {
                                // mapping next node's prev to temp_hole_node
                                current_subNode->next->prev = temp_hole_subnode;
                            }
                            if (current_subNode->prev != NULL)
                            {
                                // mapping prev node's next to temp_process_node
                                current_subNode->prev->next = temp_process_subnode;
                            }
                            else
                            {
                                // this means this current_subNode was first subnode in subchain. Mapping main chaing first subnode to temp_process_node
                                current_mainNode->next_subNode = temp_process_subnode;
                            }
                        }

                        MeMs_virtual_address = (void *)temp_process_subnode->start_virtual_adress; // FIX adding (void *)
                        return MeMs_virtual_address;
                    }
                    current_subNode = current_subNode->next;
                }
            }
            current_mainNode = current_mainNode->next;
        }
    }
    // we were not able to find adequately sized hole in the free list to allocate memory. Therefore adding new mainNode to free list.
    struct mainNode *last_mainNode = free_list_head;
    // transversing to last mainNode in free list.
    while (last_mainNode->next != NULL)
    {
        last_mainNode = last_mainNode->next;
    }
    struct mainNode *temp_main_node = create_mainNode();
    last_mainNode->next = temp_main_node;

    size_t memory_to_be_allocated;
    if (size % PAGE_SIZE == 0)
    {
        memory_to_be_allocated = size; // memory in multiple of PAGE_SIZE that will be called by mmap
    }
    else
    {
        memory_to_be_allocated = (size / PAGE_SIZE + 1) * PAGE_SIZE; // memory in multiple of PAGE_SIZE that will be called by mmap
    }

    temp_main_node->start_address = mmap(NULL, memory_to_be_allocated, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    temp_main_node->start_virtual_adress = MeMs_virtual_adress_counter;
    temp_main_node->size = memory_to_be_allocated;
    // updating MeMs_virtual_adress_counter
    MeMs_virtual_adress_counter += memory_to_be_allocated;
    temp_main_node->prev = last_mainNode;
    temp_main_node->next = NULL;
    temp_main_node->unused_space = memory_to_be_allocated - size;

    // allocating memory for subnode
    struct subNode *temp_process_subnode = create_subNode();
    temp_process_subnode->type = 1; // for process
    temp_process_subnode->mainNode = temp_main_node;
    temp_process_subnode->start_address = temp_main_node->start_address;
    temp_process_subnode->size = size;
    temp_process_subnode->start_virtual_adress = temp_main_node->start_virtual_adress;

    MeMs_virtual_address = (void *)temp_process_subnode->start_virtual_adress; // FIX adding (void*)

    struct subNode *temp_hole_subnode = create_subNode();
    temp_hole_subnode->type = 0; // for hole
    temp_hole_subnode->mainNode = temp_main_node;
    temp_hole_subnode->start_address = (void *)((char *)temp_main_node->start_address + size); // FIX ptr = (void*)((char*)ptr + shift_size); (ptr is void*, size_t shift_size)
    temp_hole_subnode->start_virtual_adress = temp_process_subnode->start_virtual_adress + size;
    temp_hole_subnode->size = memory_to_be_allocated - size;

    temp_main_node->next_subNode = temp_process_subnode;
    if (temp_hole_subnode->size == 0)
    {
        temp_process_subnode->next = NULL;
    }
    else
    {
        temp_process_subnode->next = temp_hole_subnode;
        temp_hole_subnode->prev = temp_process_subnode;
        temp_hole_subnode->next = NULL;
    }

    temp_process_subnode->prev = NULL;

    return MeMs_virtual_address;
}

/*
this function print the stats of the MeMS system like
1. How many pages are utilised by using the mems_malloc
2. how much memory is unused i.e. the memory that is in freelist and is not used.
3. It also prints details about each node in the main chain and each segment (PROCESS or HOLE) in the sub-chain.
Parameter: Nothing
Returns: Nothing but should print the necessary information on STDOUT
*/
void mems_print_stats()
{
    printf("-------- Printing Stats [mems_print_stats] --------\n");
    struct mainNode *current_mainNode = free_list_head;
    while (current_mainNode != NULL)
    {
        // // debugginh
        // printf("\nstarting virtual address of page : %lu         starting physical adress of page : %lu\n",current_mainNode->start_virtual_adress,(unsigned long)current_mainNode->start_address);

        printf("MAIN [%lu : %lu]", current_mainNode->start_virtual_adress, current_mainNode->start_virtual_adress + current_mainNode->size - 1);
        printf(" -> ");
        struct subNode *current_subNode = current_mainNode->next_subNode;
        while (current_subNode != NULL)
        {
            printf("%d [ %lu : %lu ]", current_subNode->type, current_subNode->start_virtual_adress, current_subNode->start_virtual_adress + current_subNode->size - 1);
            printf(" <-> ");
            current_subNode = current_subNode->next;
        }
        printf("NULL\n");
        current_mainNode = current_mainNode->next;
    }

    size_t pages_used = 0;
    size_t unused_space = 0;
    struct mainNode *node = free_list_head;
    while (node != NULL)
    {
        pages_used += node->size / PAGE_SIZE;
        unused_space += node->unused_space;
        node = node->next;
    }
    printf("\nPages used: %lu\n", pages_used);
    printf("Unused space: %lu bytes\n", unused_space);

    printf(" \n--------------------------------------------------\n");
}

/*
Returns the MeMS physical address mapped to ptr ( ptr is MeMS virtual address).
Parameter: MeMS Virtual address (that is created by MeMS)
Returns: MeMS physical address mapped to the passed ptr (MeMS virtual address).
*/
void *mems_get(void *v_ptr)
{
    // You need to traverse through the free list and find the corresponding physical address
    struct mainNode *current_mainNode = free_list_head;
    while (current_mainNode != NULL)
    {
        if ((current_mainNode->start_virtual_adress <= (size_t)v_ptr) && ((size_t)v_ptr < (current_mainNode->start_virtual_adress + current_mainNode->size)))
        {
            // the virtual lies in the range of virtual address of the main node.
            void *MeMs_physical_adress = (void *)((char *)current_mainNode->start_address + ((size_t)v_ptr - current_mainNode->start_virtual_adress));
            return MeMs_physical_adress;
        }
        current_mainNode = current_mainNode->next;
    }
    return NULL; // Return NULL if the given virtual address is not found.
}

/*
this function free up the memory pointed by our virtual_address and add it to the free list
Parameter: MeMS Virtual address (that is created by MeMS)
Returns: nothing
*/
void mems_free(void *v_ptr)
{
    // You need to find the corresponding sub-node in the free list and mark it as a HOLE.
    // we get the virtual adress of a process subnode in the free list and we have to mark it as a whole.
    // also merge any adjacent holes if they exsist.

    struct mainNode *current_mainNode = free_list_head;
    while (current_mainNode != NULL)
    {
        struct subNode *current_subNode = current_mainNode->next_subNode;
        while (current_subNode != NULL)
        {
            if (current_subNode->start_virtual_adress == (size_t)v_ptr && current_subNode->type == 1)
            {
                current_subNode->type = 0; // Mark it as a HOLE.
                // Optionally, you can also update the unused_space in the mainNode.
                current_mainNode->unused_space += current_subNode->size;

                // Check for adjacent hole nodes and merge them.
                if (current_subNode->prev != NULL && current_subNode->prev->type == 0)
                {
                    // Merge with the previous hole node.
                    current_subNode->prev->size += current_subNode->size;
                    // Update the pointers to skip the current subNode.
                    current_subNode->prev->next = current_subNode->next;
                    if (current_subNode->next != NULL)
                    {
                        // mapping the prev of node next to current subnode to point to node prev to current subnode
                        current_subNode->next->prev = current_subNode->prev;
                    }
                    // Free the current subNode.
                    // munmap(current_subNode, sizeof(struct subNode));
                }

                if (current_subNode->next != NULL && current_subNode->next->type == 0)
                // if hole is in front of the node.
                {
                    // Merge this hole with next hole node.
                    current_subNode->next->size += current_subNode->size;
                    if (current_subNode->prev == NULL)
                    {
                        current_mainNode->next_subNode = current_subNode->next; // mapping it to main node
                        current_subNode->next->start_address = current_subNode->start_address;
                        current_subNode->next->start_virtual_adress = current_subNode->start_virtual_adress;
                    }
                    else
                    {
                        // skipping this node
                        current_subNode->prev->next = current_subNode->next;
                        current_subNode->next->start_address = current_subNode->start_address;
                        current_subNode->next->start_virtual_adress = current_subNode->start_virtual_adress;
                    }
                    // Free the next subNode.
                    // munmap(current_subNode->next, sizeof(struct subNode));
                }
                return;
            }
            current_subNode = current_subNode->next;
        }
        current_mainNode = current_mainNode->next;
    }
}