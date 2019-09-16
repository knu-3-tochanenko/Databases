#include "application.h"
#include "database.h"
#include "indexTable.h"

bool runnable(const char *vendorFileName, const char *indexFileName, const char *osFileName) {
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


bool openFile(const char fileName[25], FILE **ptr) {
    *ptr = fopen(fileName, "r+b");
    if (*ptr == NULL) {
        setbuf(stdout, 0);
        printf("File %s does not exist!\n", fileName);
        setbuf(stdout, 0);
        printf("Creating new %s file...\n", fileName);
        *ptr = fopen(fileName, "w+b");
        if (*ptr == NULL) {
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
    char buffer[100];
    while (gets(buffer)) {
        char *ptr = strtok(buffer, " ");
        if (ptr == NULL)
            continue;
        else if (strcmp(ptr, "end") == 0)
            break;
        else if (strcmp(ptr, "-add-v") == 0) {
            ptr = strtok(NULL, " ");
            if (insertVendor(ptr, vendorFile))
                continue;
            else
                printf("Error!");
        } else if (strcmp(ptr, "-add-o") == 0) {
            ptr = strtok(NULL, " ");
            if (insertOs(ptr, vendorFile, osFile))
                continue;
            else
                printf("Error!");
        } else if (strcmp(ptr, "-get-v") == 0) {
            ptr = strtok(NULL, " ");
            getVendor(ptr, vendorFile);
            continue;
        } else if (strcmp(ptr, "-get-o") == 0) {
            ptr = strtok(NULL, " ");
            getOs(ptr, vendorFile, osFile);
            continue;
        } else if (strcmp(ptr, "removeIndex-m") == 0) {
            ptr = strtok(NULL, " ");
            if (removeVendor(ptr, vendorFile, osFile))
                continue;
            else
                printf("Wrong command!");
        } else if (strcmp(ptr, "-rmi-v") == 0) {
            ptr = strtok(NULL, " ");
            if (removeOs(ptr, vendorFile, osFile))
                continue;
            else
                printf("Wrong command!");
        } else if (strcmp(ptr, "-update-v") == 0) {
            ptr = strtok(NULL, " ");
            if (updateVendor(ptr, vendorFile))
                continue;
            else
                printf("Wrong command!");
        } else if (strcmp(ptr, "-update-o") == 0) {
            ptr = strtok(NULL, " ");
            if (updateOs(ptr, vendorFile, osFile))
                continue;
            else
                printf("Error!");
        } else if (strcmp(ptr, "-count-v") == 0) {
            ptr = strtok(NULL, " ");
            setbuf(stdout, 0);
            printf("Number of cells in vendor file: %i", countVendor(ptr, vendorFile));
            continue;
        } else if (strcmp(ptr, "-count-o") == 0) {
            ptr = strtok(NULL, " ");
            printf("Number of cells for user in os file: %i", countOs(ptr, osFile));
            continue;
        } else if (strcmp(ptr, "-count-all") == 0) {
            ptr = strtok(NULL, " ");
            printf("Number of cells in os file: %i", countAllOs(ptr, osFile));
            continue;
        } else {
            setbuf(stdout, 0);
            printf("Wrong command!\n");
            continue;
        }
    }
    return true;
}

void rewrite(const char vendorFileName[25], const char indexFileName[25], const char osFileName[25],
             FILE **vendorFile, FILE **indexFile, FILE **osFile) {
    writeTable(indexFile, indexFileName);
    fclose(*vendorFile);
    fclose(*indexFile);
    fclose(*osFile);
}
