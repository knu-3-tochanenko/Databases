#include "io.h"
#include "structures.h"
#include "get.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool update_m(char *ptr, FILE **masterFile);

bool update_s(char *ptr, FILE **masterFile, FILE** slaveFile);