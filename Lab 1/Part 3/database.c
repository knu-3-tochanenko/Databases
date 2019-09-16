#include "database.h"
#include "inputoutput.h"

unsigned int countVendor(char *ptr, FILE **vendorFile) {
    if (ptr != NULL) {
        setbuf(stdout, 0);
        printf("Wrong command.");
        return false;
    }
    unsigned int status = 0, i = 0;
    struct Vendor *vendor = malloc(sizeof(struct Vendor));
    fseek(*vendorFile, 0, SEEK_SET);
    while (fread(vendor, sizeof(struct Vendor), 1, *vendorFile)) {
        fread(&status, sizeof(int), 1, *vendorFile);
        if (status == 1) {
            i++;
        }
    }
    return i;
}

unsigned int countOs(char *ptr, FILE **osFile) {
    unsigned long vendorSAP = 0;

    if (ptr != NULL) {
        char *pEnd;
        vendorSAP = strtol(ptr, &pEnd, 10);
        ptr = strtok(NULL, " ");
        if (ptr != NULL)
            return false;
    } else {
        return false;
    }

    struct Os *os = malloc(sizeof(struct Os));

    unsigned int status = 0, i = 0;
    fseek(*osFile, 0, SEEK_SET);
    while (fread(os, sizeof(struct Os), 1, *osFile)) {
        fread(&status, sizeof(int), 1, *osFile);
        if (status == 1 && os->vendorSAP == vendorSAP)
            i++;
    }
    return i;
}

unsigned int countAllOs(char *ptr, FILE **osFile) {
    if (ptr != NULL) {
        setbuf(stdout, 0);
        printf("Wrong command.");
        return false;
    }
    struct Os *os = malloc(sizeof(struct Os));
    unsigned int status = 0, i = 0;
    fseek(*osFile, 0, SEEK_SET);
    while (fread(os, sizeof(struct Os), 1, *osFile)) {
        fread(&status, sizeof(int), 1, *osFile);
        if (status == 1)
            i++;
    }
    return i;
}

bool removeVendor(char *ptr, FILE **vendorFile, FILE **osFile) {
    char *tmp = ptr;
    long vendorSAP = 0;
    if (ptr != NULL) {
        char *pEnd;
        vendorSAP = strtol(ptr, &pEnd, 10);
    }

    int index = getVendor(tmp, vendorFile);
    if (index == -1)
        return false;
    unsigned int status = 0;
    int lastos = getOsIndex(index, vendorFile);

    char cStr[200];
    ltoa(vendorSAP, cStr, 10);

    while (lastos != -1) {
        char tmp[50];
        itoa(lastos, tmp, 10);
        strcat(cStr, " ");
        strcat(cStr, tmp);
        char* newPtr = strtok(cStr, " ");
        removeOs(newPtr, vendorFile, osFile);
        fseek(*osFile, (sizeof(struct Os) + sizeof(int)) * (lastos + 1) - 2 * sizeof(int), SEEK_SET);
        fread(&lastos, sizeof(int), 1, *osFile);
    }

    removeIndex(vendorSAP);
    fseek(*vendorFile, (sizeof(struct Vendor) + sizeof(int)) * (index + 1) - sizeof(int), SEEK_SET);
    fwrite(&status, sizeof(unsigned int), 1, *vendorFile);
    return true;
}

