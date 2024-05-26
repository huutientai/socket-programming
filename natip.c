#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>

void get_ip_address(const char *iface) {
    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    strncpy(ifr.ifr_name, iface, IFNAMSIZ-1);
    ifr.ifr_name[IFNAMSIZ-1] = '\0';

    if (ioctl(fd, SIOCGIFADDR, &ifr) == -1) {
        perror("ioctl");
        close(fd);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in *ip_addr = (struct sockaddr_in *)&ifr.ifr_addr;
    printf("IP Address of %s: %s\n", iface, inet_ntoa(ip_addr->sin_addr));

    close(fd);
}

int main() {
    const char *iface = "eth0";  // Change this to your network interface name
    get_ip_address(iface);
    return 0;
}
