#include "database.h"
#include <stdio.h>

void init_fields(
        const char vendor_file_name[],
        const char os_file_name[],
        const char index_file_name[]
) {
    vendor_file = fopen(vendor_file_name, "ab+");
    os_file = fopen(os_file_name, "a+");
    index_file = fopen(index_file_name, "a+");
}

struct Vendor_Cell *add_vendor(struct Vendor *vendor) {
    struct Vendor_Cell newVendor = {
            .vendor = vendor,
            .connected_to = 0,
            .number_of_connected = 0,
            .is_deleted = false
    };
    
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
        .init_files = init_fields
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