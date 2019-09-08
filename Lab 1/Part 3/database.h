#ifndef PART_3_DATABASE_H
#define PART_3_DATABASE_H

#include <stdio.h>
#include "tables.h"

#define MAX_NUMBER_OF_ENTRIES 30

FILE *vendor_file;
FILE *os_file;
FILE *index_file;

int number_of_entries;
struct Index indexes[MAX_NUMBER_OF_ENTRIES];

struct db {
    bool (*add_vendor)(struct Vendor *vendor);
    bool (*add_os)(struct OS os, char);
    struct Vendor *(*get_vendor)(char key[5]);   //  Key - SAP code
    struct OS *(*get_os)(char key[8]);           //  Key - baseband version
    struct OS *(*get_all_os)(char key[5]);       //  Key - linked vendor's SAP
    void (*remove_vendor)(char key[5]);          //  Key - SAP code
    void (*remove_os)(char key[8]);              //  Key - baseband version
    void (*update_vendor)(char key[5]);          //  Key - SAP code
    void (*update_os)(char key[8]);              //  Key - baseband version

    void (*init_files)(
            const char vendor_file_name[],
            const char os_file_name[],
            const char index_file_name[]
    );

    void (*normalize)();
    void (*end)();
};

extern const struct db DB;

#endif //PART_3_DATABASE_H
