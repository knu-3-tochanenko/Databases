#include "io.h"
#include "structures.h"
#include "get.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool updateVendor(char *pointer, FILE **vendorFile);

bool updateOs(char *pointer, FILE **vendorFile, FILE** osFile);