#include "get.h"

int getVendor(char *ptr, FILE **vendorFile) {
    long id;

    if (ptr != NULL) {
        char *pEnd;
        id = strtol(ptr, &pEnd, 10);
        ptr = strtok(NULL, " ");
        setbuf(stdout, 0);
        printf("%ld", id);
        if (ptr != NULL)
            return -1;
    } else {
        return -1;
    }

    int index = 0;
    if ((index = getVendorSAP(id, vendorFile)) == -1) {
        setbuf(stdout, 0);
        printf("Vendor with given SAP is not available");
        return -1;
    } else {
        struct Vendor *vendor = malloc(sizeof(struct Vendor));
        fseek(*vendorFile, sizeof(struct Vendor) * index, SEEK_SET);
        fread(vendor, sizeof(struct Vendor), 1, *vendorFile);
        writeVendor(vendor);
        free(vendor);
        return index;
    }
}

int getOs(char *ptr, FILE **vendorFile, FILE **osFile) {
    long vendorSAP = 0, osBasebandVersion = 0;

    if (ptr != NULL) {
        char *pEnd;
        vendorSAP = strtol(ptr, &pEnd, 10);
        ptr = strtok(NULL, " ");
        if (ptr == NULL)
            return -1;
        else {
            osBasebandVersion = strtol(ptr, &pEnd, 10);
            ptr = strtok(NULL, " ");
            if (ptr != NULL)
                return -1;
        }
    } else {
        return -1;
    }

    int index = 0;
    if ((index = getVendorSAP(vendorSAP, vendorFile)) == -1) {
        setbuf(stdout, 0);
        printf("Vendor does not exist.");
        return -1;
    } else {
        int indexElem = getOsIndex(index, vendorFile);
        printf("%i", indexElem);
        if (indexElem == -1)
            return -1;
        struct Os *os = malloc(sizeof(struct Os));
        // TODO What is status?
        unsigned int status = 0;
        while (indexElem != -1) {
            fseek(*osFile, sizeof(struct Os) * indexElem, SEEK_SET);
            fread(os, sizeof(struct Os), 1, *osFile);
            fread(&status, sizeof(unsigned int), 1, *osFile);
            if (os->basebandVersion == osBasebandVersion && status == 1) {
                writeOs(os);
                free(os);
                return indexElem;
            }
            indexElem = os->nextIndex;
        }
        free(os);
        return -1;
    }
}
