#include "application.h"
#include <stdio.h>
#include <string.h>

void add(char *tag) {
    if (strcmp(tag, "vendor") == 0) {
        setbuf(stdout, 0);
        printf("SAP[5] Name[up to 32] Country_Code[3]\n");
        char word[256];
        fgets(word, sizeof(word), stdin);
        char *words = strtok(word, " \n");
        char *sap = words;
        setbuf(stdout, 0);
        printf("\nACHTUNG! SAP: %s\n", sap);
        words = strtok(NULL, " \n");
        char *name = words;
        words = strtok(NULL, " \n");
        char *country_code = words;

        struct Vendor vendor;
        strcpy(vendor.name, name);
        strcpy(vendor.countryCode, country_code);
        strcpy(vendor.SAP, sap);
        setbuf(stdout, 0);
        printf("\nVENDOR->SAP: %s", vendor.SAP);
        setbuf(stdout, 0);
        printf("\nSAP: %s", sap);
        DB.addVendor(vendor);
    } else {
        setbuf(stdout, 0);
        printf("In Add function : else");
    }
}

void change(char *tag) {

}

void count(char *tag) {

}

void delete(char *tag) {

}

void get(char *tag) {
    if (strcmp(tag, "vendor") == 0) {
        setbuf(stdout, 0);
        printf("SAP[5]\n");
        char word[256];
        fgets(word, sizeof(word), stdin);
        char *words = strtok(word, " \n");
        char *sap = words;
        struct Vendor vendor = DB.getVendor(sap);
        if (strncmp(vendor.SAP, "UNDEF\000", 6) != 0) {
            setbuf(stdout, 0);
            printf("Name: %s\nCountry Code: %s\n", vendor.name, vendor.countryCode);
        }
        else {
            setbuf(stdout, 0);
            printf("\nAn error occurred\n");
        }
    } else {
        setbuf(stdout, 0);
        printf("\nIn get function : else\n");
    }
}

void run() {
    DB.initFiles("vendor.fl", "os.fl", "index.ind");
    numberOfEntries = 0;
    char word[256];
    char *commands;
    while (true) {
        setbuf(stdout, 0);
        printf("\nWrite command :\n");
        fgets(word, sizeof(word), stdin);
        commands = strtok(word, " \n");

        char *command = commands;
        commands = strtok(NULL, " \n");
        char *tag = commands;

        //  Add
        if (strcmp(command, "add") == 0)
            add(tag);
            //  Change
        else if (strcmp(command, "change") == 0)
            change(tag);
            //  Count
        else if (strcmp(command, "count") == 0)
            count(tag);
            //  Delete
        else if (strcmp(command, "delete") == 0)
            delete(tag);
            //  Get
        else if (strcmp(command, "get") == 0)
            get(tag);
            //  Unknown tag
        else {
            setbuf(stdout, 0);
            printf("\nError! Unknown tag!\n");
            break;
        }
    }
    DB.end();
}

const struct app App = {
        .add = add,
        .change = change,
        .count = count,
        .delete = delete,
        .run = run,
        .get = get
};