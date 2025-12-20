#ifndef UTILS_H
#define UTILS_H

/* robust input helpers */
int getInt(char *prompt, int min, int max);
void getString(char *prompt, char *str, int size);
int validateNonEmptyString(char *str);

/* date helpers */
time_t parseDateString(const char *dateStr); /* "YYYY-MM-DD" -> time_t (midnight) */
void formatDate(time_t t, char *out, int size); /* format time_t into YYYY-MM-DD string */

#endif