bool removeOs(char *ptr, FILE **vendorFile, FILE **osFile) {
    char *tmp = ptr;
    long vendorSAP = 0;
    if (ptr != NULL) {
        char *pEnd;
        vendorSAP = strtol(ptr, &pEnd, 10);
    }

    int osIndex = getOs(tmp, vendorFile, osFile);
    if (osIndex == -1)
        return false;

    int vendorIndex = getVendorIndex(vendorSAP, vendorFile);
    int lastos = getOsIndex(vendorIndex, vendorFile);

    if (osIndex == -1)
        return false;

    unsigned int status = 0;
    fseek(*osFile, (sizeof(int) + sizeof(struct Os) * (osIndex + 1)) - sizeof(int), SEEK_SET);
    fwrite(&status, sizeof(unsigned int), 1, *osFile);

    if (lastos == osIndex) {
        int nextIndex = -1;
        fseek(*vendorFile, (sizeof(struct Vendor) + sizeof(int)) * (vendorIndex + 1) - 2 * sizeof(int),
              SEEK_SET);
        fseek(*osFile, (sizeof(struct Os) + sizeof(int)) * (osIndex + 1) - 2 * sizeof(int), SEEK_SET);
        fread(&nextIndex, sizeof(int), 1, *osFile);
        fwrite(&nextIndex, sizeof(int), 1, *vendorFile);
    } else {
        int prevIndex = lastos, nextIndex = lastos;
        while (prevIndex != -1) {
            prevIndex = nextIndex;
            fseek(*osFile, (sizeof(struct Os) + sizeof(int)) * (nextIndex + 1) - 2 * sizeof(int), SEEK_SET);
            fread(&nextIndex, sizeof(int), 1, *osFile);
            if (nextIndex == osIndex) {
                fseek(*osFile, (sizeof(int) + sizeof(struct Os)) * (nextIndex + 1) - 2 * sizeof(int), SEEK_SET);
                fread(&nextIndex, sizeof(int), 1, *osFile);
                fseek(*osFile, (sizeof(int) + sizeof(struct Os)) * (prevIndex + 1) - 2 * sizeof(int), SEEK_SET);
                fwrite(&nextIndex, sizeof(int), 1, *osFile);
            }
        }
    }
    return true;
}

int getVendor(char *ptr, FILE **vendorFile) {
    long SAP;

    if (ptr != NULL) {
        char *pEnd;
        SAP = strtol(ptr, &pEnd, 10);
        ptr = strtok(NULL, " ");
        if (ptr != NULL)
            return -1;
    } else {
        return -1;
    }

    int index = 0;
    if ((index = getVendorIndex(SAP, vendorFile)) == -1) {
        setbuf(stdout, 0);
        printf("SAP does not exist. Try to enter another one");
        return -1;
    } else {
        struct Vendor *vendor = malloc(sizeof(struct Vendor));
        fseek(*vendorFile, (sizeof(struct Vendor) + sizeof(int)) * index, SEEK_SET);
        fread(vendor, sizeof(struct Vendor), 1, *vendorFile);
        printVendor(vendor);
        free(vendor);
        return index;
    }
}

int getOs(char *ptr, FILE **vendorFile, FILE **osFile) {
    long vendorSAP = 0, osBaseband = 0;

    if (ptr != NULL) {
        vendorSAP = strtol(ptr, NULL, 10);
        ptr = strtok(NULL, " ");
        if (ptr == NULL)
            return -1;
        else {
            osBaseband = strtol(ptr, NULL, 10);
            ptr = strtok(NULL, " ");
            if (ptr != NULL)
                return -1;
        }
    } else {
        return -1;
    }

    int index = 0;
    if ((index = getVendorIndex(vendorSAP, vendorFile)) == -1) {
        setbuf(stdout, 0);
        printf("ID does not exist.");
        return -1;
    } else {
        int osIndex = getOsIndex(index, vendorFile);
        if (osIndex == -1)
            return -1;
        struct Os *os = malloc(sizeof(struct Os));
        unsigned int status = 0;
        while (osIndex != -1) {
            fseek(*osFile, (sizeof(struct Os) + sizeof(int)) * osIndex, SEEK_SET);
            fread(os, sizeof(struct Os), 1, *osFile);
            fread(&status, sizeof(unsigned int), 1, *osFile);
            if (os->baseband == osBaseband && status == 1) {
                printOs(os);
                free(os);
                return osIndex;
            }
            osIndex = os->nextIndex;
        }
        free(os);
        return -1;
    }
}

int os_NUMBER = -1;

