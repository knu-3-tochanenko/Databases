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


struct Vendor {
    unsigned long SAP;
    char name[33];
    char countryCode[4];
    int firstImage;
};


const unsigned long VENDOR_SIZE;

struct Os {
    unsigned long basebandVersion;
    unsigned long SAP;
    char name[33];
    struct Date* buildDate;
    int nextIndex;
};


const unsigned long OS_SIZE;

struct Cell {
    unsigned long id;
    unsigned int index;
};

#endif //UNTITLED_STRUCTURES_H
