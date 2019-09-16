#include "get.h"

int get_m(char *ptr, FILE **masterFile) {
    long id;

    if (ptr != NULL) {
        char *pEnd;
        id = strtol(ptr, &pEnd, 10);
        ptr = strtok(NULL, " ");
        setbuf(stdout, 0);
        printf("%ld", id);
        if (ptr != NULL)
            return -1;
    } else {
        return -1;
    }

    int index = 0;
    if ((index = getContributorIndex(id, masterFile)) == -1) {
        setbuf(stdout, 0);
        printf("ID does not exist. Try to enter another one");
        return -1;
    } else {
        struct Contributor *contributor = malloc(sizeof(struct Contributor));
        fseek(*masterFile, sizeof(struct Contributor) * index, SEEK_SET);
        fread(contributor, sizeof(struct Contributor), 1, *masterFile);
        printContributor(contributor);
        free(contributor);
        return index;
    }
}

int get_s(char *ptr, FILE **masterFile, FILE **slaveFile) {
    long contributorID = 0, imageID = 0;

    if (ptr != NULL) {
        char *pEnd;
        contributorID = strtol(ptr, &pEnd, 10);
        ptr = strtok(NULL, " ");
        if (ptr == NULL)
            return -1;
        else {
            imageID = strtol(ptr, &pEnd, 10);
            ptr = strtok(NULL, " ");
            if (ptr != NULL)
                return -1;
        }
    } else {
        return -1;
    }

    int index = 0;
    if ((index = getContributorIndex(contributorID, masterFile)) == -1) {
        setbuf(stdout, 0);
        printf("ID does not exist.");
        return -1;
    } else {
        int imageIndex = getImageIndex(index, masterFile);
        printf("%i", imageIndex);
        if (imageIndex == -1)
            return -1;
        struct Image *image = malloc(sizeof(struct Image));
        unsigned int status = 0;
        while (imageIndex != -1) {
            fseek(*slaveFile, sizeof(struct Image) * imageIndex, SEEK_SET);
            fread(image, sizeof(struct Image), 1, *slaveFile);
            fread(&status, sizeof(unsigned int), 1, *slaveFile);
            if (image->imageID == imageID && status == 1) {
                printImage(image);
                free(image);
                return imageIndex;
            }
            imageIndex = image->nextIndex;
        }
        free(image);
        return -1;
    }
}
