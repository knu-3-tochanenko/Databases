#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Index readIndex(FILE *file) {
    char SAP[6];
    long long index;

    fread(SAP, sizeof(char), 6, file);
    fread(&index, sizeof(long long), 1, file);

    struct Index elem;
    elem.index = index;
    strncpy(elem.SAP, SAP, 6);
    return elem;
}

void writeIndex(FILE *file, struct Index index) {
    fwrite(&index, sizeof(struct Index), 1, file);
}

struct Vendor readVendor(FILE *file) {
    char SAP[6];
    char name[33];
    char countryCode[4];

    fread(SAP, sizeof(char), 6, file);
    fread(name, sizeof(char), 33, file);
    fread(countryCode, sizeof(char), 4, file);

    struct Vendor vendor;
    strncpy(vendor.SAP, SAP, 6);
    strncpy(vendor.name, name, 33);
    strncpy(vendor.countryCode, countryCode, 4);

    return vendor;
}

void writeVendor(FILE* file, struct Vendor vendor) {
    fwrite(&vendor, sizeof(struct Vendor), 1, file);
}

struct VendorCell readVendorCell(FILE *file) {
    struct Vendor vendor = readVendor(file);
    long long connectedTo;
    int numberOfConnected;
    bool isDeleted;

    fread(&connectedTo, sizeof(long long), 1, file);
    fread(&numberOfConnected, sizeof(int), 1, file);
    fread(&isDeleted, sizeof(bool), 1, file);

    struct VendorCell vendorCell;
    vendorCell.vendor = vendor;
    vendorCell.connectedTo = connectedTo;
    vendorCell.numberOfConnected = numberOfConnected;
    vendorCell.isDeleted = isDeleted;

    return vendorCell;
}

void writeVendorCell(FILE* file, struct VendorCell vendorCell) {
//    writeVendor(file, vendorCell.vendor);
    fwrite(&vendorCell, sizeof(struct VendorCell), 1, file);
}

struct Os readOs(FILE *file) {
    char basebandVersion[9]; // Key
    int buildNumber;
    int androidVersion;
    char name[33];
    int buildDate;
    char vendorSAP[6];   // Key to it's vendor

    fread(basebandVersion, sizeof(char), 9, file);
    fread(&buildNumber, sizeof(int), 1, file);
    fread(&androidVersion, sizeof(int), 1, file);
    fread(name, sizeof(char), 33, file);
    fread(&buildDate, sizeof(int), 1, file);
    fread(vendorSAP, sizeof(char), 6, file);

    struct Os os;
    strncpy(os.basebandVersion, basebandVersion, 9);
    os.buildNumber = buildNumber;
    os.androidVersion = androidVersion;
    strncpy(os.name, name, 33);
    os.buildDate = buildDate;
    strncpy(os.vendorSAP, vendorSAP, 6);

    return os;
}

void writeOs(FILE* file, struct Os os) {
    fwrite(&os, sizeof(struct Os), 1, file);
}

struct OsCell readOsCell(FILE *file) {
    struct Os os = readOs(file);
    bool isDeleted;
    fread(&isDeleted, sizeof(bool), 1, file);

    struct OsCell osCell;
    osCell.os = os;
    osCell.isDeleted = isDeleted;

    return osCell;
}

void writeOsCell(FILE* file, struct OsCell osCell) {
    fwrite(&osCell, sizeof(struct OsCell), 1, file);
}

void fileEnd(FILE *file) {
    fseek(file, 0L, SEEK_END);
}

void fileStart(FILE *file) {
    fseek(file, 0L, SEEK_SET);
}

void fileSet(FILE *file, long pos) {
    fseek(file, pos, SEEK_SET);
}

