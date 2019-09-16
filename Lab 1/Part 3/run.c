#include "run.h"

bool function(const char masterFName[25], const char indexTableFName[25], const char slaveFName[25]) {
    //open or create database files
    FILE *masterFile = NULL, *indexTable = NULL, *slaveFile = NULL;
    if (!openFile(masterFName, &masterFile))
        return false;
    if (!openFile(indexTableFName, &indexTable))
        return false;
    if (!openFile(slaveFName, &slaveFile))
        return false;

    initializeTable();
    readTable(&indexTable);

    if (!listen(&masterFile, &indexTable, &slaveFile))
        return false;

    rewrite(masterFName, indexTableFName, slaveFName,
            &masterFile, &indexTable, &slaveFile);

    return true;
}


bool openFile(const char fName[25], FILE **ptr) {
    *ptr = fopen(fName, "r+b");
    if (*ptr == NULL) {
        setbuf(stdout, 0);
        printf("File %s does not exist!\n", fName);
        setbuf(stdout, 0);
        printf("Creating new %s file...\n", fName);
        *ptr = fopen(fName, "w+b");
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

bool listen(FILE **masterFile, FILE **indexFile, FILE **slaveFile) {
    char buffer[100];
    while (gets(buffer)) {
        char *ptr = strtok(buffer, " ");
        if (ptr == NULL)
            continue;
        else if (strcmp(ptr, "end") == 0)
            break;
        else if (strcmp(ptr, "insert-m") == 0) {
            ptr = strtok(NULL, " ");
            if (insert_m(ptr, masterFile))
                continue;
            else
                printf("Error!");
        } else if (strcmp(ptr, "insert-s") == 0) {
            ptr = strtok(NULL, " ");
            if (insert_s(ptr, masterFile, slaveFile))
                continue;
            else
                printf("Error!");
        } else if (strcmp(ptr, "get-m") == 0) {
            ptr = strtok(NULL, " ");
            get_m(ptr, masterFile);
            continue;
        } else if (strcmp(ptr, "get-s") == 0) {
            ptr = strtok(NULL, " ");
            get_s(ptr, masterFile, slaveFile);
            continue;
        } else if (strcmp(ptr, "del-m") == 0) {
            ptr = strtok(NULL, " ");
            if (del_m(ptr, masterFile))
                continue;
            else
                printf("Wrong command!");
        } else if (strcmp(ptr, "del-s") == 0) {
            ptr = strtok(NULL, " ");
            if (del_s(ptr, masterFile, slaveFile))
                continue;
            else
                printf("Wrong command!");
        } else if (strcmp(ptr, "update-m") == 0) {
            ptr = strtok(NULL, " ");
            if (update_m(ptr, masterFile))
                continue;
            else
                printf("Wrong command!");
        } else if (strcmp(ptr, "update-s") == 0) {
            ptr = strtok(NULL, " ");
            if (update_s(ptr, masterFile, slaveFile))
                continue;
            else
                printf("Error!");
        } else if (strcmp(ptr, "count-m") == 0) {
            ptr = strtok(NULL, " ");
            setbuf(stdout, 0);
            printf("Number of cells in master file: %i", count_m(ptr, masterFile));
            continue;
        } else if (strcmp(ptr, "count-s") == 0) {
            ptr = strtok(NULL, " ");
            printf("Number of cells for user in slave file: %i", count_s(ptr, slaveFile));
            continue;
        } else if (strcmp(ptr, "count-all") == 0) {
            ptr = strtok(NULL, " ");
            printf("Number of cells in slave file: %i", count_all(ptr, slaveFile));
            continue;
        } else {
            setbuf(stdout, 0);
            printf("Wrong command!");
            continue;
        }
    }
    return true;
}

void rewrite(const char masterFName[25], const char indexTableFName[25], const char slaveFName[25],
             FILE **masterFile, FILE **indexFile, FILE **slaveFile) {

    FILE *newMaster = fopen("master.fl", "w+");
    FILE *newIndex = fopen("master.ind", "w+");

    fclose(*masterFile);
    fclose(*indexFile);
    fclose(*slaveFile);

    remove(masterFName);
    remove(indexTableFName);

    fclose(newMaster);
    fclose(newIndex);
    rename("master.fl", masterFName);
    rename("master.ind", indexTableFName);
}
