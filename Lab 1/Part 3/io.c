#include "io.h"

struct Contributor *readContributor() {
    struct Contributor *contributor = malloc(sizeof(struct Contributor));
    char name[25];
    setbuf(stdout, 0);
    printf("\nEnter Contributor name:");
    scanf("%s", name);
    strcpy(contributor->name, name);
    setbuf(stdout, 0);
    printf("\nEnter Contributor e-mail:");
    scanf("%s", contributor->eMail);
    setbuf(stdout, 0);
    printf("\nEnter Contributor password:");
    scanf("%s", contributor->password);
    setbuf(stdout, 0);
    printf("\nEnter Contributor address:");
    scanf("%s", contributor->address);
    return contributor;
}

struct Image *readImage() {
    struct Image *image = malloc(sizeof(struct Image));
    char width[25], height[25], earnings[25];
    setbuf(stdout, 0);
    printf("\nEnter Image type:");
    scanf("%s", image->imageType);
    setbuf(stdout, 0);
    printf("\nEnter Image width:");
    scanf("%s", width);
    image->width = strtof(width, NULL);
    setbuf(stdout, 0);
    printf("\nEnter Image height:");
    scanf("%s", height);
    image->height = strtof(height, NULL);
    setbuf(stdout, 0);
    printf("\nEnter earning for Image:");
    scanf("%s", earnings);
    image->earnings = strtof(earnings, NULL);
    setbuf(stdout, 0);
    printf("\nEnter Image status:");
    scanf("%s", image->status);
    image->date = readDate();
    return image;
}

struct Date *readDate() {
    struct Date *date = malloc(sizeof(struct Date));
    date->year = readTimeUnit("Enter Year: ", 1920, 2019);
    date->month = readTimeUnit("Enter Month: ", 1, 12);
    unsigned int days = 31;
    if (date->month == 2) {
        if (date->year % 4 == 0) {
            days = 29;
        } else {
            days = 28;
        }
    } else if (date->month == 4 || date->month == 6 || date->month == 9 || date->month == 11) {
        days = 30;
    } else {
        days = 31;
    }
    date->day = readTimeUnit("Enter Day: ", 1, days);
    date->hour = readTimeUnit("Enter Hour: ", 0, 24);
    if (date->hour == 24) {
        date->minute = readTimeUnit("Enter Minute: ", 0, 0);
    } else {
        date->minute = readTimeUnit("Enter Minute: ", 0, 60);
    }
    return date;
}

unsigned int readTimeUnit(const char *text, const int left, const int right) {
    char unitStr[5];
    unsigned int unit = 0;
    bool valid = false;
    while (!valid) {
        setbuf(stdout, 0);
        printf("\n%s", text);
        scanf("%s", unitStr);
        unit = strtol(unitStr, NULL, 10);
        if (unit >= left && unit <= right)
            valid = true;
    }
    return unit;
}

void writeContributor(const struct Contributor *contributor, FILE **masterFile) {
    int status = 1;
    fwrite(contributor, sizeof(struct Contributor), 1, *masterFile);
    fwrite(&status, sizeof(unsigned int), 1, *masterFile);
}

void writeImage(const struct Image *image, FILE **slaveFile) {
    int status = 1;
    fwrite(image, sizeof(struct Image), 1, *slaveFile);
    fwrite(&status, sizeof(unsigned int), 1, *slaveFile);
}

void writeDate(const struct Date *date, FILE **slaveFile) {
    fwrite(&date->year, sizeof(unsigned int), 1, *slaveFile);
    fwrite(&date->month, sizeof(unsigned int), 1, *slaveFile);
    fwrite(&date->day, sizeof(unsigned int), 1, *slaveFile);
    fwrite(&date->hour, sizeof(unsigned int), 1, *slaveFile);
    fwrite(&date->minute, sizeof(unsigned int), 1, *slaveFile);
}

void printContributor(const struct Contributor *contributor) {
    setbuf(stdout, 0);
    printf("\nContributor ID: %ld", contributor->userID);
    setbuf(stdout, 0);
    printf("\nContributor name: %s", contributor->name);
    setbuf(stdout, 0);
    printf("\nContributor e-mail: %s", contributor->eMail);
    setbuf(stdout, 0);
    printf("\nContributor password: %s", contributor->password);
    setbuf(stdout, 0);
    printf("\nContributor address: %s", contributor->address);
}

void printImage(const struct Image *image) {
    setbuf(stdout, 0);
    printf("\nImage type: %s", image->imageType);
    setbuf(stdout, 0);
    printf("\nImage width: %f", image->width);
    setbuf(stdout, 0);
    printf("\nImage height: %f", image->height);
    setbuf(stdout, 0);
    printf("\nEarnings for Image: %f", image->earnings);
    setbuf(stdout, 0);
    printf("\nImage status: %s", image->status);
    printDate(image->date);
}

void printDate(const struct Date *date) {
    printf("Date: %i:%i  %i/%i/%i", date->hour, date->minute, date->month, date->day, date->year);
}

int getContributorIndex(const unsigned long id, FILE **masterFile) {
    unsigned int status = 0;
    int index = searchTable(id);
    if (index != -1) {
        fseek(*masterFile, (index + 1) * (sizeof(struct Contributor) + sizeof(int)) - sizeof(int), SEEK_SET);
        fread(&status, sizeof(unsigned int), 1, *masterFile);
        printf("\n%i   ", status);
        if (status == 1)
            return index;
    }
    return -1;
}

int getImageID(const unsigned long id, FILE **slaveFile) {
    int i = 0;
    struct Image *image = malloc(sizeof(struct Image));
    fseek(*slaveFile, 0, SEEK_SET);
    while (fread(image, sizeof(struct Image), 1, *slaveFile)) {
        if (image->imageID == id)
            return i;
        i++;
    }
    return -1;
}

void setImageIndex(const unsigned long contributorIndex, int imageIndex, FILE **masterFile) {
    fseek(*masterFile, (sizeof(struct Contributor) + sizeof(int)) * (contributorIndex + 1) - 2 * sizeof(int), SEEK_SET);
    fwrite(&imageIndex, sizeof(int), 1, *masterFile);
}

int getImageIndex(const int index, FILE **masterFile) {
    struct Contributor *contributor = malloc(sizeof(struct Contributor));
    fseek(*masterFile, (sizeof(struct Contributor) + sizeof(int)) * index, SEEK_SET);
    fread(contributor, sizeof(struct Contributor), 1, *masterFile);
    int firstImage = contributor->firstImage;
    free(contributor);
    return firstImage;
}