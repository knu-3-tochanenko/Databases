#include "tables.h"
#include "indexTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct Vendor *readVendor();
struct Date *readDate();
struct Os *readOs();
unsigned int readTimeUnit(const char *text, const int left, const int right);
void writeVendor(const struct Vendor *vendor, FILE **vendorFile);
void writeOs(const struct Os *os, FILE **osFile);
void writeDate(const struct Date *date, FILE **osFile);
void printVendor(const struct Vendor *vendor);
void printOs(const struct Os *os);
void printDate(const struct Date *date);
int getVendorIndex(const unsigned long SAP, FILE **vendorFile);
int getOsBaseband(const unsigned long baseband, FILE **osFile);
int getOsIndex(const int index, FILE **vendorFile);
void setOsIndex(const unsigned long vendorIndex, int osIndex, FILE **vendorFile);