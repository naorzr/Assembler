
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

/**
 * Prints formatted string into the relevant output according to the log level
 * LOG_LEVEL = LOG_DEBUG - will print to output all messages from type debug/info/error
 * LOG_LEVEL = LOG_INFO - will print to output all messages from type info/error
 * LOG_LEVEL = LOG_ERROR - will print to output messages from type error
 * @param lvl - LOG_LEVEL
 * @param fmt - string format of the arguments
 * @param ... accepts 1 and more arguments like printf
 */
void log_trace(LogLevel lvl, char *fmt, ...);


#endif /*ENDPROJECT_LOGGER_H*/