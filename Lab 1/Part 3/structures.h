//
// Created by Taya on 9/8/2019.
//

#ifndef UNTITLED_STRUCTURES_H
#define UNTITLED_STRUCTURES_H

struct Date {
    unsigned int year;
    unsigned int month;
    unsigned int day;
    unsigned int hour;
    unsigned int minute;
};


const unsigned long DATE_SIZE;


struct Contributor {
    unsigned long userID;
    char name[25];
    char eMail[25];
    char password[10];
    char address[25];
    int firstImage;
};


const unsigned long CONTRIBUTOR_SIZE;

struct Image {
    unsigned long imageID;
    unsigned long contributorID;
    char imageType[15];
    float width;
    float height;
    float earnings;
    char status[25];
    struct Date* date;
    int nextIndex;
};


const unsigned long IMAGE_SIZE;

struct Cell {
    unsigned long id;
    unsigned int index;
};

#endif //UNTITLED_STRUCTURES_H
