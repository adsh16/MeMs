// include other header files as needed
#include "mems.h"

int main(int argc, char const *argv[])
{
    // initialise the MeMS system
    mems_init();

    int *a = (int *)mems_malloc(4096);
    int *b = (int *)mems_malloc(4096);
    int *c = (int *)mems_malloc(196);

    int *phy_ptr_b = (int *)mems_get(&b);
    int *phy_ptr_a = (int *)mems_get(&a);
    int *phy_ptr_c = (int *)mems_get(&c);



    mems_print_stats();
    printf("Physical Address: %lu\n", (unsigned long)a);
    printf("Physical Address: %lu\n", (unsigned long)b);
    printf("Physical Address: %lu\n", (unsigned long)phy_ptr_a);
    printf("Physical Address: %lu\n", (unsigned long)phy_ptr_b);
    printf("Physical Address: %lu\n", (unsigned long)phy_ptr_c);

    // printf("Actual adress of a : %lu\n", (unsigned long)phy_ptr_a[0]);

    mems_finish();
    return 0;
}
