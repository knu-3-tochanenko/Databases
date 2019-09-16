#include "delete.h"

bool del_m(char *ptr, FILE **masterFile, FILE **slaveFile) {
    char *tmp = ptr;
    long contributorID = 0;
    if (ptr != NULL) {
        char *pEnd;
        contributorID = strtol(ptr, &pEnd, 10);
    }

    int index = get_m(tmp, masterFile);
    if (index == -1)
        return false;
    unsigned int status = 0;
    int lastImage = getImageIndex(index, masterFile);

    char cStr[200];
    ltoa(contributorID, cStr, 10);

    while (lastImage != -1) {
        char tmp[50];
        itoa(lastImage, tmp, 10);
        strcat(cStr, " ");
        strcat(cStr, tmp);
        char* newPtr = strtok(cStr, " ");
        del_s(newPtr, masterFile, slaveFile);
        fseek(*slaveFile, (sizeof(struct Image) + sizeof(int)) * (lastImage + 1) - 2 * sizeof(int), SEEK_SET);
        fread(&lastImage, sizeof(int), 1, *slaveFile);
    }

    del(contributorID);
    fseek(*masterFile, (sizeof(struct Contributor) + sizeof(int)) * (index + 1) - sizeof(int), SEEK_SET);
    fwrite(&status, sizeof(unsigned int), 1, *masterFile);
    return true;
}

bool del_s(char *ptr, FILE **masterFile, FILE **slaveFile) {
    char *tmp = ptr;
    long contributorID = 0;
    if (ptr != NULL) {
        char *pEnd;
        contributorID = strtol(ptr, &pEnd, 10);
    }

    int imageIndex = get_s(tmp, masterFile, slaveFile);
    if (imageIndex == -1)
        return false;

    int contributorIndex = getContributorIndex(contributorID, masterFile);
    int lastImage = getImageIndex(contributorIndex, masterFile);

    if (imageIndex == -1)
        return false;

    unsigned int status = 0;
    fseek(*slaveFile, (sizeof(int) + sizeof(struct Image) * (imageIndex + 1)) - sizeof(int), SEEK_SET);
    fwrite(&status, sizeof(unsigned int), 1, *slaveFile);

    if (lastImage == imageIndex) {
        int nextIndex = -1;
        fseek(*masterFile, (sizeof(struct Contributor) + sizeof(int)) * (contributorIndex + 1) - 2 * sizeof(int),
              SEEK_SET);
        fseek(*slaveFile, (sizeof(struct Image) + sizeof(int)) * (imageIndex + 1) - 2 * sizeof(int), SEEK_SET);
        fread(&nextIndex, sizeof(int), 1, *slaveFile);
        fwrite(&nextIndex, sizeof(int), 1, *masterFile);
    } else {
        int prevIndex = lastImage, nextIndex = lastImage;
        while (prevIndex != -1) {
            prevIndex = nextIndex;
            fseek(*slaveFile, (sizeof(struct Image) + sizeof(int)) * (nextIndex + 1) - 2 * sizeof(int), SEEK_SET);
            fread(&nextIndex, sizeof(int), 1, *slaveFile);
            if (nextIndex == imageIndex) {
                fseek(*slaveFile, (sizeof(int) + sizeof(struct Image)) * (nextIndex + 1) - 2 * sizeof(int), SEEK_SET);
                fread(&nextIndex, sizeof(int), 1, *slaveFile);
                fseek(*slaveFile, (sizeof(int) + sizeof(struct Image)) * (prevIndex + 1) - 2 * sizeof(int), SEEK_SET);
                fwrite(&nextIndex, sizeof(int), 1, *slaveFile);
            }
        }
    }
    return true;
}