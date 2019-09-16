#ifndef UNTITLED_RUN_H
#define UNTITLED_RUN_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
bool runnable(const char *vendorFileName, const char *indexFileName, const char *osFileName);

bool openFile(const char fileName[25], FILE **ptr);

bool listen(FILE **vendorFile, FILE **indexFile, FILE **osFile);

void rewrite(
        const char vendorFileName[25],
        const char indexFileName[25],
        const char osFileName[25],
        FILE **vendorFile, FILE **indexFile, FILE **osFile);

int compare(const void *a, const void *b);

#endif //UNTITLED_RUN_H
