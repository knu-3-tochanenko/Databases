//
// Created by Vlad on 07.09.2019.
//

#include "database.h"
#include <stdio.h>

void run() {
    printf("It works!");
}

const struct db DB = {
        .run = run
};