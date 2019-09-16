#include "insert.h"

int IMAGE_NUMBER = -1;

bool insert_m(char *ptr, FILE **masterFile) {
    if (ptr != NULL) {
        setbuf(stdout, 0);
        printf("Wrong command.");
        return false;
    }

    char userID[21];
    unsigned long id = 0;
    setbuf(stdout, 0);
    printf("\nEnter Contributor User ID:");
    scanf("%s", userID);
    id = strtol(userID, NULL, 10);

    if (getContributorIndex(id, masterFile) != -1) {
        setbuf(stdout, 0);
        printf("ID exists. Try to enter another one");
        return insert_m(ptr, masterFile);
    } else {
        struct Contributor *contributor = readContributor();
        contributor->userID = id;

        fseek(*masterFile, 0, SEEK_END);
        long index = ftell(*masterFile) / (sizeof(struct Contributor) + sizeof(int));
        writeContributor(contributor, masterFile);
        add(id, index);
        free(contributor);
        return true;
    }

}


bool insert_s(char *ptr, FILE **masterFile, FILE **slaveFile) {
    unsigned long contributorID = 0;
    char *tmp = ptr;

    if (ptr != NULL) {
        char *pEnd;
        contributorID = strtol(ptr, &pEnd, 10);
        ptr = strtok(NULL, " ");
        setbuf(stdout, 0);
        printf("%ld", contributorID);
        if (ptr != NULL)
            return false;
    } else {
        return false;
    }

    int index = 0, imageIndex = -1;
    if ((index = getContributorIndex(contributorID, masterFile)) == -1) {
        setbuf(stdout, 0);
        printf("ID not exists. ");
        return false;
    } else {

        char userID[21];
        unsigned long id = 0;

        setbuf(stdout, 0);
        printf("\nEnter Image ID:");
        scanf("%s", userID);
        id = strtol(userID, NULL, 10);

        if (getImageID(id, slaveFile) != -1) {
            setbuf(stdout, 0);
            printf("ID exists. Try to enter another one");
            return insert_s(tmp, masterFile, slaveFile);
        } else {
            struct Image *image = readImage();
            image->contributorID = contributorID;
            image->imageID = id;

            fseek(*slaveFile, 0, SEEK_END);
            long end = ftell(*slaveFile) / (sizeof(struct Image) + sizeof(int));
            image->nextIndex = getImageIndex(index, masterFile);
            setImageIndex(index, end, masterFile);
            writeImage(image, slaveFile);
            free(image);
            return true;
        }
    }
}