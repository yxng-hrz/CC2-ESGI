#include "malware.h"

#define PORT 9001

int main()
{
    int sockfd, newsockfd;
    struct sockaddr_in addr;
    char buffer[256];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Erreur socket");
        exit(1);
    }
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Erreur bind");
        exit(1);
    }
    
    listen(sockfd, 5);
    printf("C2 server en écoute sur le port %d...\n", PORT);
    while (1) {
        newsockfd = accept(sockfd, NULL, NULL);
        if (newsockfd < 0) {
            perror("Erreur accept");
            continue;
        }
        int n = read(newsockfd, buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            printf("Message reçu: %s", buffer);
        }
        close(newsockfd);
    }
    
    close(sockfd);
    return 0;
}
