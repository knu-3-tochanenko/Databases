#include "delete.h"

bool del_m(char *ptr, FILE **masterFile) {
    int index = getVendor(ptr, masterFile);
    if (index == -1)
        return false;
    unsigned int status = 0;
    fseek(*masterFile, sizeof(struct Vendor) * (index + 1), SEEK_SET);
    fwrite(&status, sizeof(unsigned int), 1, *masterFile);
    return true;
}

bool del_s(char *ptr, FILE **masterFile, FILE **slaveFile) {
    int index = getOs(ptr, masterFile, slaveFile);
    if (index == -1)
        return false;
    unsigned int status = 0;
    fseek(*slaveFile, sizeof(struct Os) * (index + 1), SEEK_SET);
    fwrite(&status, sizeof(unsigned int), 1, *slaveFile);
    return true;
}