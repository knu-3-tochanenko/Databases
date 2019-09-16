#include "structures.h"

const unsigned long CONTRIBUTOR_SIZE = sizeof(unsigned long) + sizeof(char) * (25 + 25 + 10 + 25) + sizeof(int);
const unsigned long DATE_SIZE = sizeof(unsigned int) * 5;
const unsigned long IMAGE_SIZE =
        sizeof(unsigned long) * 2 + sizeof(char) * (15 + 25) + sizeof(float) * 3 + DATE_SIZE + sizeof(int);
