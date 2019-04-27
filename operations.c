//
// Created by 石嘉晗 on 2019-04-26.
//
/**
 * @file operations.c
 * @author Shi
 * @date 0425
 * @brief 本次实验可能用的操作
 */

#include "operations.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "extmem.h"

/**
 * 从指定地址开始连续读数据
 * @param addr 起始地址
 * @retval 0 读取成功
 */
int read_data(int addr) {
    Buffer buf;
    int i, j = 0;
    int num, capacity;
    unsigned char *blk = NULL;

    init_buffer(&buf); //Init Buffer
    do {
        read_blk(addr, &buf, &blk);
        capacity = convert(blk + 8 * 7); //capacity: 块内元组数目，默认为7
        if (!capacity) {
            capacity = 7;
        }
        printf("Read data from block %d.blk\n", addr);
        printf("Tuple amount:%2d\n", capacity);
        for (i = 0; i < capacity; i++) {
            num = convert(blk + 8 * i);
            printf("%d: %10d, ", i, num);
            num = convert(blk + 8 * i + 4);
            printf("%10d\n", num);
        }
        addr = convert(blk + 8 * 7 + 4);
        printf("The next block address is: %5d\n", addr);
        freeBlockInBuffer(blk, &buf);
    } while (addr);
    printf("End Read\n");
    printf("---------------\n");
    freeBuffer(&buf);
    return 0;
}


int do_create_data(int start_addr, int begin1, int end1, int begin2, int end2, int flag, char *relation) {
    int i, j = 0;
    Buffer buf;
    int now_addr = start_addr;
    int next_addr = now_addr;
    int value1, value2;
    unsigned char *blk;
    init_buffer(&buf);
    for (i = 0; i < 8; i++) {
        blk = getNewBlockInBuffer(&buf);

        for (j = 0; j < 7; j++) {
            value1 = get_a_data(begin1, end1);
            value2 = get_a_data(begin2, end2);
            save(blk + j * 8, value1);
            save(blk + j * 8 + 4, value2);
            //hash
        }
        next_addr = now_addr + 1;
        save(blk + 7 * 8, 7);

        if (i == 7 && flag == 0) {
            save(blk + 7 * 8 + 4, 0); //最后一个地址
        } else {
            save(blk + 7 * 8 + 4, next_addr);
        }
        writeBlockToDisk(blk, (unsigned int) now_addr, &buf);
        now_addr = next_addr;
    }
    freeBuffer(&buf);
    return next_addr;
}

void create_data(char *relation, int base_address, int times, int begin1, int end1, int begin2, int end2) {
    int i;
    int start_addr = 0;
    int next_addr = start_addr + base_address;
    for (i = 0; i < times; i++) {
        next_addr = do_create_data(next_addr, begin1, end1, begin2, end2, times - i - 1, relation);
    }
}

/**
 * 线性搜索算法，结果保存在20000开头的block中
 * @param base_address 关系基地址
 * @param value 要搜索的值
 */
void linear_search(int base_address, int value) {
    int i;
    Buffer buf;
    unsigned char *blk, *blk_saver;
    int start_addr, save_to;
    int index_saver = 0;
    unsigned int next_addr;
    int capacity, data;
    start_addr = get_next_addr(SEARCH_START);
    save_to = start_addr;
    int addr = base_address;
    next_addr = (unsigned int) addr;
    init_buffer(&buf);
    blk_saver = getNewBlockInBuffer(&buf);
    do {
        read_blk(next_addr, &buf, &blk);
        capacity = convert(blk + 8 * 7);
        for (i = 0; i < capacity; ++i) {
            data = convert(blk + i * 8);
            if (data == value) {
                save_blk(&buf, &blk_saver, blk + i * 8, &index_saver, &save_to, SEARCH_START);
            }
        }
        next_addr = (unsigned int) convert(blk + 8 * 7 + 4);
        freeBlockInBuffer(blk, &buf);
    } while (next_addr);
    save_last_blk(&buf, blk_saver, index_saver, save_to);
    freeBuffer(&buf);
}

