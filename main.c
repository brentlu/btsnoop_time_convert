#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

// Epoch in microseconds since 01/01/0000.
static const unsigned long long int BTSNOOP_EPOCH_DELTA = 0x00dcddb30f2f8000ULL;

static void usage(char *ap)
{
    printf("usage: %s <btsnoop time or local time>\n\n", ap);
    printf("   ex: %s 63701068761984000\n", ap);
    printf("       %s \"2018 07-29 05:19:21.984\"\n", ap);
}

static unsigned long long int local_to_btsnoop_time(struct timeval *tv)
{
    unsigned long long int timestamp = tv->tv_sec * 1000 * 1000LL;

    timestamp += tv->tv_usec;
    timestamp += BTSNOOP_EPOCH_DELTA;

    return timestamp;
}

static int btsnoop_to_local_time(unsigned long long int btsnoop_time, struct timeval *local_time)
{
    if (btsnoop_time < BTSNOOP_EPOCH_DELTA) {
        printf("error: invalid btsnoop time %llu\n", btsnoop_time);
        return -1;
    }

    btsnoop_time -= BTSNOOP_EPOCH_DELTA;
    local_time->tv_sec = btsnoop_time / 1000000;
    local_time->tv_usec = btsnoop_time % 1000000;

    return 0;
}

int main(int argc, char **argv)
{
    int ret;
    struct tm info;
    struct tm* ptm;
    char buffer[80];

    struct timeval tv;
    unsigned long long int btsnoop_time;

    if (argc != 2) {
        usage(argv[0]);
        return 0;
    }

    ret = sscanf(argv[1], "%d %d-%d %d:%d:%d.%d", &info.tm_year, &info.tm_mon,
                    &info.tm_mday, &info.tm_hour, &info.tm_min, &info.tm_sec,
                    (int *)&tv.tv_usec);
    if (ret == 7) {
        info.tm_year -= 1900;
        info.tm_mon -= 1;
        info.tm_isdst = -1;

        tv.tv_sec = mktime(&info);
        if (tv.tv_sec == -1) {
            printf("error: mktime fail\n");
        } else {
           strftime(buffer, sizeof(buffer), "%c", &info);
           printf("local time:   %s\n", buffer);

           tv.tv_usec *= 1000;
           btsnoop_time = local_to_btsnoop_time(&tv);

           printf("btsnoop time: %llu\n", btsnoop_time);
        }

        return 0;
    }

    ret = sscanf(argv[1], "%llu", &btsnoop_time);
    if (ret == 1) {
        if (btsnoop_to_local_time(btsnoop_time, &tv)) {
            printf("error: btsnoop_to_local_time fail\n");
            return 0;
        }

        printf("btsnoop time: %llu\n", btsnoop_time);

        ptm = localtime(&tv.tv_sec);
        if (ptm == 0) {
            printf("error: localtime fail\n");
        } else {
            strftime(buffer, sizeof(buffer), "%Y %m-%d %H:%M:%S", ptm);
            printf("local time:   %s.%03ld\n", buffer, tv.tv_usec / 1000);
        }

        return 0;
    }

    usage(argv[0]);
    return 0;
}
