#include "indexTable.h"

unsigned TABLE_SIZE = 0;
bool sorted = false;

void initializeTable() {
    for (int i = 0; i < 100; i++) {
        indexTable[i] = malloc(sizeof(struct Cell));
    }
}

void readTable(FILE **indexFile) {
    unsigned long id = 0;
    unsigned int index = 0, i = 0;
    while (fread(&id, sizeof(unsigned long), 1, *indexFile)) {
        fread(&index, sizeof(unsigned int), 1, *indexFile);
        indexTable[i]->SAP = id;
        indexTable[i]->index = index;
        i++;
    }
    TABLE_SIZE = i;
}

void writeTable(FILE **indexFile, const char indexFileName[25]) {
    sortTable();
    FILE *newIndex = fopen("vendor.ind", "w+");
    fseek(newIndex, 0, SEEK_SET);
    for (unsigned int i = 0; i < TABLE_SIZE; i++) {
        fwrite(indexTable[i], sizeof(struct Cell), 1, newIndex);
    }
    fclose(*indexFile);
    remove(indexFileName);
    fclose(newIndex);
    rename("vendor.ind", indexFileName);
}

void deleteTable() {
    TABLE_SIZE = 0;
}

int searchTable(unsigned long SAP) {
    if (TABLE_SIZE == 0)
        return -1;
    sortTable();
    unsigned int first = 0, last = TABLE_SIZE - 1, mid;
    bool flag = false;
    while (first <= last) {
        mid = (first + last) / 2;
        if (SAP == indexTable[mid]->SAP) {
            flag = true;
            break;
        } else {
            if (SAP > indexTable[mid]->SAP)
                first = mid + 1;
            else
                last = mid - 1;
        }
    }
    if (flag) {
        while (indexTable[mid]->SAP == indexTable[mid + 1]->SAP &&
               indexTable[mid]->index < SAP < indexTable[mid + 1]->index) {
            mid++;
            printf("%i", indexTable[mid]->index);
        }
        return indexTable[mid]->index;
    }
    return -1;
}

void sortTable() {
    if (!sorted) {
        qsort(indexTable, TABLE_SIZE, sizeof(*indexTable), compare);
    }
}

void addIndex(unsigned long SAP, unsigned int index) {
    if (TABLE_SIZE >= 100)
        return;
    indexTable[TABLE_SIZE]->SAP = SAP;
    indexTable[TABLE_SIZE]->index = index;
    TABLE_SIZE++;
    sorted = false;
}

void removeIndex(unsigned long SAP) {
    if (TABLE_SIZE == 0)
        return;
    int index = -1;
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (indexTable[i]->SAP == SAP) {
            index = i;
        }
    }
    if (index != -1) {
        for (int i = index; i < TABLE_SIZE - 1; i++) {
            indexTable[i] = indexTable[i + 1];
        }
        TABLE_SIZE--;
    }
}

int compare(const void *a, const void *b) {
    struct Cell f = *((struct Cell *) a);
    if (b == NULL)
        return 1;
    struct Cell s = *((struct Cell *) b);
    if (f.SAP > s.SAP)
        return 1;
    if (f.SAP < s.SAP) return -1;
    if (f.SAP == s.SAP) {
        if (f.index > s.index)
            return 1;
        return -1;
    }
    return 0;
}