#include "structures.h"
#include "io.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

int getVendor(char *ptr, FILE **vendorFile);

int getOs(char *ptr, FILE **vendorFile, FILE **osFile);