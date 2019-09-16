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
    while (fread(&id, sizeof(unsigned long), 1, *indexFile) == 1) {
        fread(&index, sizeof(unsigned int), 1, *indexFile);
        indexTable[i]->id = id;
        indexTable[i]->index = index;
        i++;
    }
    TABLE_SIZE = i;
}

void writeTable(FILE **indexFile, const char indexTableFName[25]) {
    sortTable();
    FILE *newIndex = fopen("master.ind", "w+");
    fseek(newIndex, 0, SEEK_SET);
    for (unsigned int i = 0; i < TABLE_SIZE; i++) {
        fwrite(indexTable[i], sizeof(struct Cell), 1, newIndex);
    }
    fclose(*indexFile);
    remove(indexTableFName);
    fclose(newIndex);
    rename("master.ind", indexTableFName);
}

void deleteTable() {
    TABLE_SIZE = 0;
}

int searchTable(unsigned long id) {
    if (TABLE_SIZE == 0)
        return -1;
    sortTable();
    unsigned int first = 0, last = TABLE_SIZE, mid;
    bool flag = false;
    while (first <= last) {
        mid = (first + last) / 2;
        if (id == indexTable[mid]->id) {
            flag = true;
            break;
        } else {
            if (id > indexTable[mid]->id)
                first = mid + 1;
            else
                last = mid - 1;
        }
    }
    if (flag)
        return indexTable[mid]->index;
    return -1;
}

void sortTable() {
    if (!sorted) {
        qsort(indexTable, TABLE_SIZE, sizeof(*indexTable), comp);
    }
}

void add(unsigned long id, unsigned int index) {
    if (TABLE_SIZE >= 100)
        return;
    indexTable[TABLE_SIZE]->id = id;
    indexTable[TABLE_SIZE]->index = index;
    TABLE_SIZE++;
    sorted = false;
}

int comp(const void *elem1, const void *elem2) {
    struct Cell f = *((struct Cell *) elem1);
    if (elem2 == NULL)
        return 1;
    struct Cell s = *((struct Cell *) elem2);
    if (f.id > s.id) return 1;
    if (f.id < s.id) return -1;
    return 0;
}