//
// Created by 石嘉晗 on 2019-04-26.
//
/**
 * @date
 */
#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
static short mark[MAX];

void init_buffer(Buffer *buf) {
    if (!initBuffer(BUFFER_SIZE, BLOCK_SIZE, buf)) {
        perror("Buffer Initialization Failed!\n");
        exit(1);
    }
}

void read_blk(unsigned int addr, Buffer *buf, unsigned char **blk) {
    if ((*blk = readBlockFromDisk(addr, buf)) == NULL) {
        perror("Reading Block Failed!\n");
        exit(3);
    }
}

void write_blk(unsigned char *blk, unsigned int addr, Buffer *buf) {
    if (writeBlockToDisk(blk, addr, buf) != 0) {
        perror("Writing Block Failed!\n");
        exit(2);
    }
}

int convert(unsigned char *blk) {
    int result = 0;
    memcpy(&result, blk, sizeof(int));
    return result;
}

void save(unsigned char *blk, int num) {
    memcpy(blk, &num, sizeof(int));
}

int get_a_data(int begin, int end) {
    int n = rand() % (end + 1 - begin) + begin;
    return n;
}

void save_blk(Buffer *buf, unsigned char **des, unsigned char *from, int *index, int *save_to, int base_address) {
    int next;
    if (*index >= 7) {
        *index = 0;
        save(*des + 8 * 7, 7);
        next = get_next_addr(base_address);
        save(*des + 7 * 8 + 4, next);
        write_blk(*des, *save_to, buf);
        *save_to = next;
        *des = getNewBlockInBuffer(buf);
    }
    memcpy(*des + *index * 8, from, 8);
    ++(*index);
}

void save_last_blk(Buffer *buf, unsigned char *from, int times, int save_to) {
    save(from + 7 * 8, times);
    save(from + 7 * 8 + 4, 0);
    write_blk(from, save_to, buf);
}

void clear_mark() {
    memset(mark, 0, MAX);
}

int get_next_addr(int base) {
    int num=0;
    do {
        num++;
    } while (mark[num]);
    mark[num] = 1;
    return num + base;
}