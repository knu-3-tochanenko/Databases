#include "database.h"
#include <stdio.h>

void run() {
    printf("It works!");
}

const struct db DB = {
        .run = run
};