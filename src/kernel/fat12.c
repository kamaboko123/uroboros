#include "fat12.h"

// FATディスクの構造として、BPB, FAT12, RDE, Clusterの4つの構造がある
// BPB: ブートセクタの情報を格納する
// FAT12: クラスタの情報を格納する
// RDE: ルートディレクトリエントリの情報を格納
// Cluster: データを格納する

FAT12 *get_fat12(BPB *bpb){
    // 先頭から予約済みセクタの後ろからがFATになる
    uint8_t *head = (uint8_t *)bpb;
    return (FAT12 *)(head + bpb->reserved_sectors * bpb->bytes_per_sector);
}

RDE *get_rde(BPB *bpb){
    uint8_t *head = (uint8_t *)bpb;
    // reserved sector + fat1 + fat2
    // ルートディレクトリエントリはFATの後ろにある
    // 先頭アドレス + 予約済みセクタ数 * セクタサイズ + FATの数 * FATのセクタ数 * セクタサイズ
    // FATは通常2つある(FATにおいては最も重要なので、通常用+バックアップ用で2つ)
    return (RDE *)(head
            + (bpb->reserved_sectors * bpb->bytes_per_sector)
            + (bpb->number_of_fats * bpb->sectors_per_fat * bpb->bytes_per_sector));
}

uint32_t get_total_clusters(BPB *bpb){
    return bpb->total_sectors / bpb->sectors_per_cluster;
}

FAT_TYPE get_fat_type(BPB *bpb){
    // fat typeを決定する唯一の方法はクラスタ数のみらしい
    // http://elm-chan.org/docs/fat.html
    uint32_t total_clusters = get_total_clusters(bpb);
    if (total_clusters <= 4085){
        return FAT_TYPE_12;
    }
    else if (total_clusters <= 65525){
        return FAT_TYPE_16;
    }
    else{
        return FAT_TYPE_32;
    }
}

uint8_t *get_first_data_sector(BPB *bpb){
    uint8_t *head = (uint8_t *)bpb;
    // 予約済みセクタ数 + FATのセクタ数 * FATの数 + ルートディレクトリセクタ数
    uint32_t offset_sectors
        = bpb->reserved_sectors // 予約済みセクタ数
        + bpb->number_of_fats * bpb->sectors_per_fat // FATのセクタ数 * FATの数
        + (bpb->root_entries * sizeof(DE) / bpb->bytes_per_sector); // ルートディレクトリセクタ数
    
    return head + offset_sectors * bpb->bytes_per_sector;
}


DE *find_entry(DE *entry, FileName *filename){
    // ファイル名が一致するエントリを探す
    // (ファイル名の先頭が0x00の場合はテーブルの終端)
    while (entry->filename[0] != 0x00){
        if (entry->filename[0] == 0xE5){
            // ファイル名の先頭が0xE5の場合は削除済みのエントリ
            entry++;
            continue;
        }
        if (entry->attribute == 0x0F){
            // ファイル名の先頭が0x0Fの場合はLFN(Long File Name)エントリ
            // TODO: LFNの処理
            entry++;
            continue;
        }
        if (compare_short_filename(entry, filename)){
            // ファイル名が一致した場合はエントリを返す
            return entry;
        }
        entry++;
    }
    return NULL;
}

/*
void get_full_filename(DE *entry, FileName *filename){
    // TODO: error handling
    // ファイル名を取得する
    memset(filename_buf, 0, 12);
    strncat(filename_buf, (char *)entry->filename, 8);
    strncat(filename_buf, (char *)entry->ext, 3);
}
*/

void padding_short_filename(char *buf, char *filename){
    char padding_str[] = "        ";
    strncat(filename, padding_str, 9);
}

bool compare_short_filename(DE *entry, FileName *filename){
    //printf("'%8s' == '%8s'\n", entry->filename, filename->name);
    return strncmp(entry->filename, filename->name, 8) == 0 && strncmp(entry->ext, filename->ext, 3) == 0;
}

void set_filename(FileName *filename, char *name, char *ext){
    copy_filename(filename->name, name, 8);
    copy_filename(filename->ext, ext, 3);
    
    memset(filename->display, 0, 12);
    strncat(filename->display, filename->name, 8);
    if (filename->ext[0] != ' '){
        strncat(filename->display, ".", 2);
        strncat(filename->display, filename->ext, 3);
    
    }
}

void copy_filename(char *dst, char *src, int len){
    memset(dst, ' ', len);

    for (int i = 0; i < len; i++){
        if (src[i] == ' ' || src[i] == '\0'){
            break;
        }
        dst[i] = src[i];
    }
    upcase(dst, len);
}

void upcase(char *str, int len){
    for (int i = 0; i < len; i++){
        if (*str >= 'a' && *str <= 'z'){
            *str = *str + ('A' - 'a');
        }
        str++;
    }
}

uint32_t get_cluster_number(uint8_t *fat, DE *entry){
    uint16_t low = entry->first_cluster_low;
    // FAT12のクラスタ番号は12bitなので、highはない
    // FAT16, 32の場合はhighもある
    uint16_t high = entry->first_cluster_high;
    uint32_t cluster = (high << 8) | low;
    return cluster;
}

