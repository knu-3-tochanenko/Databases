#include "boolean.h"

#ifndef PART_3_TABLES_H
#define PART_3_TABLES_H

struct Vendor {
    char SAP[5];  // Key
    char name[32];
    char country_code[3];
};

struct OS {
    char baseband_version[8]; // Key
    int build_number;
    int android_version;
    int security_patch;
    char name[32];
    int build_date;
    char vendor_SAP[5];   // Key to it's vendor
};

struct Vendor_Cell {
    struct Vendor* vendor;
    int connected_to;
    int number_of_connected;
    bool is_deleted;
};

struct OS_Cell {
    struct OS* os;
    bool is_deleted;
};

struct Index {
    char SAP[5];
    int index;
};

#endif //PART_3_TABLES_H
