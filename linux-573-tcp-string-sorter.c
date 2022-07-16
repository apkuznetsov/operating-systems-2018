#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

struct sockaddr_in server_addr;

int compare_char(const void *a, const void *b)
{
    if (*(char *) a != *(char *) b)
        return *(char *) b - *(char *) a;

    return 0;
}

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        printf("Enter port number");
        return -1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Socket is not created\n");
        return -1;
    }

    inet_aton("127.0.0.1", &server_addr.sin_addr);
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_family = AF_INET;

    if (bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        printf("Socket was not bound\n");
        exit(1);
    }

    if (listen(sock, 5) == -1) {
        printf("Socket was not listened\n");
        return -1;
    }

    int cs = accept(sock, NULL, NULL);
    if (cs == -1) {
        printf("Socket was not accepted\n");
        return -1;
    }

    while (1) {
        char buf[BUFSIZ] = {0};
        char sorted_buf[BUFSIZ] = {0};
        int read_bytes = read(cs, buf, BUFSIZ);

        if (strncmp("OFF", buf, 3) == 0) {
            break;
        }

        strncpy(sorted_buf, buf, sizeof buf);
        qsort(sorted_buf, strlen(buf), sizeof *buf, compare_char);
        write(cs, sorted_buf, read_bytes);
    }

    close(cs);
    return 0;
}
