#include "insert.h"

int IMAGE_NUMBER = -1;

bool addVendor(char *ptr, FILE **vendorFile) {
    if (ptr != NULL) {
        setbuf(stdout, 0);
        printf("Wrong command.");
        return false;
    }

    char vendorSAP[21];
    unsigned long SAP = 0;
    setbuf(stdout, 0);
    printf("\nEnter Vendor SAP:");
    scanf("%s", vendorSAP);
    SAP = strtol(vendorSAP, NULL, 10);

    if (getVendorSAP(SAP, vendorFile) != -1) {
        setbuf(stdout, 0);
        printf("Vendor with given SAP already exists");
        return addVendor(ptr, vendorFile);
    } else {
        struct Vendor *vendor = readVendor();
        vendor->SAP = SAP;

        fseek(*vendorFile, 0, SEEK_END);
        long index = ftell(*vendorFile) / (sizeof(struct Vendor) + sizeof(int));
        writeVendorToFile(vendor, vendorFile);
        add(SAP, index);
        free(vendor);
        return true;
    }

}


bool addOs(char *ptr, FILE **vendorFile, FILE **osFile) {
    unsigned long vendorSAP = 0;
    char *tmp = ptr;

    if (ptr != NULL) {
        char *pEnd;
        vendorSAP = strtol(ptr, &pEnd, 10);
        ptr = strtok(NULL, " ");
        setbuf(stdout, 0);
        printf("%ld", vendorSAP);
        if (ptr != NULL)
            return false;
    } else {
        return false;
    }

    int index = 0, imageIndex = -1;
    if ((index = getVendorSAP(vendorSAP, vendorFile)) == -1) {
        setbuf(stdout, 0);
        printf("Vendor does not exist.");
        return false;
    } else {

        char SAP[21];
        unsigned long id = 0;

        setbuf(stdout, 0);
        printf("\nEnter Os ID:");
        scanf("%s", SAP);
        id = strtol(SAP, NULL, 10);

        if (getOsVersion(id, osFile) != -1) {
            setbuf(stdout, 0);
            printf("ID exists. Try to enter another one");
            return addOs(tmp, vendorFile, osFile);
        } else {
            struct Os *os = readOs();
            os->SAP = vendorSAP;
            os->basebandVersion = id;

            fseek(*osFile, 0, SEEK_END);
            long end = ftell(*osFile) / (sizeof(struct Os) + sizeof(int));
            os->nextIndex = getOsIndex(index, vendorFile);
            setImageIndex(index, end, vendorFile);
            writeOsToFile(os, osFile);
            free(os);
            return true;
        }
    }
}