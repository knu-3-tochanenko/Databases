#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initFields(
        const char *vendorFileName,
        const char *indexFileName,
        const char *osFileName
) {
    vendorFile = fopen(vendorFileName, "ab+");
    indexFile = fopen(indexFileName, "ab+");
    osFile = fopen(osFileName, "ab+");

    //  Read all indexes to memory
    fseek(indexFile, 0L, SEEK_END);
    numberOfEntries = ftell(indexFile) / sizeof(struct Index);
    fseek(indexFile, 0L, SEEK_SET);
    struct Index *sub;
    for (unsigned long long i = 0; i < numberOfEntries; i++) {
        fread(sub, sizeof(sub), 1, indexFile);
        indexes[i] = *sub;
    }
}

struct Vendor* getVendor(char *key) {
    struct Vendor* found = NULL;
    for (int i = 0; i < numberOfEntries; i++) {
        if (strcmp(indexes[i].SAP, key) == 0) {
            fseek(vendorFile, indexes[i].index, SEEK_SET);
            // init fields
            struct VendorCell vendorCell;
            struct Vendor *vendor;
            int connectedTo, numberOfConnected;
            bool isDeleted;
            // Read all fields from file
            fread(vendor, sizeof(struct Vendor), 1, vendorFile);
            fread(&connectedTo, sizeof(int), 1, vendorFile);
            fread(&numberOfConnected, sizeof(int), 1, vendorFile);
            fread(&isDeleted, sizeof(bool), 1, vendorFile);
            // Init VendorCell object
            vendorCell.vendor = vendor;
            vendorCell.numberOfConnected = numberOfConnected;
            vendorCell.connectedTo = connectedTo;
            vendorCell.isDeleted = isDeleted;

            return vendorCell.vendor;
        }
    }
    return NULL;
}

struct Os* getOs(char *key) {
    fseek(osFile, 0L, SEEK_END);
    unsigned long long numberOfOs = ftell(osFile) / sizeof(struct OsCell);
    fseek(osFile, 0L, SEEK_SET);

    struct OsCell osCell;
    struct Os *os;
    bool isDeleted;
    for (unsigned long long i = 0; i < numberOfOs; i++) {
        fread(os, sizeof(struct Os), 1, osFile);
        fread(&isDeleted, sizeof(bool), 1, osFile);
        if (strcmp(osCell.os->basebandVersion, key) == 0)
            return osCell.os;
    }
    return NULL;
}

struct Os* getAllOs() {
    fseek(osFile, 0L, SEEK_END);
    unsigned long long numberOfOs = ftell(osFile) / sizeof(struct OsCell);
    fseek(osFile, 0L, SEEK_SET);

    struct Os array[numberOfOs];
    struct Os* os;
    bool isDeleted;
    for (unsigned long long i = 0; i < numberOfOs; i++) {
        fread(os, sizeof(struct Os), 1, osFile);
        fread(&isDeleted, sizeof(bool), 1, osFile);
        array[i] = *os;
    }
    return array;
}

bool addVendor(struct Vendor *vendor) {
    const struct VendorCell newVendor = {
            .vendor = vendor,
            .connectedTo = 0,
            .numberOfConnected = 0,
            .isDeleted = false
    };
    if (getVendor(vendor->SAP) == NULL) {
        fseek(vendorFile, 0L, SEEK_END);
        unsigned long long pos = ftell(vendorFile);
        fwrite(newVendor.vendor, sizeof(struct Vendor), 1, vendorFile);
        fwrite(vendor->SAP, sizeof(vendor->SAP), 1, indexFile);
        fwrite(&pos, sizeof(unsigned long long), 1, indexFile);
        // TODO Write proper file function
        fwrite(&newVendor, sizeof(newVendor), 1, vendorFile);
        fwrite(&newVendor.vendor->SAP, sizeof(newVendor.vendor->SAP), 1, indexFile);
        fwrite(&pos, sizeof(pos), 1, indexFile);
        indexes[numberOfEntries] = (struct Index) {
                .SAP = *newVendor.vendor->SAP,
                .index = pos
        };
        numberOfEntries++;
        return true;
    }
    return false;
}

