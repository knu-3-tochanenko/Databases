#include "tables.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

unsigned TABLE_SIZE;
struct Cell *indexTable[100];
bool sorted;

void initializeTable();

void readTable(FILE **indexFile);
void writeTable(FILE **indexFile, const char indexFileName[25]);
void deleteTable();
int searchTable(unsigned long SAP);
void sortTable();
void addIndex(unsigned long SAP, unsigned int index);
void removeIndex(unsigned long SAP);
int compare(const void *a, const void *b);