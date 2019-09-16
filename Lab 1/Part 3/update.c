#include "update.h"

bool updateVendor(char *pointer, FILE **vendorFile) {
    int index = getVendor(pointer, vendorFile);
    if (index == -1)
        return false;
    unsigned long SAP = 0;
    fseek(*vendorFile, sizeof(struct Vendor) * index, SEEK_SET);
    fread(&SAP, sizeof(unsigned long), 1, *vendorFile);
    struct Vendor *vendor = readVendor();
    vendor->SAP = SAP;
    fseek(*vendorFile, sizeof(struct Vendor) * index, SEEK_SET);
    fwrite(vendor, sizeof(struct Vendor), 1, *vendorFile);
    free(vendor);
    return true;
}

bool updateOs(char *pointer, FILE **vendorFile, FILE **osFile) {
    int index = getOs(pointer, vendorFile, osFile);
    if (index == -1)
        return false;
    unsigned long contributorID = 0, imageID = 0;
    int nextImage = -1;
    fseek(*osFile, sizeof(struct Os) * index, SEEK_SET);
    fread(&imageID, sizeof(unsigned long), 1, *osFile);
    fread(&contributorID, sizeof(unsigned long), 1, *osFile);
    fseek(*osFile, sizeof(struct Os) * (index + 1) - sizeof(int), SEEK_SET);
    fread(&nextImage, sizeof(int), 1, *osFile);
    struct Os *image = readOs();
    image->basebandVersion = imageID;
    image->SAP = contributorID;
    image->nextIndex = nextImage;
    fseek(*osFile, sizeof(struct Os) * index, SEEK_SET);
    fwrite(image, sizeof(struct Os), 1, *osFile);
    free(image);
    return true;
}