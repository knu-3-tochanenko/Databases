#ifndef PART_3_APPLICATION_H
#define PART_3_APPLICATION_H

#include "database.h"

struct app {
    struct db DB;
    void(*run)(void);
    void(*add)(void);
    void(*delete)(void);
    void(*change)(void);
    void(*count)(void);
};

extern const struct app App;

#endif //PART_3_APPLICATION_H
