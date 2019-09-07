#include "application.h"
#include <stdio.h>

void add() {

}

void change() {

}

void count() {

}

void delete() {

}

void run() {
    printf("Does it work?");
    
}

const struct app App = {
        .add = add,
        .change = change,
        .count = count,
        .delete = delete,
        .run = run
};