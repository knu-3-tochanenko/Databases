#include "structures.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "stdlib.h"

unsigned int count_m(char *ptr, FILE **masterFile);

unsigned int count_s(char *ptr, FILE **slaveFile);

unsigned int count_all(char *ptr, FILE **slaveFile);