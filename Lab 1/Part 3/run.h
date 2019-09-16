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
// <3
bool function(const char masterFName[25], const char indexTableFName[25], const char slaveFName[25]);

bool openFile(const char fName[25], FILE **ptr);

bool listen(FILE **masterFile, FILE **indexFile, FILE **slaveFile);

void rewrite(const char masterFName[25], const char indexTableFName[25], const char slaveFName[25],
             FILE **masterFile, FILE **indexFile, FILE **slaveFile);

int comp(const void *elem1, const void *elem2);

#endif //UNTITLED_RUN_H