bool insertVendor(char *ptr, FILE **vendorFile) {
    if (ptr != NULL) {
        setbuf(stdout, 0);
        printf("Wrong command.");
        return false;
    }

    char userID[21];
    unsigned long SAP = 0;
    setbuf(stdout, 0);
    printf("\nEnter Vendor SAP:");
    scanf("%s", userID);
    SAP = strtol(userID, NULL, 10);

    if (getVendorIndex(SAP, vendorFile) != -1) {
        setbuf(stdout, 0);
        printf("Vendor with sameSAP exists\n");
        return insertVendor(ptr, vendorFile);
    } else {
        struct Vendor *vendor = readVendor();
        vendor->SAP = SAP;
        vendor->cennoctedTo = -1;

        fseek(*vendorFile, 0, SEEK_END);
        long index = ftell(*vendorFile) / (sizeof(struct Vendor) + sizeof(int));
        writeVendor(vendor, vendorFile);
        addIndex(SAP, index);
        free(vendor);
        return true;
    }

}


bool insertOs(char *ptr, FILE **vendorFile, FILE **osFile) {
    unsigned long vendorSAP = 0;
    char *tmp = ptr;

    if (ptr != NULL) {
        char *pEnd;
        vendorSAP = strtol(ptr, &pEnd, 10);
        ptr = strtok(NULL, " ");
        if (ptr != NULL)
            return false;
    } else {
        return false;
    }

    int index = 0, osIndex = -1;
    if ((index = getVendorIndex(vendorSAP, vendorFile)) == -1) {
        setbuf(stdout, 0);
        printf("ID not exists. ");
        return false;
    } else {

        char userID[21];
        unsigned long id = 0;

        setbuf(stdout, 0);
        printf("\nEnter Os ID:");
        scanf("%s", userID);
        id = strtol(userID, NULL, 10);

        if (getOsBaseband(id, osFile) != -1) {
            setbuf(stdout, 0);
            printf("ID exists. Try to enter another one");
            return insertOs(tmp, vendorFile, osFile);
        } else {
            struct Os *os = readOs();
            os->vendorSAP = vendorSAP;
            os->baseband = id;

            fseek(*osFile, 0, SEEK_END);
            long end = ftell(*osFile) / (sizeof(struct Os) + sizeof(int));
            os->nextIndex = getOsIndex(index, vendorFile);
            setOsIndex(index, end, vendorFile);
            writeOs(os, osFile);
            free(os);
            return true;
        }
    }
}

bool updateVendor(char *ptr, FILE **vendorFile) {
    int index = getVendor(ptr, vendorFile);
    if (index == -1)
        return false;
    unsigned long SAP = 0;
    fseek(*vendorFile, (sizeof(struct Vendor) + sizeof(int)) * index, SEEK_SET);
    fread(&SAP, sizeof(unsigned long), 1, *vendorFile);
    struct Vendor *vendor = readVendor();
    vendor->SAP = SAP;
    fseek(*vendorFile, (sizeof(struct Vendor) + sizeof(int)) * index, SEEK_SET);
    fwrite(vendor, sizeof(struct Vendor), 1, *vendorFile);
    free(vendor);
    return true;
}

bool updateOs(char *ptr, FILE **vendorFile, FILE **osFile) {
    int index = getOs(ptr, vendorFile, osFile);
    if (index == -1)
        return false;
    unsigned long vendorSAP = 0, osBaseband = 0;
    int nextos = -1;
    fseek(*osFile, (sizeof(struct Os) + sizeof(int)) * index, SEEK_SET);
    fread(&osBaseband, sizeof(unsigned long), 1, *osFile);
    fread(&vendorSAP, sizeof(unsigned long), 1, *osFile);
    fseek(*osFile, (sizeof(struct Os) + sizeof(int)) * (index + 1) - 2 * sizeof(int), SEEK_SET);
    fread(&nextos, sizeof(int), 1, *osFile);
    struct Os *os = readOs();
    os->baseband = osBaseband;
    os->vendorSAP = vendorSAP;
    os->nextIndex = nextos;
    fseek(*osFile, (sizeof(struct Os) + sizeof(int)) * index, SEEK_SET);
    fwrite(os, sizeof(struct Os), 1, *osFile);
    free(os);
    return true;
}