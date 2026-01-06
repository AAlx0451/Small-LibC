#include <netinet/in.h>
#include <arpa/inet.h>

in_addr_t inet_addr(const char *cp) {
    struct in_addr addr;

    if (inet_aton(cp, &addr)) {
        return addr.s_addr;
    }
    return INADDR_NONE;
}
