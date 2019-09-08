#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_fields(
        const char vendor_file_name[],
        const char index_file_name[],
        const char os_file_name[]
) {
    vendor_file = fopen(vendor_file_name, "ab+");
    index_file = fopen(index_file_name, "ab+");
    os_file = fopen(os_file_name, "ab+");

    //  Read all indexes to memory
    fseek(index_file, 0L, SEEK_END);
    number_of_entries = ftell(index_file) / sizeof(struct Index);
    fseek(index_file, 0L, SEEK_SET);
    struct Index* sub;
    for (int i = 0; i < number_of_entries; i++) {
        fread(&sub, sizeof(sub), 1, index_file);
        indexes[i] = *sub;
    }
}

struct Vendor_Cell *add_vendor(struct Vendor *vendor) {
    const struct Vendor_Cell newVendor = {
            .vendor = vendor,
            .connected_to = 0,
            .number_of_connected = 0,
            .is_deleted = false
    };
    fseek(vendor_file, 0L, SEEK_END);
    int pos = ftell(vendor_file);
    fwrite(&newVendor, sizeof(newVendor), 1, vendor_file);
    fwrite(&newVendor.vendor->SAP, sizeof(newVendor.vendor->SAP), 1, index_file);
    fwrite(&pos, sizeof(pos), 1, index_file);
    indexes[number_of_entries] = (struct Index){
            .SAP = *newVendor.vendor->SAP,
            .index = pos
    };
    number_of_entries++;
}

static int compare_vendor(const void* a, const void* b) {
    struct Vendor_Cell* a_vendor = (struct Vendor_Cell*)a;
    struct Vendor_Cell* b_vendor = (struct Vendor_Cell*)b;
    return strcmp(a_vendor->vendor->SAP, b_vendor->vendor->SAP);
}

void normalize_vendor() {

}

static int compare_os(const void* a, const void* b) {
    struct OS_Cell* a_os = (struct OS_Cell*)a;
    struct OS_Cell* b_os = (struct OS_Cell*)b;
    int key_SAP = strcmp(a_os->os.vendor_SAP, b_os->os.vendor_SAP);
    if (key_SAP == 0)
        return strcmp(a_os->os.baseband_version, b_os->os.baseband_version);
    else
        return key_SAP;
}

void normalize_os(const char os_file_name[]) {
    fseek(os_file, 0L, SEEK_END);
    int number_of_os = ftell(os_file) / sizeof(struct OS_Cell);
    fseek(os_file, 0L, SEEK_SET);

    struct OS_Cell os_array[number_of_os];

    //  Read all OSes
    for (int i = 0; i < number_of_os; i++) {
        struct OS_Cell* os_cell;
        fread(&os_cell, sizeof(os_cell), 1, os_file);
        os_array[i] = *os_cell;
    }

    // Sort by Vendor's SAP and by baseband version
    qsort(os_array, number_of_os, sizeof(struct OS_Cell), compare_os);

    fclose(os_file);
    os_file = fopen(os_file_name, "wb");
    //  TODO Check if it writes all elements
    fwrite(os_array, sizeof(struct OS_Cell), number_of_os, os_file);
    fclose(os_file);
}

void normalize_index(const char index_file_name[]) {
    fclose(index_file);
    index_file = fopen(index_file_name, "wb");
    for (int i = 0; i < number_of_entries; i++) {
        fwrite(indexes[i].SAP, sizeof(indexes[i].SAP), 1, index_file);
        fwrite(&indexes[i].index, sizeof(indexes[i].index), 1, index_file);
    }
    fclose(index_file);
}

//  Clears all unused fields and rewrites files.
//  Reopens files when done.
void normalize(
        const char vendor_file_name[],
        const char index_file_name[],
        const char os_file_name[]
) {
    normalize_os(os_file_name);
    normalize_vendor(vendor_file_name);
    normalize_index(index_file_name);
    //  Reopen files
    init_fields(vendor_file_name, index_file_name, os_file_name);
}

const struct db DB = {
        // User functions
        .add_os = add_os,
        .add_vendor = add_vendor,
        .get_all_os = get_all_os,
        .get_os = get_os,
        .get_vendor = get_vendor,
        .remove_os = remove_os,
        .remove_vendor = remove_vendor,
        .update_os = update_os,
        .update_vendor = update_vendor,
        // Helping functions
        .init_files = init_fields,
        .normalize = normalize
};

// struct Vendor_Cell(*add_vendor)(struct Vendor vendor);
// struct OS(*add_os)(struct OS os, char);
// struct Vendor(*get_vendor)(char key[5]);    //  Key - SAP code
// struct OS(*get_os)(char key[8]);            //  Key - baseband version
// struct OS*(*get_all_os)(char key[5]);       //  Key - linked vendor's S
// void(*remove_vendor)(char key[5]);          //  Key - SAP code
// void(*remove_os)(char key[8]);              //  Key - baseband version
// void(*update_vendor)(char key[5]);          //  Key - SAP code
// void(*update_os)(char key[8]);              //  Key - baseband version