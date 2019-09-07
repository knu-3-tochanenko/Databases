#ifndef PART_3_DATABASE_H
#define PART_3_DATABASE_H

struct db {
    void(*run)();
};

extern const struct db DB;

#endif //PART_3_DATABASE_H
