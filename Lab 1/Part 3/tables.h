#include "boolean.h"

#ifndef PART_3_TABLES_H
#define PART_3_TABLES_H

struct Vendor {
    char SAP[6];  // Key
    char name[33];
    char countryCode[4];
};

struct Os {
    char basebandVersion[9]; // Key
    int buildNumber;
    int androidVersion;
    char name[33];
    int buildDate;
    char vendorSAP[6];   // Key to it's vendor
};

struct VendorCell {
    struct Vendor vendor;
    unsigned long long connectedTo;
    int numberOfConnected;
    bool isDeleted;
};

struct OsCell {
    struct Os os;
    bool isDeleted;
};

struct Index {
    char SAP[5];
    unsigned long long index;
};

#endif //PART_3_TABLES_H
