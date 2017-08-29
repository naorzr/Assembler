
#ifndef ENDPROJECT_LOGGER_H
#define ENDPROJECT_LOGGER_H

/* Enum for the various log levels to trace */
typedef enum
{
    LOG_ERROR,
    LOG_INFO,
    LOG_DEBUG
} LogLevel;

/* Defines the current log level (if set to DEBUG, all debug traces and above will appear)
 * Default - LOG_INFO */
#define SELECTED_LOG_LEVEL LOG_INFO

void log_trace(LogLevel lvl, char *fmt, ...);


#endif /*ENDPROJECT_LOGGER_H*/