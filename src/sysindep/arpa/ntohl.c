#include <stdint.h>

uint32_t ntohl(uint32_t netlong) {
    return ((netlong & 0xFF) << 24) |
           ((netlong & 0xFF00) << 8) |
           ((netlong & 0xFF0000) >> 8) |
           ((netlong & 0xFF000000) >> 24);
}
