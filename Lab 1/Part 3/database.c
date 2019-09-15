#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Index readIndex(FILE* file) {
    char SAP[5];
    unsigned long long index;
    fread(SAP, sizeof(char) * 5, 1, file);
    fread(&index, sizeof(unsigned long long), 1, file);
    struct Index elem;
    elem.index = index;
    //strn
}

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
    numberOfEntries = ftell(indexFile) / (sizeof(char) * 5 + sizeof(unsigned long long));
    fseek(indexFile, 0L, SEEK_SET);
    struct Index sub;
    for (unsigned long long i = 0; i < numberOfEntries; i++) {
        fread(sub, sizeof(sub), 1, indexFile);
        indexes[i] = *sub;
    }
}

struct Vendor *getVendor(char *key) {
    struct Vendor *found = NULL;
    for (int i = 0; i < numberOfEntries; i++) {
        if (strcmp(indexes[i].SAP, key) == 0) {
            fseek(vendorFile, indexes[i].index, SEEK_SET);
            // init fields
            struct VendorCell vendorCell;
            struct Vendor *vendor;
            unsigned long long connectedTo;
            int numberOfConnected;
            bool isDeleted;
            // Read all fields from file
            fread(vendor, sizeof(struct Vendor), 1, vendorFile);
            fread(&connectedTo, sizeof(unsigned long long), 1, vendorFile);
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

struct Os *getOs(char *key) {
    fseek(osFile, 0L, SEEK_END);
    unsigned long long numberOfOs = ftell(osFile) / (sizeof(struct Os) + sizeof(bool));
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

struct Os *getAllOs() {
    fseek(osFile, 0L, SEEK_END);
    unsigned long long numberOfOs = ftell(osFile) / (sizeof(struct Os) + sizeof(bool));
    fseek(osFile, 0L, SEEK_SET);

    struct Os array[numberOfOs];
    struct Os *os;
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
        indexes[numberOfEntries] = (struct Index) {
                .SAP = *newVendor.vendor->SAP,
                .index = pos
        };
        numberOfEntries++;
        return true;
    }
    return false;
}

bool addOs(struct Os *os) {
    const struct OsCell newOs = {
            .os = os,
            .isDeleted = 0
    };

    if (getOs(os->basebandVersion) == NULL) {
        fseek(osFile, 0L, SEEK_END);
        // idk what this is supposet to do
        // int pos = ftell(osFile);
        fwrite(os, sizeof(struct Os), 1, osFile);
        fwrite(&newOs.isDeleted, sizeof(bool), 1, osFile);
        return true;
    }
    return false;
}

static int compareVendor(const void *a, const void *b) {
    struct VendorCell *aVendor = (struct VendorCell *) a;
    struct VendorCell *bVendor = (struct VendorCell *) b;
    return strcmp(aVendor->vendor->SAP, bVendor->vendor->SAP);
}

void normalizeVendor(const char *vendorFileName) {
    fseek(vendorFile, 0L, SEEK_END);
    unsigned long long numberOfVendors = ftell(vendorFile) /
                                         (sizeof(struct Vendor) + sizeof(unsigned long long) + sizeof(int) + sizeof(bool));
    fseek(vendorFile, 0, SEEK_SET);

    struct VendorCell vendorArray[numberOfVendors];
    struct Vendor vendor;
    unsigned long long connectedTo;
    int numberOfConnected;
    bool isDeleted;
    int totalVendors = 0;

    for (unsigned long long i = 0; i < numberOfVendors; i++) {
        fread(&vendor, sizeof(struct Vendor), 1, vendorFile);
        fread(&connectedTo, sizeof(unsigned long long), 1, vendorFile);
        fread(&numberOfConnected, sizeof(int), 1, vendorFile);
        fread(&isDeleted, sizeof(bool), 1, vendorFile);

        // Write vendor if still used
        if (!isDeleted) {
            vendorArray[totalVendors] = (struct VendorCell) {
                    .vendor = &vendor,
                    .numberOfConnected = numberOfConnected,
                    .connectedTo = connectedTo,
                    .isDeleted = isDeleted
            };
            totalVendors++;
        }
    }

    // Sort vendors
    qsort(vendorArray, totalVendors, sizeof(struct VendorCell), compareVendor);

    // Update index table
    numberOfEntries = totalVendors;
    for (unsigned long long i = 0; i < totalVendors; i++) {
        strncpy(indexes[i].SAP, vendorArray[i].vendor->SAP, 5);
        indexes[i].index = i * (sizeof(struct Vendor) + sizeof(unsigned long long) + sizeof(int) + sizeof(bool));
    }


    fclose(vendorFile);
    // Connect Vendors to os
    fseek(osFile, 0L, SEEK_END);
    unsigned long long numberOfOs = ftell(osFile) /
                                    (sizeof(struct Os) + sizeof(bool));
    fseek(osFile, 0L, SEEK_SET);
    struct Os os;
    int currentVendor = 0;
    int currentOs = 0;
    int osPosition = 0;
    int compResult;
    while (currentOs < numberOfOs) {
        osPosition = ftell(osFile);
        fread(&os, sizeof(struct Os), 1, osFile);
        fread(&isDeleted, sizeof(bool), 1, osFile);
        compResult = strcmp(os.vendorSAP, vendorArray[currentVendor].vendor->SAP);
        if (compResult == 0) {
            if (vendorArray[currentVendor].numberOfConnected == 0) {
                vendorArray[currentVendor].numberOfConnected = 1;
                vendorArray[currentVendor].connectedTo = osPosition;
            } else
                vendorArray[currentVendor].numberOfConnected++;
        } else if (compResult < 0) {
            // This shouldn't happen
            // But it happened
        } else if (compResult > 0)
            currentVendor++;
        currentOs++;
    }

    // Write vendors in file
    vendorFile = fopen(vendorFileName, "wb");
    for (unsigned long long i = 0; i < totalVendors; i++) {
        fwrite(vendorArray[i].vendor, sizeof(struct Vendor), 1, vendorFile);
        fwrite(&(vendorArray[i].connectedTo), sizeof(unsigned long long), 1, vendorFile);
        fwrite(&(vendorArray[i].numberOfConnected), sizeof(int), 1, vendorFile);
        fwrite(&(vendorArray[i].isDeleted), sizeof(bool), 1, vendorFile);
    }
    fclose(vendorFile);
}

static int compareOs(const void *a, const void *b) {
    struct OsCell *aOs = (struct OsCell *) a;
    struct OsCell *bOs = (struct OsCell *) b;
    int keySAP = strcmp(aOs->os->vendorSAP, bOs->os->vendorSAP);
    if (keySAP == 0)
        return strcmp(aOs->os->basebandVersion, bOs->os->basebandVersion);
    else
        return keySAP;
}

void normalizeOs(const char *osFileName) {
    fseek(osFile, 0L, SEEK_END);
    unsigned long long numberOfOs = ftell(osFile) / sizeof(struct OsCell);
    fseek(osFile, 0L, SEEK_SET);

    struct OsCell osArray[numberOfOs];

    //  Read all OSes
    struct Os os;
    bool isDeleted;
    int totalOs = 0;
    for (unsigned long long i = 0; i < numberOfOs; i++) {
        fread(&os, sizeof(struct Os), 1, osFile);
        fread(&isDeleted, sizeof(bool), 1, osFile);
        if (!isDeleted) {
            osArray[totalOs] = (struct OsCell) {
                .isDeleted = isDeleted,
                .os = &os
            };
            totalOs++;
        }
    }

    // Sort by Vendor's SAP and by baseband version
    qsort(osArray, totalOs, sizeof(struct OsCell), compareOs);

    fclose(osFile);
    osFile = fopen(osFileName, "wb");
    for (int i = 0; i < totalOs; i++) {
        fwrite(osArray[i].os, sizeof(struct Os), 1, osFile);
        fwrite(&(osArray[i].isDeleted), sizeof(bool), 1, osFile);
    }
    fclose(osFile);
}

void normalizeIndex(const char *index_file_name) {
    fclose(indexFile);
    indexFile = fopen(index_file_name, "wb");
    for (unsigned long long i = 0; i < numberOfEntries; i++) {
        fwrite(indexes[i].SAP, sizeof(char) * 5, 1, indexFile);
        fwrite(&(indexes[i].index), sizeof(unsigned long long), 1, indexFile);
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
    normalizeOs(os_file_name);
    normalizeVendor(vendor_file_name);
    normalizeIndex(index_file_name);
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
        .addOs = addOs,
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