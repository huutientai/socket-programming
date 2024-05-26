#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT "3490" // cổng mà client sẽ kết nối đến
#define MAXDATASIZE 4096 // kích thước tối đa của dữ liệu mà client có thể nhận được

// Hàm để lấy địa chỉ IP từ cấu trúc sockaddr
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[]) {
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr, "usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // Lặp qua tất cả các kết quả và kết nối với cái đầu tiên mà chúng ta có thể
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // Giải phóng danh sách kết quả

    // Nhận dữ liệu từ server
    int totalBytesReceived = 0;
    while ((numbytes = recv(sockfd, buf + totalBytesReceived, MAXDATASIZE - totalBytesReceived - 1, 0)) > 0) {
        totalBytesReceived += numbytes;
    }

    if (numbytes == -1) {
        perror("recv");
        exit(1);
    }

    // Đảm bảo kết thúc chuỗi dữ liệu bằng ký tự null
    buf[totalBytesReceived] = '\0';

    printf("client: received '%s'\n", buf);

    close(sockfd);

    return 0;
}
