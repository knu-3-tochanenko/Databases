//
// Created by Vlad on 07.09.2019.
//

#ifndef PART_3_TABLES_H
#define PART_3_TABLES_H

struct Vendor {
    char* SAP;  // Key
    char* name;
    char* country_code;
};

struct OS {
    char* baseband_version; // Key
    int build_number;
    int android_version;
    int security_patch;
    char* name;
    int build_date;
    char* vendor_SAP;   // Key to it's vendor
};

#endif //PART_3_TABLES_H
