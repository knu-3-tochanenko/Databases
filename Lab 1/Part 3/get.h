#include "structures.h"
#include "io.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

int get_m(char *ptr, FILE **masterFile);

int get_s(char *ptr, FILE **masterFile, FILE **slaveFile);