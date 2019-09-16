#include "run.h"

bool function(const char vendorFileName[25], const char indexFileName[25], const char osFileName[25]) {
    //open or create database files
    FILE *vendorFile = NULL, *indexTable = NULL, *osFile = NULL;
    if (!openFile(vendorFileName, &vendorFile))
        return false;
    if (!openFile(indexFileName, &indexTable))
        return false;
    if (!openFile(osFileName, &osFile))
        return false;

    initializeTable();
    readTable(&indexTable);

    if (!listen(&vendorFile, &indexTable, &osFile))
        return false;

    rewrite(vendorFileName, indexFileName, osFileName,
            &vendorFile, &indexTable, &osFile);

    return true;
}


bool openFile(const char fileName[25], FILE **file) {
    *file = fopen(fileName, "r+b");
    if (*file == NULL) {
        setbuf(stdout, 0);
        printf("File %s does not exist!\n", fileName);
        setbuf(stdout, 0);
        printf("Creating new %s file...\n", fileName);
        *file = fopen(fileName, "w+b");
        if (*file == NULL) {
            setbuf(stdout, 0);
            printf("Creation failed.\n");
            return false;
        }
        setbuf(stdout, 0);
        printf("Creation successed.\n");
    }
    return true;
}

bool listen(FILE **vendorFile, FILE **indexFile, FILE **osFile) {
    // TODO Change CLI a bit
    char buffer[100];
    while (gets(buffer)) {
        char *ptr = strtok(buffer, " ");
        if (ptr == NULL)
            continue;
        else if (strcmp(ptr, "end") == 0)
            break;
        else if (strcmp(ptr, "insert-m") == 0) {
            ptr = strtok(NULL, " ");
            if (addVendor(ptr, vendorFile))
                continue;
            else
                printf("Error!");
        } else if (strcmp(ptr, "insert-s") == 0) {
            ptr = strtok(NULL, " ");
            if (addOs(ptr, vendorFile, osFile))
                continue;
            else
                printf("Error!");
        } else if (strcmp(ptr, "get-m") == 0) {
            ptr = strtok(NULL, " ");
            getVendor(ptr, vendorFile);
            continue;
        } else if (strcmp(ptr, "get-s") == 0) {
            ptr = strtok(NULL, " ");
            getOs(ptr, vendorFile, osFile);
            continue;
        } else if (strcmp(ptr, "del-m") == 0) {
            ptr = strtok(NULL, " ");
            if (del_m(ptr, vendorFile))
                continue;
            else
                printf("Wrong command!");
        } else if (strcmp(ptr, "del-s") == 0) {
            ptr = strtok(NULL, " ");
            if (del_s(ptr, vendorFile, osFile))
                continue;
            else
                printf("Wrong command!");
        } else if (strcmp(ptr, "update-m") == 0) {
            ptr = strtok(NULL, " ");
            if (updateVendor(ptr, vendorFile))
                continue;
            else
                printf("Wrong command!");
        } else if (strcmp(ptr, "update-s") == 0) {
            ptr = strtok(NULL, " ");
            if (updateOs(ptr, vendorFile, osFile))
                continue;
            else
                printf("Error!");
        } else if (strcmp(ptr, "count-m") == 0) {
            ptr = strtok(NULL, " ");
            setbuf(stdout, 0);
            printf("Number of cells in master file: %i", count_m(ptr, vendorFile));
            continue;
        } else if (strcmp(ptr, "count-s") == 0) {
            ptr = strtok(NULL, " ");
            printf("Number of cells for user in slave file: %i", count_s(ptr, osFile));
            continue;
        } else if (strcmp(ptr, "count-all") == 0) {
            ptr = strtok(NULL, " ");
            printf("Number of cells in slave file: %i", count_all(ptr, osFile));
            continue;
        } else {
            setbuf(stdout, 0);
            printf("Wrong command!");
            continue;
        }
    }
    return true;
}

void rewrite(const char vendorFileName[25], const char indexFileName[25], const char osFileName[25],
             FILE **vendorFile, FILE **indexFile, FILE **osFile) {

    FILE *newVendorFile = fopen("vendor.fl", "w+");
    FILE *newIndexFile = fopen("vendor.ind", "w+");

    fclose(*vendorFile);
    fclose(*indexFile);
    fclose(*osFile);

    remove(vendorFileName);
    remove(indexFileName);

    fclose(newVendorFile);
    fclose(newIndexFile);
    rename("vendor.fl", vendorFileName);
    rename("vendor.ind", indexFileName);
}