//int liner_search(char *rel, char *col, int op, int value) {
//    int i;
//    Buffer buf;
//    unsigned char *blk, *blk_saver;
//    int addr[2], offset, times, data, index_saver = 0;
//    /* when find -> save it into there */
//    int next_addr, start_addr, save_to;
//    start_addr = get_next_addr(SEARCH_BASE);
//    save_to = start_addr;
//    /* get start addr of relation */
//    get_start_addr_from_file(rel, col, addr);
//    offset = addr[1];
//    next_addr = addr[0];
//
//    init_buf(&buf);
//    blk_saver = getNewBlockInBuffer(&buf);
//    /* read blk from disk and save it into buffer */
//    while (next_addr) {
//        read_blk(next_addr, &buf, &blk);
//        /* the number of tuple in this blk */
//        times = convert(blk + 8 * 7);
//        for (i = 0; i < times; ++i) {
//            data = convert(blk + 8 * i + offset * 4);
//            if (compare(data, value, op)) {
//                save_blk(&buf, &blk_saver, blk + i * 8, &index_saver, &save_to);
//            }
//        }
//        next_addr = convert(blk + 8 * 7 + 4);
//        /* free blk */
//        freeBlockInBuffer(blk, &buf);
//    }
//    save_last_blk(&buf, blk_saver, index_saver, save_to);
//
//    freeBuffer(&buf);
//    return start_addr;
//}

/**
 * 连接操作
 * @param base_address 关系基地址，在本实验中为R的基地址
 */
void project(int base_address) {
    clear_mark();
    Buffer buf;
    short label[1000];
    memset(label, 0, 1000);
    unsigned char *blk, *blk_saver;
    init_buffer(&buf);
    blk_saver = getNewBlockInBuffer(&buf);
    int addr = base_address;
    int capacity, index_saver = 0, data;
    int start_addr, save_to, next_addr;
    start_addr = get_next_addr(PROJECTION_START);
    save_to = start_addr;
    do {
        read_blk((unsigned int) addr, &buf, &blk);
        capacity = convert(blk + 8 * 7);
        next_addr = convert(blk + 8 * 7 + 4);
        for (int i = 0; i < capacity; ++i) {
            data = convert(blk + 8 * i);
            if (!label[data]) {
                label[data] = 1;
                save_blk(&buf, &blk_saver, blk + i * 8, &index_saver, &save_to, PROJECTION_START);
            }
        }
        freeBlockInBuffer(blk, &buf);
        addr = next_addr;
    } while (addr);
    save_last_blk(&buf, blk_saver, index_saver, save_to);
    freeBuffer(&buf);
}

/**
 * Nested loop join 操作
 * @param base_addr_r R 关系基地址
 * @param base_addr_s S 关系基地址
 * @return join后起始地址
 */

int nested_loop_join(int base_addr_r, int base_addr_s) {
    Buffer buf;
    int index_saver = 0;
    int start_addr, save_to;
    unsigned char *blk_saver, *blk_1, *blk_2;
    unsigned char tmp[8];
    int capacity1, capacity2, value1, value2;
    init_buffer(&buf);
    clear_mark();
    start_addr = get_next_addr(JOIN_START);
    save_to = start_addr;
    blk_saver = getNewBlockInBuffer(&buf);
    int next_r, next_s;
    next_r = base_addr_r;
    do {
        read_blk((unsigned int) next_r, &buf, &blk_1);
        capacity1 = convert(blk_1 + 8 * 7);
        next_r = convert(blk_1 + 8 * 7 + 4);
        next_s = base_addr_s;
        while (next_s) {
            read_blk((unsigned int) next_s, &buf, &blk_2);
            capacity2 = convert(blk_2 + 8 * 7);
            next_s = convert(blk_2 + 8 * 7 + 4);
            for (int offset1 = 0; offset1 < capacity1; ++offset1) {
                for (int offset2 = 0; offset2 < capacity2; ++offset2) {
                    value1 = convert(blk_1 + offset1 * 8);
                    value2 = convert(blk_2 + offset2 * 8);
                    if (value1 == value2) {
                        save_blk(&buf, &blk_saver, blk_1 + offset1 * 8, &index_saver, &save_to, JOIN_START);
                        memcpy(tmp, blk_2 + offset2 * 8 + 4, 4);
                        memset(tmp + 4, 0, 4);
                        save_blk(&buf, &blk_saver, tmp, &index_saver, &save_to, JOIN_START);
                    }
                }
            }
            freeBlockInBuffer(blk_2, &buf);
        }
        freeBlockInBuffer(blk_1, &buf);
    } while (next_r);
    save_last_blk(&buf, blk_saver, index_saver, save_to);
    freeBuffer(&buf);
    return start_addr;
}
