#include <stdint.h>
#include <arpa/inet.h>

uint16_t ntohs(uint16_t netshort) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return (uint16_t)
           ((netshort & 0x00FF) << 8) |
           ((netshort & 0xFF00) >> 8);
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return netshort;
#endif
}
