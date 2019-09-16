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
    char name[25];
    char countryCode[10];
    int cennoctedTo;
};


const unsigned long vendor_SIZE;

struct Os {
    unsigned long baseband;
    unsigned long vendorSAP;
    char androidVersion[15];
    char name[25];
    struct Date* date;
    int nextIndex;
};


const unsigned long os_SIZE;

struct Cell {
    unsigned long SAP;
    unsigned int index;
};

#endif //UNTITLED_STRUCTURES_H
