#include "inputoutput.h"

struct Vendor *readVendor() {
    struct Vendor *vendor = malloc(sizeof(struct Vendor));
    char name[25];
    setbuf(stdout, 0);
    printf("\nName:");
    scanf("%s", name);
    strcpy(vendor->name, name);
    setbuf(stdout, 0);
    printf("\nCountry code:");
    scanf("%s", vendor->countryCode);
    return vendor;
}

struct Os *readOs() {
    struct Os *os = malloc(sizeof(struct Os));
    char width[25], height[25], earnings[25];
    setbuf(stdout, 0);
    printf("\nAndroid version:");
    scanf("%s", os->androidVersion);
    setbuf(stdout, 0);
    printf("\nName:");
    scanf("%s", os->name);
    os->date = readDate();
    return os;
}

struct Date *readDate() {
    struct Date *date = malloc(sizeof(struct Date));
    date->year = readTimeUnit("Year: ", 1920, 2019);
    date->month = readTimeUnit("Month: ", 1, 12);
    unsigned int days = 31;
    if (date->month == 2) {
        if (date->year % 4 == 0) {
            days = 29;
        } else {
            days = 28;
        }
    } else if (date->month == 4 || date->month == 6 || date->month == 9 || date->month == 11) {
        days = 30;
    } else {
        days = 31;
    }
    date->day = readTimeUnit("Day: ", 1, days);
    date->hour = readTimeUnit("Hour: ", 0, 24);
    if (date->hour == 24) {
        date->minute = readTimeUnit("Minute: ", 0, 0);
    } else {
        date->minute = readTimeUnit("Minute: ", 0, 60);
    }
    return date;
}

unsigned int readTimeUnit(const char *text, const int left, const int right) {
    char unitStr[5];
    unsigned int unit = 0;
    bool valid = false;
    while (!valid) {
        setbuf(stdout, 0);
        printf("\n%s", text);
        scanf("%s", unitStr);
        unit = strtol(unitStr, NULL, 10);
        if (unit >= left && unit <= right)
            valid = true;
    }
    return unit;
}

void writeVendor(const struct Vendor *vendor, FILE **vendorFile) {
    int status = 1;
    fwrite(vendor, sizeof(struct Vendor), 1, *vendorFile);
    fwrite(&status, sizeof(unsigned int), 1, *vendorFile);
}

void writeOs(const struct Os *os, FILE **osFile) {
    int status = 1;
    fwrite(os, sizeof(struct Os), 1, *osFile);
    fwrite(&status, sizeof(unsigned int), 1, *osFile);
}

void writeDate(const struct Date *date, FILE **osFile) {
    fwrite(&date->year, sizeof(unsigned int), 1, *osFile);
    fwrite(&date->month, sizeof(unsigned int), 1, *osFile);
    fwrite(&date->day, sizeof(unsigned int), 1, *osFile);
    fwrite(&date->hour, sizeof(unsigned int), 1, *osFile);
    fwrite(&date->minute, sizeof(unsigned int), 1, *osFile);
}

void printVendor(const struct Vendor *vendor) {
    setbuf(stdout, 0);
    printf("\nSAP: %ld", vendor->SAP);
    setbuf(stdout, 0);
    printf("\nName: %s", vendor->name);
    setbuf(stdout, 0);
    printf("\nCountry code: %s", vendor->countryCode);
}

void printOs(const struct Os *os) {
    setbuf(stdout, 0);
    printf("\nAndroid version: %s", os->androidVersion);
    setbuf(stdout, 0);
    printf("\nName: %s", os->name);
    printDate(os->date);
}

void printDate(const struct Date *date) {
    printf("\nDate: %i:%i  %i/%i/%i", date->hour, date->minute, date->month, date->day, date->year);
}

int getVendorIndex(const unsigned long SAP, FILE **vendorFile) {
    unsigned int status = 0;
    int index = searchTable(SAP);
    if (index != -1) {
        fseek(*vendorFile, (index + 1) * (sizeof(struct Vendor) + sizeof(int)) - sizeof(int), SEEK_SET);
        fread(&status, sizeof(unsigned int), 1, *vendorFile);
        printf("\n%i   ", status);
        if (status == 1)
            return index;
    }
    return -1;
}

int getOsBaseband(const unsigned long baseband, FILE **osFile) {
    int i = 0;
    struct Os *os = malloc(sizeof(struct Os));
    fseek(*osFile, 0, SEEK_SET);
    while (fread(os, sizeof(struct Os), 1, *osFile)) {
        if (os->baseband == baseband)
            return i;
        i++;
    }
    return -1;
}

void setOsIndex(const unsigned long vendorIndex, int osIndex, FILE **vendorFile) {
    fseek(*vendorFile, (sizeof(struct Vendor) + sizeof(int)) * (vendorIndex + 1) - 2 * sizeof(int), SEEK_SET);
    fwrite(&osIndex, sizeof(int), 1, *vendorFile);
}

int getOsIndex(const int index, FILE **vendorFile) {
    struct Vendor *vendor = malloc(sizeof(struct Vendor));
    fseek(*vendorFile, (sizeof(struct Vendor) + sizeof(int)) * index, SEEK_SET);
    fread(vendor, sizeof(struct Vendor), 1, *vendorFile);
    int firstos = vendor->cennoctedTo;
    free(vendor);
    return firstos;
}