#include <netdb.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        printf("Enter hostname\n");
        return -1;
    }

    struct hostent *h;
    h = gethostbyname(argv[1]);
    if (h == NULL) {
        printf("Error\n");
        return -1;
    }

    int i = 0;
    while (NULL != h->h_addr_list[i]) {
        struct in_addr *a = (struct in_addr *) h->h_addr_list[i];
        printf("%s\n", inet_ntoa(*a));
        i++;
    }

    return 0;
}
