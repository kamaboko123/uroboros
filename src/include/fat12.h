#ifndef FAT12_H
#define FAT12_H

#include "stdlibc.h"

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

typedef void FAT12;

typedef struct DE{
    char filename[8];
    char ext[3];
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
}__attribute__((packed)) DE;

typedef DE RDE;

typedef struct Cluster{
    uint8_t data[512];
}__attribute__((packed)) Cluster;

#define FAT12_ATTR_RO 0x01
#define FAT12_ATTR_HIDDEN 0x02
#define FAT12_ATTR_SYSTEM 0x04
#define FAT12_ATTR_VOLUME_ID 0x08
#define FAT12_ATTR_DIRECTORY 0x10
#define FAT12_ATTR_ARCHIVE 0x20
#define FAT12_ATTR_LFN 0x0F

typedef struct FileName{
    char name[8];
    char ext[3];
    char display[13];
} FileName;


typedef enum FAT_TYPE{
    FAT_TYPE_12 = 12,
    FAT_TYPE_16 = 16,
    FAT_TYPE_32 = 32
} FAT_TYPE;

#define FAT12_AVAILABLE 0x00000000
#define FAT12_RESERVED 0x00000001
#define FAT12_BAD 0x0FFFFFF7
#define FAT12_EOC 0x0FFFFFF8

#define FILE_NAME_EOT 0x00
#define FILE_NAME_DELETED 0xE5

FAT12 *get_fat12(BPB *bpb);
RDE *get_rde(BPB *bpb);
uint8_t *get_first_data_sector(BPB *bpb);

DE *find_entry(DE *entry, FileName *filename);
bool compare_short_filename(DE *entry, FileName *filename);
void set_filename(FileName *filename, char *name, char *ext);
void upcase(char *str, int len);
void copy_filename(char *str1, char *str2, int len);
uint32_t get_total_clusters(BPB *bpb);
uint16_t get_fat12_entry(BPB *bpb, uint16_t cluster_number);
void read_sector(BPB *bpb, uint32_t cluster, uint8_t *buf, uint32_t size);
uint32_t read_file(BPB *bpb, DE *entry, uint8_t *buf, uint32_t from, uint32_t size);
uint32_t find_free_cluster(BPB *bpb);
uint32_t get_max_files_in_cluster(BPB *bpb);
uint32_t count_cluster_link(BPB *bpb, uint32_t cluster_number);
uint32_t get_cluster_number(uint8_t *fat, DE *entry);

#endif
