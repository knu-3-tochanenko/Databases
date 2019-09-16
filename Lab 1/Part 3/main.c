#include <stdio.h>
#include "application.h"

int main() {

    const char vendorFName[25] = "Vendor.fl";
    const char vendorIndexTable[25] = "Vendor.ind";
    const char osFName[25] = "Os.fl";


    bool success = runnable(vendorFName, vendorIndexTable, osFName);
    if (success) {
        setbuf(stdout, 0);
        printf("Success!\n");
    } else {
        setbuf(stdout, 0);
        printf("Error handled!\n");
    }
    return 0;
}