void initFields(
        const char *vendorFileName,
        const char *indexFileName,
        const char *osFileName
) {
    vendorFile = fopen(vendorFileName, "r+b");
    if (vendorFile == NULL) {
        printf("\nError while opening vendor file with name %s!\nCreating new file\n", vendorFileName);
        vendorFile = fopen(vendorFileName, "w+b");
    }
    indexFile = fopen(indexFileName, "r+b");
    if (indexFile == NULL) {
        printf("\nError while opening index file with name %s!\nCreating new file\n", indexFileName);
        indexFile = fopen(indexFileName, "w+b");
    }
    osFile = fopen(osFileName, "r+b");
    if (osFile == NULL) {
        printf("\nError while opening os file with name %s!\nCreating new file\n", osFileName);
        osFile = fopen(osFileName, "w+b");
    }

    //  Read all indexes to memory
    fileEnd(indexFile);
    numberOfEntries = ftell(indexFile) / (sizeof(struct Index));
    printf("\n??? %ld ???\n", numberOfEntries);
    fileStart(indexFile);
    struct Index sub;
    for (long long i = 0; i < numberOfEntries; i++)
        indexes[i] = readIndex(indexFile);
}

struct Vendor getVendor(char *key) {
    DB.normalize("vendor.fl", "os.fl", "index.ind");
    for (int i = 0; i < numberOfEntries; i++) {
        if (strcmp(indexes[i].SAP, key) == 0) {
            fseek(vendorFile, indexes[i].index, SEEK_SET);
            struct VendorCell vendorCell = readVendorCell(vendorFile);
            return vendorCell.vendor;
        }
    }
    return (struct Vendor) {
        .SAP = "UNDEF\000",
        .name = "UNDEFINED\000",
        .countryCode = "UND\000"
    };
}

void getOs(char *key, struct Os *os) {
    os = NULL; // Default value if not found
    fileEnd(osFile);
    long long numberOfOs = ftell(osFile) /
                                    (sizeof(struct Os) + sizeof(bool));
    fileStart(osFile);

    struct OsCell osCell;
    for (long long i = 0; i < numberOfOs; i++) {
        osCell = readOsCell(osFile);
        if (strcmp(osCell.os.basebandVersion, key) == 0) {
            os = &osCell.os;
            return;
        }
    }
}

// TODO Change method
int getAllOs(char key[6], struct Os os[]) {
    fileEnd(osFile);
    long long numberOfOs = ftell(osFile) /
                                    (sizeof(struct Os) + sizeof(bool));
    fileStart(osFile);

    os = malloc(numberOfOs * sizeof(struct Os));

    for (long long i = 0; i < numberOfOs; i++)
        os[i] = readOsCell(osFile).os;
    return numberOfOs;
}

bool addVendor(struct Vendor vendor) {
    const struct VendorCell newVendor = {
            .vendor = vendor,
            .connectedTo = 0,
            .numberOfConnected = 0,
            .isDeleted = false
    };
    struct Vendor vendorFound = getVendor(vendor.SAP);
    if (strncmp(vendorFound.SAP, "UNDEF\000", 6) == 0) {
        fileEnd(vendorFile);
        long long pos = ftell(vendorFile);
        writeVendorCell(vendorFile, newVendor);

        struct Index index;
        index.index = pos;
        strncpy(index.SAP, vendor.SAP, 6);

        writeIndex(indexFile, index);
        indexes[numberOfEntries] = index;
        numberOfEntries++;
        return true;
    }
    return false;
}

bool addOs(struct Os os) {
    const struct OsCell newOs = {
            .os = os,
            .isDeleted = 0
    };
    struct Os* osRes = NULL;
    getOs(os.basebandVersion, osRes);
    if (osRes == NULL) {
        fileEnd(osFile);
        writeOsCell(osFile, newOs);
        return true;
    }
    return false;
}

static int compareVendor(const void *a, const void *b) {
    struct VendorCell *aVendor = (struct VendorCell *) a;
    struct VendorCell *bVendor = (struct VendorCell *) b;
    return strcmp(aVendor->vendor.SAP, bVendor->vendor.SAP);
}

