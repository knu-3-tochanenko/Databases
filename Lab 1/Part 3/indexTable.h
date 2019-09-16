#include "structures.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

unsigned TABLE_SIZE;
struct Cell *indexTable[100];
bool sorted;

void initializeTable();

void readTable(FILE **indexFile);

void writeTable(FILE **indexFile, const char indexTableFName[25]);

void deleteTable();

int searchTable(unsigned long id);

void sortTable();

void add(unsigned long id, unsigned int index);

int comp(const void *a, const void *b);