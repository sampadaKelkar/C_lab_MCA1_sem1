#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

/* Read integer using fgets -> avoids scanf newline problems */
int getInt(char *prompt, int min, int max) {
    char buf[128];
    long val;
    int ok = 0;

    while(!ok) {
        printf("%s", prompt);
        if(!fgets(buf, sizeof(buf), stdin)) {
            /* input error, return min */
            return min;
        }
        buf[strcspn(buf, "\n")] = '\0';
        val = strtol(buf, NULL, 10);
        if(val < min || val > max) {
            printf("Please enter a number between %d and %d.\n", min, max);
        } else {
            ok = 1;
        }
    }
    return (int)val;
}

/* Get string safely (fgets), trims newline */
void getString(char *prompt, char *str, int size) {
    char buf[512];
    printf("%s", prompt);
    if(!fgets(buf, sizeof(buf), stdin)) {
        buf[0] = '\0';
    }
    buf[strcspn(buf, "\n")] = '\0';
    strncpy(str, buf, size-1);
    str[size-1] = '\0';
}

int validateNonEmptyString(char *str) {
    if(str == NULL) return 0;
    return (str[0] != '\0');
}

/* Parse YYYY-MM-DD -> time_t (midnight local time). Returns (time_t)-1 on invalid */
time_t parseDateString(const char *dateStr) {
    int year, month, day;
    if(sscanf(dateStr, "%d-%d-%d", &year, &month, &day) != 3) return (time_t)-1;
    if(month < 1 || month > 12 || day < 1 || day > 31) return (time_t)-1;

    {
        struct tm t;
        memset(&t, 0, sizeof(t));
        t.tm_year = year - 1900;
        t.tm_mon = month - 1;
        t.tm_mday = day;
        t.tm_hour = 0;
        t.tm_min = 0;
        t.tm_sec = 0;
        t.tm_isdst = -1;
        return mktime(&t);
    }
}

/* format time_t as YYYY-MM-DD */
void formatDate(time_t t, char *out, int size) {
    struct tm *tm_info;
    tm_info = localtime(&t);
    if(tm_info == NULL) {
        strncpy(out, "0000-00-00", size-1);
        out[size-1] = '\0';
        return;
    }
    strftime(out, size, "%Y-%m-%d", tm_info);
}

