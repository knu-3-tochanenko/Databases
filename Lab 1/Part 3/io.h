#include "structures.h"
#include "indexTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct Contributor *readContributor();

struct Date *readDate();

struct Image *readImage();

unsigned int readTimeUnit(const char *text, const int left, const int right);

void writeContributor(const struct Contributor *contributor, FILE **masterFile);

void writeImage(const struct Image *image, FILE **slaveFile);

void writeDate(const struct Date *date, FILE **slaveFile);

void printContributor(const struct Contributor *contributor);

void printImage(const struct Image *image);

void printDate(const struct Date *date);

int getContributorIndex(const unsigned long id, FILE **masterFile);

int getImageID(const unsigned long id, FILE **slaveFile);

int getImageIndex(const int index, FILE **masterFile);

void setImageIndex(const unsigned long contributorIndex, const unsigned long imageIndex, FILE **masterFile);