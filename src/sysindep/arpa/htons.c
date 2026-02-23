#include <arpa/inet.h>
#include <stdint.h>

uint16_t htons(uint16_t hostshort) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return (uint16_t)((hostshort & 0x00FF) << 8) | ((hostshort & 0xFF00) >> 8);
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return hostshort;
#endif
}
