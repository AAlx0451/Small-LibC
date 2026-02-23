#include <stdint.h>
#include <arpa/inet.h>

uint32_t ntohl(uint32_t netlong) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return ((netlong & 0x000000FF) << 24) |
           ((netlong & 0x0000FF00) << 8) |
           ((netlong & 0x00FF0000) >> 8) |
           ((netlong & 0xFF000000) >> 24);
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return netlong;
#endif
}
