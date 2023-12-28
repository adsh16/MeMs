// include other header files as needed
#include"mems.h"


int main(int argc, char const *argv[])
{
    // initialise the MeMS system 
    mems_init();
    
    int *a = mems_malloc(sizeof(int) * 100);
    int *b = mems_malloc((size_t)4096);
    
    int* phy_ptr_a = (int*) mems_get(&a);
    int* phy_ptr_b = (int*) mems_get(&b);

    printf("Virtual address: %lu\tPhysical Address: %lu\n",(unsigned long)a,(unsigned long)phy_ptr_a);
    printf("Virtual address: %lu\tPhysical Address: %lu\n",(unsigned long)b,(unsigned long)phy_ptr_b);

    mems_print_stats();
    mems_finish();
    return 0;
}
