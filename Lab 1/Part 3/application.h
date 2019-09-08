#ifndef PART_3_APPLICATION_H
#define PART_3_APPLICATION_H

#include "database.h"

struct app {
    void(*run)(void);
    void(*add)(char*);
    void(*delete)(char*);
    void(*change)(char*);
    void(*count)(char*);
    void(*get)(char*);
};

extern const struct app App;

#endif //PART_3_APPLICATION_H
