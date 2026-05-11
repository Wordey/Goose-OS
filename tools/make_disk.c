#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

//===========================//
//==== Typedefs and enums ===//
//===========================//
// MBR Partition Record
typedef struct {
    uint8_t boot_indicator;
    uint8_t starting_chs[3];
    uint8_t os_type;
    uint8_t ending_chs[3];
    uint32_t starting_lba;
    uint32_t size_lba;
}__attribute__ ((packed)) MBR_PartitionRecord;

// MBR
typedef struct {
    uint8_t boot_code[440];
    uint32_t mbr_signature; // UniqueMBRDiskSignature
    uint16_t unknown;
    MBR_PartitionRecord partition[4];
    uint16_t boot_record;
}__attribute__ ((packed)) MBR;

//===========================//
//======== Varibles =========//
//===========================//
char* image_name = "GooseOS.img";           // 11 bytes value, it contains the name of the disk
uint64_t lba_size = 512;                    // 8 Bytes Value, it contains the size of lba
uint64_t esp_size = 1024*1024*33;           // 8 bytes value, it contains the size of EFI system partition, 33Mib
uint64_t datap_size = 1024*1024;            // 8 bytes value, it contains the size of data partition
uint64_t image_size = 0;                    // 8 bytes value, it contains the size of image
uint64_t esp_lbas, datap_lbas, image_size_lbas;   // 3 8 bytes value(total size 24 bytes), it contains the size in lbas

//=================================//
//===== Convert bytes to lbas ====//
//===============================//
uint64_t bytes_to_lbas(const uint64_t bytes) {
    return (bytes / lba_size) + (bytes % lba_size > 0 ? 1 : 0);
}

//===============================//
//===== Write Protective MBR ====//
//===============================//
bool write_pt_mbr(FILE* image){
    // MBR
    if (image_size_lbas > 0xFFFFFFFF) image_size_lbas = 0x100000000;
    MBR mbr = {
        .boot_code = { 0 },
        .mbr_signature = 0,
        .unknown = 0,
        .partition[0] = {
            .boot_indicator = 0,
            .starting_chs = { 0x00, 0x02, 0x00 },
            .os_type = 0xEE,    // Protective GPT
            .ending_chs = { 0xFF, 0xFF, 0xFF },
            .starting_lba = 0x00000001,
            .size_lba = image_size_lbas - 1,
        },
        .boot_record = 0xAA55,
    };

    // Write mbr to file
    if (fwrite(&mbr, 1, sizeof mbr, image) != sizeof mbr) {
        return false;
    }

    return true;
}

//======================//
//==== Main function ===//
//====================e=//
int main(void){
    FILE* image = fopen(image_name, "wb+");

    if (!image) {
        fprintf(stderr, "Fatal Error: Could create file %s\n", image_name);
        return EXIT_FAILURE;
    }

    // Set sizes
    image_size = esp_size + datap_size + (1024*1024);   // EFI System Partition + Data Partition + Some Extra padding
    image_size_lbas = bytes_to_lbas(image_size);

    if (!write_pt_mbr(image)) {
        fprintf(stderr, "Fatal Error: could write protective MBR for file %s\n", image_name);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
