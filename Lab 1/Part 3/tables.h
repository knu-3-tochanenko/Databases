#include "boolean.h"

#ifndef PART_3_TABLES_H
#define PART_3_TABLES_H

struct Vendor {
    char SAP[5];  // Key
    char name[32];
    char countryCode[3];
};

struct Os {
    char basebandVersion[8]; // Key
    int buildNumber;
    int androidVersion;
    int securityPatch;
    char name[32];
    int buildDate;
    char vendorSAP[5];   // Key to it's vendor
};

struct VendorCell {
    struct Vendor* vendor;
    int connectedTo;
    int numberOfConnected;
    bool isDeleted;
};

struct OsCell {
    struct Os* os;
    bool isDeleted;
};

struct Index {
    char SAP[5];
    int index;
};

#endif //PART_3_TABLES_H
