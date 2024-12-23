# Custom Memory Management System (MeMs)
## Overview

MeMs is a custom memory management system implemented in C, utilizing `mmap` and `munmap` system calls for efficient memory allocation and deallocation. The system employs a 2-D free list data structure to track memory usage, manage virtual and physical address mappings, and handle memory fragmentation.

## Key Features

- **Memory Allocation (`mems_malloc`)**: Allocates memory of the requested size by reusing free segments or expanding memory via `mmap` if necessary. Returns a MeMS virtual address.
- **Memory Deallocation (`mems_free`)**: Frees allocated memory and marks it as reusable by updating the free list structure.
- **Address Translation (`mems_get`)**: Converts a MeMS virtual address to the corresponding physical address, ensuring memory consistency.
- **Statistics Printing (`mems_print_stats`)**: Provides insights into memory usage, including the number of mapped pages, unused memory, and details about allocated and free segments.

## Functions Provided

1. **`void* mems_malloc(size_t size)`**  
   Allocates memory of the specified size. If no suitable free segment exists, allocates new memory using `mmap`.  
   **Parameters:** Size of the memory to allocate.  
   **Returns:** MeMS virtual address of the allocated memory.

2. **`void mems_free(void* ptr)`**  
   Frees the memory pointed to by `ptr` and marks it as available in the free list.  
   **Parameters:** MeMS virtual address to free.  
   **Returns:** None.

3. **`void* mems_get(void* v_ptr)`**  
   Retrieves the physical address corresponding to a given MeMS virtual address.  
   **Parameters:** MeMS virtual address.  
   **Returns:** Physical address mapped to the virtual address.

4. **`void mems_print_stats()`**  
   Prints detailed statistics about memory usage, including:  
   - Total mapped pages (`mmap` calls)  
   - Unused memory in bytes  
   - Details of nodes and segments (PROCESS or HOLE)  

   **Parameters:** None.  
   **Returns:** None (prints to `STDOUT`).

## Data Structure

- **2-D Free List**:  
  - Tracks memory allocation and deallocation with a doubly linked list for efficient management.  
  - Each node represents a segment of memory marked as either `PROCESS` (allocated) or `HOLE` (free).

## Address Management

- **Virtual Memory Address**: Represents the contiguous address space for processes in MeMs. Virtual addresses are meaningful within MeMs but not externally.  
- **Physical Memory Address**: Points to the actual memory location in the system. Physical addresses may be non-contiguous and are tracked to prevent memory overlap between processes.

## Conclusion

MeMs provides a lightweight, customizable solution for memory management in C programs. By leveraging system calls and a robust data structure, it ensures efficient allocation, deallocation, and memory usage statistics. This project demonstrates a deeper understanding of memory management techniques and virtual-to-physical address translation.

---

Feel free to clone this repository and experiment with the provided functions to enhance your understanding of custom memory management systems!
