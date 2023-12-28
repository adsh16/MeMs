# operating_system
<<<<<<< HEAD
Code related to assignment does in operation system course.
Have made a custom MeMs.Function Implementations:

------------------------------------------------------------------------------------------------------------------------------------------------------------------------
=======
Making a custom (MeMS) Memory Management System.

Problem Statement:
Implement a custom memory management system (MeMS) using the C programming language. MeMS should utilize the system calls mmap and munmap for memory allocation and deallocation, respectively. The system must satisfy the following constraints and requirements outlined below:


Constraints and Requirements:

MeMS can solely use the system calls mmap and munmap for memory management. The use of any other memory management library functions such as malloc, calloc, free, and realloc are STRICTLY PROHIBITED.
MeMS should request memory from the OS using mmap in multiples of the system's PAGE_SIZE, which can be determined using the command getconf PAGE_SIZE. For most Linux distributions, the PAGE_SIZE is 4096 bytes (4 KB); however, it might differ for other systems.
MeMS should deallocate memory only through munmap and deallocation should only occur in multiples of PAGE_SIZE.
As the value of PAGE_SIZE can differ from system to system hence use the macro “PAGE_SIZE” provided in the template wherever you need the value of PAGE_SIZE in your code so that this size can be modified if required for evaluation purposes.
The user program must use the functions provided by MeMS for memory allocation and deallocation. It is not allowed to use any other memory management library functions, including malloc, calloc, free, realloc, mmap, and munmap.
Although MeMS requests memory from the OS in multiples of PAGE_SIZE, it only allocates that much amount of memory to the user program as requested by the user program. MeMS maintains a free list data structure to keep track of the heap memory which MeMS has requested from the OS. This free list keeps track of two items:
memory allocated to each user program. We will call this memory as PROCESS in the free list (details below).
Memory which has not been allocated to any user program. We will call this memory as a HOLE in the free list (details below).

Free List Structure:

Free List is represented as a doubly linked list. Let's call this doubly linked list as the main chain of the free list. The main features of the main chain are:

Whenever MeMS requests memory from the OS (using mmap), it adds a new node to the main chain.
Each node of the main chain points to another doubly linked list which we call as sub-chain. This sub-chain can contain multiple nodes. Each node corresponds to a segment of memory within the range of the memory defined by its main chain node. Some of these nodes (segments) in the sub-chain are mapped to the user program. We call such nodes (segments) as PROCESS nodes. Rest of the nodes in the sub-chain are not mapped to the user program and are called as HOLES or HOLE nodes.
Whenever the user program requests for memory from MeMS, MeMS first tries to find a sufficiently large segment in any sub-chain of any node in the main chain. If a sufficiently large segment is found, MeMS uses it to allocate memory to the user program and updates the segment’s type from HOLE to PROCESS. Else, MeMS requests the OS to allocate more memory on the heap (using mmap) and add a new node corresponding to it in the main chain.

NOTE: You must handle the corner cases and make sure that your system should avoids memory fragmentation within the free list.

MeMS Virtual Address and MeMS Physical Address:
Let us call the address (memory location) returned by mmap as the MeMS physical address. In reality, the address returned by mmap is actually a virtual address in the virtual address space of the process in which MeMS is running. But for the sake of this assignment, since we are simulating memory management by the OS, we will call the virtual address returned by mmap as MeMS physical address.
Just like a call to mmap returns a virtual address in the virtual address space of the calling process, a call to mems_malloc will return a MeMS virtual address in the MeMS virtual address space of the calling process. For the sake of this assignment, MeMS manages heap memory for only one process at a time.
Just like OS maintains a mapping from virtual address space to physical address space, MeMS maintains a mapping from MeMS virtual address space to MeMS physical address space. So, for every MeMS physical address (which is provided by mmap), we need to assign a MeMS virtual address. As you may understand, this MeMS virtual address has no meaning outside the MeMS system.
Any time the user process wants to write/store anything to the heap, it has to make use of the MeMS virtual address. But we cannot directly write using MeMS virtual address as the OS does not have any understanding of MeMS virtual address space. Therefore, we first need to get the MeMS physical address for that MeMS virtual address. Then, the user process needs to use this MeMS physical address to write on the heap.

Function Implementations:
>>>>>>> 14848f13e7481a863412a0977a671d9ad31183a2
Implement the following functions within MeMS:

void mems_init(): Initializes all the required parameters for the MeMS system. The main parameters to be initialized are
the head of the free list i.e. the pointer that points to the head of the free list
the starting MeMS virtual address from which the heap in our MeMS virtual address space will start.
any other global variable that you want for the MeMS implementation can be initialized here.
Input Parameter: Nothing

Returns: Nothing

void mems_finish(): This function will be called at the end of the MeMS system and its main job is to unmap the allocated memory using the munmap system call.
Input Parameter: Nothing

Returns: Nothing

void* mems_malloc(size_t size): Allocates memory of the specified size by reusing a segment from the free list if a sufficiently large segment is available. Else, uses the mmap system call to allocate more memory on the heap and updates the free list accordingly.
Parameter: The size of the memory the user program wants

Returns: MeMS Virtual address (that is created by MeMS)

void mems_free(void* ptr): Frees the memory pointed by ptr by marking the corresponding sub-chain node in the free list as HOLE. Once a sub-chain node is marked as HOLE, it becomes available for future allocations.
Parameter: MeMS Virtual address (that is created by MeMS)

Returns: nothing

void mems_print_stats(): Prints the total number of mapped pages (using mmap) and the unused memory in bytes (the total size of holes in the free list). It also prints details about each node in the main chain and each segment (PROCESS or HOLE) in the sub-chain.
Parameter: Nothing

Returns: Nothing but should print the necessary information on STDOUT

void *mems_get(void*v_ptr): Returns the MeMS physical address mapped to ptr ( ptr is MeMS virtual address).
Parameter: MeMS Virtual address (that is created by MeMS)

Returns: MeMS physical address mapped to the passed ptr (MeMS virtual address).
<<<<<<< HEAD
------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Have made two types of nodes for free list.
    1. mainSubnode
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
    2.struct subNode
    {
    int type; // 0 for HOLE, 1 for PROCESS
    void *start_address;
    size_t start_virtual_adress;
    size_t size;
    struct subNode *prev;
    struct subNode *next;
    struct mainNode *mainNode; // main node under which this subNode comes
    };

the starting mems_virtual adress in 0.
memory mapping in inclusive of both adresses.
// creating function to create mainNode and subNode using chunking.
this uses chunking using mmap to call PAGE_SIZE to allocate nodes for free list.
=======
>>>>>>> 14848f13e7481a863412a0977a671d9ad31183a2
