#include "application.h"
#include <stdio.h>
#include <string.h>

void add(char * tag) {
    if (strcmp(tag, "vendor") ==  0) {
        setbuf(stdout, 0);printf("SAP[5] Name[up to 32] Country_Code[3]\n");
        char word[256];
        fgets(word, sizeof(word), stdin);
        char * words = strtok(word, " \n");
        char * sap = words;
        setbuf(stdout, 0);printf("\nACHTUNG! SAP: %s\n", sap);
        words = strtok(NULL, " \n");
        char * name = words;
        words = strtok(NULL, " \n");
        char * country_code = words;

        struct Vendor vendor;
        strcpy(vendor.name, name);
        strcpy(vendor.country_code, country_code);
        strcpy(vendor.SAP, sap);
        setbuf(stdout, 0);printf("\nVENDOR->SAP: %s", vendor.SAP);
        setbuf(stdout, 0);printf("\nSAP: %s", sap);
        DB.add_vendor(&vendor);
    }
    else {
        setbuf(stdout, 0);printf("In Add function : else");
    }
}

void change(char * tag) {

}

void count(char * tag) {

}

void delete(char * tag) {

}

void get(char * tag) {
    if (strcmp(tag, "vendor") ==  0) {
        setbuf(stdout, 0);printf("SAP[5]\n");
        char word[256];
        fgets(word, sizeof(word), stdin);
        char * words = strtok(word, " \n");
        char * sap = words;
        struct Vendor* vendor = DB.get_vendor(sap);
        setbuf(stdout, 0);printf("Name: %s\nCountry Code: %s", vendor->name, vendor->country_code);
    }
    else {
        setbuf(stdout, 0);printf("In get function : else");
    }
}

void run() {
    DB.init_files("vendor.fl", "os.fl", "index.ind");
    number_of_entries = 0;
    char word[256];
    char *commands;
    while (true) {
        fgets(word, sizeof(word), stdin);
        commands = strtok(word, " \n");

        char * command = commands;
        commands = strtok(NULL, " \n");
        char * tag = commands;

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
            setbuf(stdout, 0);printf("\nError! Unknown tag!\n");
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