void normalizeVendor(const char *vendorFileName) {
    fseek(vendorFile, 0L, SEEK_END);
    long long numberOfVendors = ftell(vendorFile) /
                                         (sizeof(struct Vendor) + sizeof(long long) + sizeof(int) +
                                          sizeof(bool));
    fseek(vendorFile, 0, SEEK_SET);

    struct VendorCell vendorArray[numberOfVendors];
    struct VendorCell vendorCell;
    int totalVendors = 0;

    for (long long i = 0; i < numberOfVendors; i++) {
        vendorCell = readVendorCell(vendorFile);

        // Write vendor if still used
        if (!vendorCell.isDeleted) {
            vendorArray[totalVendors] = vendorCell;
            totalVendors++;
        }
    }

    // Sort vendors
    qsort(vendorArray, totalVendors, sizeof(struct VendorCell), compareVendor);

    // Update index table
    numberOfEntries = totalVendors;
    for (long long i = 0; i < totalVendors; i++) {
        strncpy(indexes[i].SAP, vendorArray[i].vendor.SAP, 5);
        indexes[i].index = i * (sizeof(struct Vendor) + sizeof(long long) + sizeof(int) + sizeof(bool));
    }


    fclose(vendorFile);
    // Connect Vendors to os
    fseek(osFile, 0L, SEEK_END);
    printf("\n!!!! %ld !!!!\n", ftell(osFile));
    long long numberOfOs = ftell(osFile) /
                                    (sizeof(struct OsCell));
    fseek(osFile, 0L, SEEK_SET);
    printf("\n!!!! %ld !!!!\n", ftell(osFile));
    struct Os os;
    int currentVendor = 0;
    int currentOs = 0;
    int osPosition = 0;
    int compResult;

    struct OsCell osCell;
    while (currentOs < numberOfOs) {
        osPosition = ftell(osFile);
        osCell = readOsCell(osFile);
        compResult = strcmp(os.vendorSAP, vendorArray[currentVendor].vendor.SAP);
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
    vendorFile = fopen(vendorFileName, "w+b");
    for (long long i = 0; i < totalVendors; i++)
        writeVendorCell(vendorFile, vendorArray[i]);
    fclose(vendorFile);
    vendorFile = fopen(vendorFileName, "r+b");
}

static int compareOs(const void *a, const void *b) {
    struct OsCell *aOs = (struct OsCell *) a;
    struct OsCell *bOs = (struct OsCell *) b;
    int keySAP = strcmp(aOs->os.vendorSAP, bOs->os.vendorSAP);
    if (keySAP == 0)
        return strcmp(aOs->os.basebandVersion, bOs->os.basebandVersion);
    else
        return keySAP;
}

void normalizeOs(const char *osFileName) {
    fileEnd(osFile);
    long long numberOfOs = ftell(osFile) / sizeof(struct OsCell);
    fileStart(osFile);

    struct OsCell osArray[numberOfOs];

    //  Read all OSes
    struct OsCell osCell;
    int totalOs = 0;
    for (long long i = 0; i < numberOfOs; i++) {
        osCell = readOsCell(osFile);
        if (!osCell.isDeleted) {
            osArray[totalOs] = osCell;
            totalOs++;
        }
    }

    // Sort by Vendor's SAP and by baseband version
    qsort(osArray, totalOs, sizeof(struct OsCell), compareOs);

    fclose(osFile);
    osFile = fopen(osFileName, "w+b");
    if (osFile == NULL)
        printf("You are fucked, %s", osFileName);
    for (int i = 0; i < totalOs; i++) {
        writeOsCell(osFile, osArray[i]);
    }
    fclose(osFile);
    osFile = fopen(osFileName, "r+b");
}

void normalizeIndex(const char *index_file_name) {
    fclose(indexFile);
    indexFile = fopen(index_file_name, "w+b");
    for (long long i = 0; i < numberOfEntries; i++) {
        writeIndex(indexFile, indexes[i]);
    }
    fclose(indexFile);
    indexFile = fopen(index_file_name, "r+b");
}

//  Clears all unused fields and rewrites files.
//  Reopens files when done.
void normalize(
        const char *vendor_file_name,
        const char *index_file_name,
        const char *os_file_name
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