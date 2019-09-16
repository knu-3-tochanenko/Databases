#include "io.h"
#include "count.h"
#include "indexTable.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h> 

int IMAGE_NUMBER;

bool insert_m(char *ptr, FILE **masterFile);

bool insert_s(char *ptr, FILE **masterFile, FILE **slaveFile);