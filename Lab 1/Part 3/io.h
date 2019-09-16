#include "structures.h"
#include "indexTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct Vendor *readVendor();

struct Date *readDate();

struct Os *readOs();

unsigned int readTimeUnit(const char *text, const int left, const int right);

void writeVendorToFile(const struct Vendor *vendor, FILE **vendorFile);

void writeOsToFile(const struct Os *os, FILE **osFile);

void writeDate(const struct Date *date, FILE **osFile);

void writeVendor(const struct Vendor *vendor);

void writeOs(const struct Os *os);

void printDate(const struct Date *date);

int getVendorSAP(const unsigned long id, FILE **vendorFile);

int getOsVersion(const unsigned long id, FILE **osFile);

int getOsIndex(const int index, FILE **vendorFile);

void setImageIndex(const unsigned long vendorSAP, const unsigned long osBasebandVersion, FILE **vendorFile);