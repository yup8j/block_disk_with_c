//
// Created by 石嘉晗 on 2019-04-26.
//

#ifndef V1_EXP4_OPERATIONS_H
#define V1_EXP4_OPERATIONS_H

#include "utils.h"

int read_data(int addr);

void create_data(char *relation, int base_address, int times, int begin1, int end1, int begin2, int end2);

void linear_search(int base_address, int value);

void project(int base_address);

#endif //V1_EXP4_OPERATIONS_H
