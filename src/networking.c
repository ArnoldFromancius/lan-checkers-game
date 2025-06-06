#include "../include/networking.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>  // 🧵 added for threading

static int sockfd = -1;
static int serverSocket = -1;
static pthread_t accept_thread;
static bool clientConnected = false;

// 🧵 Thread function to handle accept() without freezing UI
void *acceptClientThread(void *arg) {
    int client = accept(serverSocket, NULL, NULL);
    if (client >= 0) {
        close(serverSocket);
        serverSocket = -1;
        sockfd = client;
        clientConnected = true;
    }
    return NULL;
}

const char *get_local_ip() {
    static char ip[INET_ADDRSTRLEN] = "Unknown";

    int temp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (temp_sock == -1) return ip;

    struct sockaddr_in fake_addr;
    memset(&fake_addr, 0, sizeof(fake_addr));
    fake_addr.sin_family = AF_INET;
    fake_addr.sin_port = htons(80);
    inet_pton(AF_INET, "8.8.8.8", &fake_addr.sin_addr);

    connect(temp_sock, (struct sockaddr*)&fake_addr, sizeof(fake_addr));

    struct sockaddr_in local_addr;
    socklen_t addr_len = sizeof(local_addr);
    getsockname(temp_sock, (struct sockaddr*)&local_addr, &addr_len);

    inet_ntop(AF_INET, &local_addr.sin_addr, ip, sizeof(ip));

    close(temp_sock);
    return ip;
}

bool networkSetup(bool isHost, const char *ip) {
    struct sockaddr_in addr;
    if (isHost) {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0) return false;

        addr.sin_family = AF_INET;
        addr.sin_port = htons(PORT);
        addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(serverSocket, (struct sockaddr *)&addr, sizeof(addr)) < 0) return false;
        if (listen(serverSocket, 1) < 0) return false;

        // 🧵 Start thread to accept client in background
        pthread_create(&accept_thread, NULL, acceptClientThread, NULL);
    } else {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) return false;

        addr.sin_family = AF_INET;
        addr.sin_port = htons(PORT);
        addr.sin_addr.s_addr = inet_addr(ip);

        if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) return false;
        clientConnected = true;
    }

    return true;
}

bool sendMove(MovePacket move) {
    return send(sockfd, &move, sizeof(move), 0) == sizeof(move);
}

bool receiveMove(MovePacket *move) {
    return recv(sockfd, move, sizeof(*move), 0) == sizeof(*move);
}

void closeNetwork() {
    if (sockfd != -1) {
        close(sockfd);
        sockfd = -1;
    }
    if (serverSocket != -1) {
        close(serverSocket);
        serverSocket = -1;
    }
}

// 🧩 This function lets your game check if the client has connected
bool isClientConnected() {
    return clientConnected;
}
