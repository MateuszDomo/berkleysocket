#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int passiveSockFD = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    int option = 1;
    setsockopt(passiveSockFD, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if (passiveSockFD == -1) {
        printf("%s\n", strerror(errno));
        return 1;
    }

    struct sockaddr_in6 serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin6_family = AF_INET6;
    serverAddress.sin6_port = htons(1234);
    serverAddress.sin6_addr = in6addr_any;

    if (bind(passiveSockFD, (const struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        printf("%s\n", strerror(errno));
        close(passiveSockFD);
        return 1;
    }

    if (listen(passiveSockFD, 1)) {
        printf("%s\n", strerror(errno));
        close(passiveSockFD);
        return 1;
    }

    struct sockaddr clientAddress;
    memset(&clientAddress, 0, sizeof(clientAddress));
    socklen_t clientAddressLen = sizeof(clientAddress);
    for (;;) {
        printf("Waiting for TCP Connection\n");
        int connectedSockFD = accept(passiveSockFD, &clientAddress, &clientAddressLen);
        if (connectedSockFD == -1) {
            printf("%s\n", strerror(errno));
            close(passiveSockFD);
            return 1;
        }

        char buffer[1024];
        ssize_t bytesReceived = recv(connectedSockFD, buffer, 1024, 0);
        if (bytesReceived == -1) {
            printf("Error receiving data: %s\n", strerror(errno));
            close(passiveSockFD);
            close(connectedSockFD);
            return 1;
        }
        printf("%s \n", buffer);


        char *reply = "HTTP/1.0 200 OK\r\n\r\nHello";
        ssize_t sent_bytes = send(connectedSockFD, reply, strlen(reply), 0);
        if (sent_bytes == -1) {
            printf("%s\n", strerror(errno));
            close(passiveSockFD);
            close(connectedSockFD);
            return 1;
        }

        close(connectedSockFD);
    }

    close(passiveSockFD);
    return 0;
}