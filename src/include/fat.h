#ifndef FAT_H
#define FAT_H

#include "stdint.h"

typedef struct BPB{
    uint8_t jmp[3];  //boot strap short jump
    uint8_t oem[8];  //oem name
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t number_of_fats;
    uint16_t root_entries;
    uint16_t total_sectors;
    uint8_t media_descriptor;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t number_of_heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors_big;
    uint8_t padding[512 - 36];
}__attribute__((packed)) BPB;

typedef struct FAT12{
    uint8_t filename[8];
    uint8_t ext[3];
    uint8_t attribute;
    uint8_t reserved;
    uint8_t creation_time_tenths;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t last_access_date;
    uint16_t first_cluster_high;
    uint16_t last_modification_time;
    uint16_t last_modification_date;
    uint16_t first_cluster_low;
    uint32_t file_size;
}__attribute__((packed)) FAT12;

typedef struct RDE{
    uint8_t filename[8];
    uint8_t ext[3];
    uint8_t attribute;
    uint8_t reserved;
    uint8_t creation_time_tenths;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t last_access_date;
    uint16_t first_cluster_high;
    uint16_t last_modification_time;
    uint16_t last_modification_date;
    uint16_t first_cluster_low;
    uint32_t file_size;
}__attribute__((packed)) RDE;



FAT12 *get_first_fat12(BPB *bpb);
RDE *get_first_rde(BPB *bpb);

#endif
