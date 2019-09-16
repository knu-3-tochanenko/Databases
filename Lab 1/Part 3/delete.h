#include "structures.h"
#include "io.h"
#include "indexTable.h"
#include "get.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


bool del_m(char *ptr, FILE **masterFile,FILE **slaveFile);

bool del_s(char *ptr, FILE **masterFile, FILE **slaveFile);