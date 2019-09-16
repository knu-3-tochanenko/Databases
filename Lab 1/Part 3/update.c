#include "update.h"

bool update_m(char *ptr, FILE **masterFile) {
    int index = get_m(ptr, masterFile);
    if (index == -1)
        return false;
    unsigned long id = 0;
    fseek(*masterFile, (sizeof(struct Contributor) + sizeof(int)) * index, SEEK_SET);
    fread(&id, sizeof(unsigned long), 1, *masterFile);
    struct Contributor *contributor = readContributor();
    contributor->userID = id;
    fseek(*masterFile, (sizeof(struct Contributor) + sizeof(int)) * index, SEEK_SET);
    fwrite(contributor, sizeof(struct Contributor), 1, *masterFile);
    free(contributor);
    return true;
}

bool update_s(char *ptr, FILE **masterFile, FILE **slaveFile) {
    int index = get_s(ptr, masterFile, slaveFile);
    if (index == -1)
        return false;
    unsigned long contributorID = 0, imageID = 0;
    int nextImage = -1;
    fseek(*slaveFile, (sizeof(struct Image) + sizeof(int)) * index, SEEK_SET);
    fread(&imageID, sizeof(unsigned long), 1, *slaveFile);
    fread(&contributorID, sizeof(unsigned long), 1, *slaveFile);
    fseek(*slaveFile, (sizeof(struct Image) + sizeof(int)) * (index + 1) - 2 * sizeof(int), SEEK_SET);
    fread(&nextImage, sizeof(int), 1, *slaveFile);
    struct Image *image = readImage();
    image->imageID = imageID;
    image->contributorID = contributorID;
    image->nextIndex = nextImage;
    fseek(*slaveFile, (sizeof(struct Image) + sizeof(int)) * index, SEEK_SET);
    fwrite(image, sizeof(struct Image), 1, *slaveFile);
    free(image);
    return true;
}