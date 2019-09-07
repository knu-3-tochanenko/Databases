#ifndef PART_3_APPLICATION_H
#define PART_3_APPLICATION_H

#include "database.h"

struct application {
    struct db DB;
    void(*run)(void);
    void(*add)(void);
    void(*remove)(void);
    void(*change)(void);
    void(*count)(void);
};

extern const struct Application;

#endif //PART_3_APPLICATION_H
