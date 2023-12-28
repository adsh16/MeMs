Creating a Memory Management System(MeMs)
	1. MeMS should utilizes mmap and munmap system calls for memory allocation and deallocation.
	2. Implement the following functions within MeMS:
		- Creating a Memory Management System(MeMs)
    1. MeMS should utilizes mmap and munmap system calls for memory allocation and deallocation.
    2. The following functions are exposes by MeMs.
		- void* mems_malloc(size_t size): Allocates memory of the specified size by reusing a segment from the free list if a sufficiently large segment is available. Else, uses the mmap system call to allocate more memory on the heap and updates the free list accordingly.

		Parameter: The size of the memory the user program wants
		Returns: MeMS Virtual address (that is created by MeMS)

		- void mems_free(void* ptr): Frees the memory pointed by ptr by marking the corresponding sub-chain node in the free list as HOLE. Once a sub-chain node is marked as HOLE, it becomes available for future allocations.
		Parameter: MeMS Virtual address (that is created by MeMS)

		Returns: nothing

		- void mems_print_stats(): Prints the total number of mapped pages (using mmap) and the unused memory in bytes (the total size of holes in the free list). It also prints details about each node in the main chain and each segment (PROCESS or HOLE) in the sub-chain.
		Parameter: Nothing

		Returns: Nothing but should print the necessary information on STDOUT

		- void *mems_get(void*v_ptr): Returns the MeMS physical address mapped to ptr ( ptr is MeMS virtual address).
		Parameter: MeMS Virtual address (that is created by MeMS)

		Returns: MeMS physical address mapped to the passed ptr (MeMS virtual address).
	3. MeMs used a 2-d doubly linked list (free list) data struct to track and manage memory requirements.
	4. A transalation between Virtual Memory address and Physical address is taken care of by MeMs.
		Virtual Memory Adress : correspoding to adress space of the process. Contigoous and has no meaning outside of MeMs.
		Physical Memory adrees : corrrespoding to pointer returned pointing to actual memory in the system. Can be non contigous for repeated calls. There is a need to track the adresses to ensure memory is not overlapped between processes.

