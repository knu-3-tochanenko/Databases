#ifndef PART_3_DATABASE_H
#define PART_3_DATABASE_H

#include "tables.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "stdlib.h"

unsigned int countVendor(char *ptr, FILE **vendorFile);
unsigned int countOs(char *ptr, FILE **osFile);
unsigned int countAllOs(char *ptr, FILE **osFile);

bool removeVendor(char *ptr, FILE **vendorFile, FILE **osFile);
bool removeOs(char *ptr, FILE **vendorFile, FILE **osFile);

int getVendor(char *ptr, FILE **vendorFile);
int getOs(char *ptr, FILE **vendorFile, FILE **osFile);

int os_NUMBER;
bool insertVendor(char *ptr, FILE **vendorFile);
bool insertOs(char *ptr, FILE **vendorFile, FILE **osFile);

bool updateVendor(char *ptr, FILE **vendorFile);
bool updateOs(char *ptr, FILE **vendorFile, FILE** osFile);
#endif //PART_3_DATABASE_H
