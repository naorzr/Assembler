#include <stdio.h>
#include <stdarg.h>
#include "logger.h"



/**
 * Prints formatted string into the relevant output according to the log level
 * LOG_LEVEL = LOG_DEBUG - will print to output all messages from type debug/info/error
 * LOG_LEVEL = LOG_INFO - will print to output all messages from type info/error
 * LOG_LEVEL = LOG_ERROR - will print to output messages from type error
 * @param lvl - LOG_LEVEL
 * @param fmt - string format of the arguments
 * @param ... accepts 1 and more arguments like printf
 */
void log_trace(LogLevel lvl, char *fmt, ...) {
    va_list list;
    char *s, c;
    int i;

    if (lvl <= SELECTED_LOG_LEVEL) {
        FILE *loc = stdout;

        if (lvl == LOG_ERROR) { /* print errors to stderr */
            loc = stderr;
        }
        va_start(list, fmt);

        while (*fmt) {
            if (*fmt != '%')
                putc(*fmt, loc);
            else {
                switch (*++fmt) {
                    case 's': /* handle string formatting */
                        s = va_arg(list, char *);
                        fprintf(loc, "%s", s);
                        break;

                    case 'd': /* handle decimal formatting */
                        i = va_arg(list, int);
                        fprintf(loc, "%d", i);
                        break;

                    case 'c': /* handle char formatting */
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