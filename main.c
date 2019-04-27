#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "extmem.h"
#include "operations.h"
#include "utils.h"
void create_data_main() {
    create_data("R", R_START, R_TIMES, 1, 40, 1, 1000);
    create_data("S", S_START, S_TIMES, 20, 60, 1, 1000);
}

int main() {
    srand((unsigned) time(NULL));
    int num;
//    read_data(0);
//    create_data_main();
//    read_data(R_START);
//    read_data(S_START);
    nested_loop_join(R_START,S_START);
    read_data(1+JOIN_START);
//    linear_search(S_START,60);
//    linear_search(R_START,40);
//    project(R_START);
}