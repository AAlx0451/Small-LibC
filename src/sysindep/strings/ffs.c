#include <strings.h>

int ffs(int i) {
    int bit = 1;
    if(i == 0) {
        return 0;
    }
    while(!(i & 1)) {
        i >>= 1;
        bit++;
    }
    return bit;
}
