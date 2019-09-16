#include "io.h"

struct Vendor *readVendor() {
    struct Vendor *contributor = malloc(sizeof(struct Vendor));
    char name[25];
    setbuf(stdout, 0);
    printf("\nEnter Vendor name:");
    scanf("%s", name);
    strcpy(contributor->name, name);
    setbuf(stdout, 0);
    printf("\nEnter Vendor countryCode:");
    scanf("%s", contributor->countryCode);
    return contributor;
}

struct Os *readOs() {
    struct Os *os = malloc(sizeof(struct Os));
    char width[25], height[25], earnings[25];
    setbuf(stdout, 0);
    printf("\nEnter Os type:");
    scanf("%s", os->name);
    os->buildDate = readDate();
    return os;
}

struct Date *readDate() {
    struct Date *date = malloc(sizeof(struct Date));
    date->year = readTimeUnit("Enter Year: ", 1920, 2019);
    date->month = readTimeUnit("Enter Month: ", 1, 12);
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
    date->day = readTimeUnit("Enter Day: ", 1, days);
    date->hour = readTimeUnit("Enter Hour: ", 0, 24);
    if (date->hour == 24) {
        date->minute = readTimeUnit("Enter Minute: ", 0, 0);
    } else {
        date->minute = readTimeUnit("Enter Minute: ", 0, 60);
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

void writeVendorToFile(const struct Vendor *vendor, FILE **vendorFile) {
    int status = 1;
    fwrite(vendor, sizeof(struct Vendor), 1, *vendorFile);
    fwrite(&status, sizeof(unsigned int), 1, *vendorFile);
    printf("%ld", ftell(*vendorFile));
}

void writeOsToFile(const struct Os *os, FILE **osFile) {
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

void writeVendor(const struct Vendor *vendor) {
    setbuf(stdout, 0);
    printf("\nVendor ID: %ld", vendor->SAP);
    setbuf(stdout, 0);
    printf("\nVendor name: %s", vendor->name);
    setbuf(stdout, 0);
    printf("\nVendor countryCode: %s", vendor->countryCode);
}

void writeOs(const struct Os *os) {
    setbuf(stdout, 0);
    printf("\nOs type: %s", os->name);
    printDate(os->buildDate);
}

void printDate(const struct Date *date) {
    printf("Date: %i:%i  %i/%i/%i", date->hour, date->minute, date->month, date->day, date->year);
}

int getVendorSAP(const unsigned long id, FILE **vendorFile) {
    unsigned int status = 0;
    int index = searchTable(id);
    printf("%i", index);
    if (index != -1) {
        fseek(*vendorFile, (index + 1) * sizeof(struct Vendor), SEEK_SET);
        fread(&status, sizeof(unsigned int), 1, *vendorFile);
        if (status == 1)
            return index;
    }
    return -1;
}

int getOsVersion(const unsigned long id, FILE **osFile) {
    int i = 0;
    struct Os *image = malloc(sizeof(struct Os));
    fseek(*osFile, 0, SEEK_SET);
    while (fread(image, sizeof(struct Os), 1, *osFile)) {
        if (image->basebandVersion == id)
            return i;
        i++;
    }
    return -1;
}

void setImageIndex(const unsigned long vendorSAP, const unsigned long osBasebandVersion, FILE **vendorFile) {
    fseek(*vendorFile, sizeof(struct Vendor) * (vendorSAP + 1) - sizeof(int), SEEK_SET);
    fwrite(&osBasebandVersion, sizeof(int), 1, *vendorFile);
}

int getOsIndex(const int index, FILE **vendorFile) {
    struct Vendor *vendor = malloc(sizeof(struct Vendor));
    fseek(*vendorFile, sizeof(struct Vendor) * index, SEEK_SET);
    fread(vendor, sizeof(struct Vendor), 1, *vendorFile);
    return vendor->firstImage;
}