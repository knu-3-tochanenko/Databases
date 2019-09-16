//
// Created by Taya on 9/7/2019.
//

#ifndef UNTITLED_RUN_H
#define UNTITLED_RUN_H

#include "io.h"
#include "indexTable.h"
#include "insert.h"
#include "get.h"
#include "delete.h"
#include "update.h"
#include "count.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
bool function(const char vendorFileName[25], const char indexFileName[25], const char osFileName[25]);

bool openFile(const char fileName[25], FILE **file);

bool listen(FILE **vendorFile, FILE **indexFile, FILE **osFile);

void rewrite(const char vendorFileName[25], const char indexFileName[25], const char osFileName[25],
             FILE **vendorFile, FILE **indexFile, FILE **osFile);

int comp(const void *a, const void *b);

#endif //UNTITLED_RUN_H
