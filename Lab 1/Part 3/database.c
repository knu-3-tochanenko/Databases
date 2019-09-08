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
    struct Index *sub;
    for (int i = 0; i < number_of_entries; i++) {
        fread(&sub, sizeof(sub), 1, index_file);
        indexes[i] = *sub;
    }
}

struct Vendor* get_vendor(char key[5]) {
    struct Vendor* found = NULL;
    for (int i = 0; i < number_of_entries; i++) {
        if (strcmp(indexes[i].SAP, key) == 0) {
            fseek(vendor_file, indexes[i].index, SEEK_SET);
            struct Vendor_Cell* vendor_cell;
            fread(&vendor_cell, sizeof(struct Vendor_Cell), 1, vendor_file);
            return vendor_cell->vendor;
        }
    }
    return NULL;
}

struct OS* get_os(char key[8]) {
    fseek(os_file, 0L, SEEK_END);
    int number_of_os = ftell(os_file) / sizeof(struct OS_Cell);
    fseek(os_file, 0L, SEEK_SET);

    struct OS_Cell* os_cell;
    for (int i = 0; i < number_of_os; i++) {
        fread(&os_cell, sizeof(struct OS_Cell), 1, os_file);
        if (strcmp(os_cell->os->baseband_version, key) == 0)
            return &os_cell->os;
    }
    return NULL;
}

struct OS* get_all_os() {
    fseek(os_file, 0L, SEEK_END);
    int number_of_os = ftell(os_file) / sizeof(struct OS_Cell);
    fseek(os_file, 0L, SEEK_SET);

    struct OS_Cell os_array[number_of_os];
    fread(&os_array, sizeof(struct OS_Cell), number_of_os, os_file);
    struct OS array[number_of_os];
    for (int i = 0; i < number_of_os; i++)
        array[i] = *os_array[i].os;
    return array;
}

bool add_vendor(struct Vendor *vendor) {
    const struct Vendor_Cell newVendor = {
            .vendor = vendor,
            .connected_to = 0,
            .number_of_connected = 0,
            .is_deleted = false
    };
    if (get_vendor(vendor->SAP) == NULL) {
        fseek(vendor_file, 0L, SEEK_END);
        int pos = ftell(vendor_file);
        fwrite(&newVendor, sizeof(newVendor), 1, vendor_file);
        fwrite(&newVendor.vendor->SAP, sizeof(newVendor.vendor->SAP), 1, index_file);
        fwrite(&pos, sizeof(pos), 1, index_file);
        indexes[number_of_entries] = (struct Index) {
                .SAP = *newVendor.vendor->SAP,
                .index = pos
        };
        number_of_entries++;
        return true;
    }
    return false;
}

bool add_os(struct OS *os) {
    const struct OS_Cell newOS = {
            .os = os,
            .is_deleted = 0
    };

    if (get_os(os->baseband_version) == NULL) {
        fseek(os_file, 0L, SEEK_END);
        int pos = ftell(os_file);
        fwrite(&newOS, sizeof(struct OS_Cell), 1, os_file);
        return true;
    }
    return false;
}

static int compare_vendor(const void *a, const void *b) {
    struct Vendor_Cell *a_vendor = (struct Vendor_Cell *) a;
    struct Vendor_Cell *b_vendor = (struct Vendor_Cell *) b;
    return strcmp(a_vendor->vendor->SAP, b_vendor->vendor->SAP);
}

void normalize_vendor(const char vendor_file_name[]) {
    fseek(vendor_file, 0L, SEEK_END);
    int number_of_vendors = ftell(vendor_file) / sizeof(struct Vendor_Cell);
    fseek(vendor_file, 0, SEEK_SET);

    struct Vendor_Cell vendor_array[number_of_vendors];

    fread(vendor_array, sizeof(struct Vendor_Cell), number_of_vendors, vendor_file);

    qsort(vendor_array, number_of_vendors, sizeof(struct Vendor_Cell), compare_vendor);
    fclose(vendor_file);
    //  TODO Add subelement count
    vendor_file = fopen(vendor_file_name, "wb");
    fwrite(vendor_array, sizeof(struct Vendor_Cell), number_of_vendors, vendor_file);
    fclose(vendor_file);
}

static int compare_os(const void *a, const void *b) {
    struct OS_Cell *a_os = (struct OS_Cell *) a;
    struct OS_Cell *b_os = (struct OS_Cell *) b;
    int key_SAP = strcmp(a_os->os->vendor_SAP, b_os->os->vendor_SAP);
    if (key_SAP == 0)
        return strcmp(a_os->os->baseband_version, b_os->os->baseband_version);
    else
        return key_SAP;
}

void normalize_os(const char os_file_name[]) {
    fseek(os_file, 0L, SEEK_END);
    int number_of_os = ftell(os_file) / sizeof(struct OS_Cell);
    fseek(os_file, 0L, SEEK_SET);

    struct OS_Cell os_array[number_of_os];

    //  Read all OSes
    // TODO Change reading method
    for (int i = 0; i < number_of_os; i++) {
        struct OS_Cell *os_cell;
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

void remove_os(char key[8]) {

}

void remove_vendor(char key[5]) {

}

void update_os(char key[8]) {

}

void update_vendor(char key[5]) {

}

void end() {
    fclose(vendor_file);
    fclose(os_file);
    fclose(index_file);
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
        .normalize = normalize,
        .end = end
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