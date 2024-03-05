
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sockFD = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    int option = 1;
    setsockopt(sockFD, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if (sockFD == -1) {
        printf("%s\n", strerror(errno));
        return 1;
    }

    struct sockaddr_in6 clientAddress;
    memset(&clientAddress, 0, sizeof(clientAddress));
    clientAddress.sin6_family = AF_INET6;
    clientAddress.sin6_port = htons(1234);
    clientAddress.sin6_addr = in6addr_any;

    if (connect(sockFD, (const struct sockaddr *)&clientAddress, sizeof(clientAddress)) == -1) {
        printf("%s\n", strerror(errno));
        close(sockFD);
        return 1;
    }

    char *message = "HEY";
    ssize_t sent_bytes = send(sockFD, message, strlen(message), 0);


    close(sockFD);
    return 0;
}