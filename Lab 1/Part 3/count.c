#include "count.h"

unsigned int count_m(char *ptr, FILE **masterFile) {
    if (ptr != NULL) {
        setbuf(stdout, 0);
        printf("Wrong command.");
        return false;
    }
    unsigned int status = 0, i = 0;
    struct Vendor *contributor = malloc(sizeof(struct Vendor));
    fseek(*masterFile, 0, SEEK_SET);
    while (fread(contributor, sizeof(struct Vendor), 1, *masterFile)) {
        fread(&status, sizeof(int), 1, *masterFile);
        if (status == 1) {
            i++;
        }
    }
    return i;
}

unsigned int count_s(char *ptr, FILE **slaveFile) {
    unsigned long contributorID = 0;

    if (ptr != NULL) {
        char *pEnd;
        contributorID = strtol(ptr, &pEnd, 10);
        ptr = strtok(NULL, " ");
        if (ptr != NULL)
            return false;
    } else {
        return false;
    }

    struct Os *image = malloc(sizeof(struct Os));

    unsigned int status = 0, i = 0;
    fseek(*slaveFile, 0, SEEK_SET);
    while (fread(image, sizeof(struct Os), 1, *slaveFile)) {
        fread(&status, sizeof(int), 1, *slaveFile);
        if (status == 1 && image->SAP == contributorID)
            i++;
    }
    return i;
}

unsigned int count_all(char *ptr, FILE **slaveFile) {
    if (ptr != NULL) {
        setbuf(stdout, 0);
        printf("Wrong command.");
        return false;
    }
    struct Os *image = malloc(sizeof(struct Os));
    unsigned int status = 0, i = 0;
    fseek(*slaveFile, 0, SEEK_SET);
    while (fread(image, sizeof(struct Os), 1, *slaveFile)) {
        fread(&status, sizeof(int), 1, *slaveFile);
        if (status == 1)
            i++;
    }
    return i;
}