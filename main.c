#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    int passiveSockFD = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    if (passiveSockFD == -1) {
        printf("%s\n", strerror(errno));
        return 1;
    }

    struct sockaddr_in6 serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin6_family = AF_INET6;
    serverAddress.sin6_port = htons(8080);
    if (inet_pton(AF_INET6, "3030:1234", &serverAddress.sin6_addr) == -1) {
        printf("%s\n", strerror(errno));
        return 1;
    }

    if (bind(passiveSockFD, (const struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        printf("%s\n", strerror(errno));
        return 1;
    }

    if (listen(passiveSockFD, 1)) {
        printf("%s\n", strerror(errno));
        return 1;
    }

    struct sockaddr clientAddress;
    memset(&clientAddress, 0, sizeof(clientAddress));
    socklen_t clientAddressLen = sizeof(clientAddress);

    int sockFD = accept(passiveSockFD, &clientAddress, &clientAddressLen);
    if (sockFD == -1) {
        printf("%s\n", strerror(errno));
        return 1;
    }

    printf("%d\n", htons(serverAddress.sin6_port));

    return 0;
}