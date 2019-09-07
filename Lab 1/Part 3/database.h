#ifndef PART_3_DATABASE_H
#define PART_3_DATABASE_H

struct db {
    struct Vendor_Cell(*add_vendor)(struct Vendor vendor);
    struct OS(*add_os)(struct OS os, char);
    struct Vendor(*get_vendor)(char key[5]);    //  Key - SAP code
    struct OS(*get_os)(char key[8]);            //  Key - baseband version
    struct OS*(*get_all_os)(char key[5]);       //  Key - linked vendor's SAP
    void(*remove_vendor)(char key[5]);          //  Key - SAP code
    void(*remove_os)(char key[8]);              //  Key - baseband version
    void(*update_vendor)(char key[5]);          //  Key - SAP code
    void(*update_os)(char key[8]);              //  Key - baseband version
};

#endif //PART_3_DATABASE_H
