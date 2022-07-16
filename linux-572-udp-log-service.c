#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

struct sockaddr_in local;

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        printf("Enter port number\n");
        return -1;
    }

    int s_id = socket(AF_INET, SOCK_DGRAM, 0);
    if (s_id == -1) {
        printf("Socket was not created\n");
        return -1;
    }

    inet_aton("127.0.0.1", &local.sin_addr);
    local.sin_port = htons(atoi(argv[1]));
    local.sin_family = AF_INET;

    bind(s_id, (struct sockaddr *) &local, sizeof(local));

    int BUFFER_SIZE = 5000;
    char buf[BUFFER_SIZE];

    while (1) {
        int len = read(s_id, buf, BUFFER_SIZE);
        buf[len] = '\0';
        if (strcmp(buf, "OFF\n") == 0)
            break;
        else
            printf("%s\n", buf);
    }

    return 0;
}
