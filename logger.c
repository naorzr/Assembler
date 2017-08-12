#include <stdio.h>
#include <stdarg.h>
#include "logger.h"


/* LOG_TRACE(log level, format, args ) */
void LOG_TRACE(LOG_LEVEL lvl, char *fmt, ...) {
    va_list list;
    char *s, c;
    int i;

    if (lvl <= SELECTED_LOG_LEVEL) {
        FILE * loc = stdout;

        if(lvl == LOG_ERROR) {
            loc = stderr;
        }
        va_start(list, fmt);

        while (*fmt) {
            if (*fmt != '%')
                putc(*fmt, loc);
            else {
                switch (*++fmt) {
                    case 's':
                        /* set r as the next char in list (string) */
                        s = va_arg(list,char *);
                        fprintf(loc, "%s", s);
                        break;

                    case 'd':
                        i = va_arg(list, int);
                        fprintf(loc, "%d", i);
                        break;

                    case 'c':
                        c = va_arg(list, int);
                        fprintf(loc, "%c", c);
                        break;

                    default:
                        putc(*fmt, loc);
                        break;
                }
            }
            ++fmt;
        }
        va_end(list);
    }
    fflush(stdout);
}