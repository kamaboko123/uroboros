#include "fat.h"

FAT12 *get_first_fat12(BPB *bpb){
    uint32_t buf = (uint32_t)bpb;
    return (FAT12 *)(buf + bpb->reserved_sectors * bpb->bytes_per_sector);
}

RDE *get_first_rde(BPB *bpb){
    uint32_t buf = (uint32_t)bpb;
    // reserved sector + fat1 + fat2
    return (RDE *)(buf + (bpb->reserved_sectors * bpb->bytes_per_sector) + (bpb->number_of_fats * bpb->sectors_per_fat * bpb->bytes_per_sector));
}