bool add_os(struct Os *os) {
    const struct OsCell newOS = {
            .os = os,
            .isDeleted = 0
    };

    if (getOs(os->basebandVersion) == NULL) {
        fseek(osFile, 0L, SEEK_END);
        int pos = ftell(osFile);
        // TODO Write proper file function
        fwrite(&newOS, sizeof(struct OsCell), 1, osFile);
        return true;
    }
    return false;
}

static int compare_vendor(const void *a, const void *b) {
    struct VendorCell *a_vendor = (struct VendorCell *) a;
    struct VendorCell *b_vendor = (struct VendorCell *) b;
    return strcmp(a_vendor->vendor->SAP, b_vendor->vendor->SAP);
}

void normalize_vendor(const char vendor_file_name[]) {
    fseek(vendorFile, 0L, SEEK_END);
    int number_of_vendors = ftell(vendorFile) / sizeof(struct VendorCell);
    fseek(vendorFile, 0, SEEK_SET);

    struct VendorCell vendor_array[number_of_vendors];

    // TODO Write proper file function
    fread(vendor_array, sizeof(struct VendorCell), number_of_vendors, vendorFile);

    qsort(vendor_array, number_of_vendors, sizeof(struct VendorCell), compare_vendor);
    fclose(vendorFile);
    //  TODO Add subelement count
    vendorFile = fopen(vendor_file_name, "wb");
    fwrite(vendor_array, sizeof(struct VendorCell), number_of_vendors, vendorFile);
    fclose(vendorFile);
}

static int compare_os(const void *a, const void *b) {
    struct OsCell *a_os = (struct OsCell *) a;
    struct OsCell *b_os = (struct OsCell *) b;
    int key_SAP = strcmp(a_os->os->vendorSAP, b_os->os->vendorSAP);
    if (key_SAP == 0)
        return strcmp(a_os->os->basebandVersion, b_os->os->basebandVersion);
    else
        return key_SAP;
}

void normalize_os(const char os_file_name[]) {
    fseek(osFile, 0L, SEEK_END);
    int number_of_os = ftell(osFile) / sizeof(struct OsCell);
    fseek(osFile, 0L, SEEK_SET);

    struct OsCell os_array[number_of_os];

    //  Read all OSes
    for (int i = 0; i < number_of_os; i++) {
        struct OsCell *os_cell;
        // TODO Write proper file function
        fread(&os_cell, sizeof(os_cell), 1, osFile);
        os_array[i] = *os_cell;
    }

    // Sort by Vendor's SAP and by baseband version
    qsort(os_array, number_of_os, sizeof(struct OsCell), compare_os);

    fclose(osFile);
    osFile = fopen(os_file_name, "wb");
    //  TODO Check if it writes all elements
    // TODO Write proper file function
    fwrite(os_array, sizeof(struct OsCell), number_of_os, osFile);
    fclose(osFile);
}

void normalize_index(const char index_file_name[]) {
    fclose(indexFile);
    indexFile = fopen(index_file_name, "wb");
    for (int i = 0; i < numberOfEntries; i++) {
        // TODO Write proper file function
        fwrite(indexes[i].SAP, sizeof(indexes[i].SAP), 1, indexFile);
        fwrite(&indexes[i].index, sizeof(indexes[i].index), 1, indexFile);
    }
    fclose(indexFile);
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
    initFields(vendor_file_name, index_file_name, os_file_name);
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
    fclose(vendorFile);
    fclose(osFile);
    fclose(indexFile);
}

const struct db DB = {
        // User functions
        .addOs = add_os,
        .addVendor = addVendor,
        .getAllOs = getAllOs,
        .getOs = getOs,
        .getVendor = getVendor,
        .removeOs = remove_os,
        .removeVendor = remove_vendor,
        .updateOs = update_os,
        .updateVendor = update_vendor,
        // Helping functions
        .initFiles = initFields,
        .normalize = normalize,
        .end = end
};

// struct VendorCell(*addVendor)(struct Vendor vendor);
// struct Os(*addOs)(struct Os os, char);
// struct Vendor(*getVendor)(char key[5]);    //  Key - SAP code
// struct Os(*getOs)(char key[8]);            //  Key - baseband version
// struct Os*(*getAllOs)(char key[5]);       //  Key - linked vendor's S
// void(*removeVendor)(char key[5]);          //  Key - SAP code
// void(*removeOs)(char key[8]);              //  Key - baseband version
// void(*updateVendor)(char key[5]);          //  Key - SAP code
// void(*updateOs)(char key[8]);              //  Key - baseband version