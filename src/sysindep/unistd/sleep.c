#include<sys/time.h>
#include<sys/select.h>

unsigned int sleep(unsigned int seconds) {
    struct timeval tv;
    tv.tv_sec = seconds;
    tv.tv_usec = 0;
    select(0, 0, 0, 0, &tv); 
    return 0;
}