uint16_t get_fat12_entry(BPB *bpb, uint16_t cluster_number){
    // FAT12エントリは12bit単位で格納されている
    // fat[0]: entry0の下位8bit,
    // fat[1]: enyrt1の下位4bit + entry0の上位4bit
    // fat[2]: entry1の上位8bit
    // fat[3]: entry2の下位8bit
    // fat[4]: entry3の下位4bit + entry2の上位4bit
    // ...
    // まずはクラスタ番号から関連するfatの3byteを取得する
    // 3byteごとに2つのクラスタのエントリが格納されているので、
    // cluseter_number * 1.5 でFATのエントリの位置を計算できる
    // ただし整数演算しかできない環境向けに、まずcluster_numberを2で割って(切り捨て)、そのあと3倍する
    // (gccの-msoft-floatオプションを使ってもいい気もするけど、別に浮動小数点演算をやりたいわけではない)
    // offset = (cluster_number / 2) * 3
    // entry0: 0
    // entry1: 0
    // entry2: 3
    // entry3: 3
    // entry4: 6
    // entry5: 6
    // ...

    // 関連するFATの3byteを取得
    uint16_t fat_offset = (cluster_number / 2);
    fat_offset = fat_offset * 3;

    // FATの先頭アドレスを取得して、求めたoffsetを加算して、cluster_numberに対応するFATのエントリを取得
    uint8_t *p = (uint8_t *)get_fat12(bpb);
    p += fat_offset;


    if (cluster_number % 2 == 0){
        // クラスタ番号が偶数の場合は、
        // byte[0]がそのまま下位8bit、byte[1]の下位4bitが上位4bitになる
        return (uint16_t)*p | (uint16_t)((*(p + 1) & 0x0f) << 8);
    }
    else{
        // クラスタ番号が奇数の場合は、
        // byte[1]の上位4bitが下位4bit、byte[2]が上位8bitになる
        return ((uint16_t)*(p + 1) & 0xf0) >> 4 | (uint16_t) *(p + 2) << 4;
    }
}

void read_sector(BPB *bpb, uint32_t cluster, uint8_t *buf, uint32_t size){
    uint8_t *data_sector = get_first_data_sector(bpb);
    // fat entry 0, 1はデータ領域には含まれない
    data_sector += (cluster - 2) * bpb->bytes_per_sector;
    memcpy(buf, data_sector, size);
}


uint32_t read_file(BPB *bpb, DE *entry, uint8_t *buf, uint32_t from, uint32_t size){
    // ファイルの先頭クラスタを取得
    uint32_t start_cluster = get_cluster_number(get_fat12(bpb), entry);

    // 指定された開始領域までクラスタをたどる
    // fromがファイルの先頭からのバイト数なので、セクタ数で割ることで指定された開始アドレスが含まれるクラスタまで進める
    for (int i = 0; i < (from / bpb->bytes_per_sector); i++){
        // クラスタをたどる
        start_cluster = get_fat12_entry(bpb, start_cluster);
        if (start_cluster >= FAT12_EOC){
            // EOC(End of Cluster)に到達した場合は終了
            if(i * bpb->bytes_per_sector < from){
                // 指定された開始アドレスがファイルサイズを超えている場合はエラー
                return 0;
            }
            break;
        }
    }

    uint32_t read_size = 0;

    // fromがセクタの先頭から始まっていない場合は、fromから次のセクタの先頭までを読み込む
    if(from % bpb->bytes_per_sector != 0){
        // 次のセクタの先頭まで読み込む(セクタサイズで割った余りがfromから次のセクタの先頭までのバイト数)
        read_sector(bpb, start_cluster, buf, from % bpb->bytes_per_sector);
        // 読み込んだバイト数を加算
        read_size += from % bpb->bytes_per_sector;
        // サイズから読み込んだバイト数を引いて、残りの読み込むバイト数を計算
        size -= read_size;
    }

    uint32_t cluster = start_cluster;
    // セクタ単位で読み込める分だけ読み込む
    // セクタサイズで割った商が読み込むセクタ数
    for (int i = 0; i < (size / bpb->bytes_per_sector); i++){
        // セクタを読み込む
        read_sector(bpb, cluster, buf + read_size, bpb->bytes_per_sector);
        // 読み込んだバイト数を加算
        read_size += bpb->bytes_per_sector;
        
        // 次のクラスタを取得
        cluster = get_fat12_entry(bpb, cluster);
        if (cluster >= FAT12_EOC){
            // EOC(End of Cluster)に到達した場合は終了
            return read_size;
        }
    }

    // セクタ単位で読み込めない残りのバイト数を読み込む
    if (size % bpb->bytes_per_sector != 0){
        read_sector(bpb, cluster, buf + read_size, size % bpb->bytes_per_sector);
        read_size += size % bpb->bytes_per_sector;
    }
    return read_size;
}

uint32_t find_free_cluster(BPB *bpb){
    // 使用可能なクラスタを探す
    for (int i = 2; i < get_total_clusters(bpb); i++){
        if (get_fat12_entry(bpb, i) == FAT12_AVAILABLE){
            return i;
        }
    }
    return 0;
}

uint32_t count_free_clusters(BPB *bpb){
    uint32_t count = 0;
    for (int i = 2; i < get_total_clusters(bpb); i++){
        if (get_fat12_entry(bpb, i) == FAT12_AVAILABLE){
            count++;
        }
    }
    return count;
}

uint32_t get_max_files_in_cluster(BPB *bpb){
    // クラスタ内に格納できる最大ファイル数
    uint32_t cluster_size = bpb->bytes_per_sector * bpb->sectors_per_cluster;
    return cluster_size / sizeof(DE);
}

/*
uint32_t create_file(BPB *bpb, DE *parent, FileName *filename){}
uint32_t create_dir(BPB *bpb, DE *parent, FileName *filename){
    RDE *rde = get_rde(bpb);
    if(rde == parent){
        // RDEの場合はファイル数に制限がある
    }
}
*/

uint32_t count_cluster_link(BPB *bpb, uint32_t cluster_number){
    uint32_t count = 0;

    uint32_t next_cluster = cluster_number;
    do{
        count++;
        next_cluster = get_fat12_entry(bpb, next_cluster);
    }while(next_cluster < FAT12_EOC && next_cluster > FAT12_RESERVED);

    return count - 1;
}
