#include <netinet/in.h>
#include <stdio.h> // for snprintf

char *inet_ntoa(struct in_addr in) {
    static char buffer[18];

    unsigned char b1 = (in.s_addr >> 24) & 0xFF;
    unsigned char b2 = (in.s_addr >> 16) & 0xFF;
    unsigned char b3 = (in.s_addr >> 8) & 0xFF;
    unsigned char b4 = (in.s_addr >> 0) & 0xFF;

    snprintf(buffer, sizeof(buffer), "%u.%u.%u.%u", b1, b2, b3, b4);
    
    return buffer;
}
