#include "io.h"
#include "count.h"
#include "indexTable.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

int IMAGE_NUMBER;

bool addVendor(char *ptr, FILE **vendorFile);

bool addOs(char *ptr, FILE **vendorFile, FILE **osFile);