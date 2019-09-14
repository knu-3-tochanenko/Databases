#ifndef PART_3_DATABASE_H
#define PART_3_DATABASE_H

#include <stdio.h>
#include "tables.h"

#define MAX_NUMBER_OF_ENTRIES 30

FILE *vendorFile;
FILE *osFile;
FILE *indexFile;

unsigned long long numberOfEntries;
struct Index indexes[MAX_NUMBER_OF_ENTRIES];

struct db {
    bool (*addVendor)(struct Vendor *vendor);
    bool (*addOs)(struct Os os, char);
    struct Vendor *(*getVendor)(char key[5]);   //  Key - SAP code
    struct Os *(*getOs)(char key[8]);           //  Key - baseband version
    struct Os *(*getAllOs)(char key[5]);       //  Key - linked vendor's SAP
    void (*removeVendor)(char key[5]);          //  Key - SAP code
    void (*removeOs)(char key[8]);              //  Key - baseband version
    void (*updateVendor)(char key[5]);          //  Key - SAP code
    void (*updateOs)(char key[8]);              //  Key - baseband version

    void (*initFiles)(
            const char vendorFileName[],
            const char osFileName[],
            const char indexFileName[]
    );

    void (*normalize)();
    void (*end)();
};

extern const struct db DB;

#endif //PART_3_DATABASE_H
