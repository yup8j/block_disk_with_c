//
// Created by 石嘉晗 on 2019-04-26.
//

#ifndef V1_EXP4_UTILS_H
#define V1_EXP4_UTILS_H
#define BUFFER_SIZE 520
#define BLOCK_SIZE 64
#define R_START 0
#define S_START 20
#define MAX 10000
#define R_TIMES 2
#define S_TIMES 4
#define SEARCH_START 20000
#define PROJECTION_START 30000
#define JOIN_BASE 40000

#include "extmem.h"

void init_buffer(Buffer *buf);

void read_blk(unsigned int addr, Buffer *buf, unsigned char **blk);

int convert(unsigned char *blk);


int get_a_data(int begin, int end);

void save(unsigned char *blk, int num);

void save_blk(Buffer *buf, unsigned char **des, unsigned char *from, int *index, int *save_to, int base_address);

void save_last_blk(Buffer *buf, unsigned char *from, int times, int save_to);

void clear_mark();

int get_next_addr(int base);

#endif //V1_EXP4_UTILS_H
