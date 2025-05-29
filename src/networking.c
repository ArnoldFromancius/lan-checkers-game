#include "../include/networking.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

const char *get_local_ip() {
    static char ip[INET_ADDRSTRLEN] = "Unknown";

    int temp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (temp_sock == -1) return ip;

    struct sockaddr_in fake_addr;
    memset(&fake_addr, 0, sizeof(fake_addr));
    fake_addr.sin_family = AF_INET;
    fake_addr.sin_port = htons(80);  // arbitrary
    inet_pton(AF_INET, "8.8.8.8", &fake_addr.sin_addr);  // Google's DNS

    connect(temp_sock, (struct sockaddr*)&fake_addr, sizeof(fake_addr));

    struct sockaddr_in local_addr;
    socklen_t addr_len = sizeof(local_addr);
    getsockname(temp_sock, (struct sockaddr*)&local_addr, &addr_len);

    inet_ntop(AF_INET, &local_addr.sin_addr, ip, sizeof(ip));

    close(temp_sock);
    return ip;
}
