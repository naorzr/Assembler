
#ifndef ENDPROJECT_LOGGER_H
#define ENDPROJECT_LOGGER_H


typedef enum
{
    LOG_ERROR,
    LOG_INFO,
    LOG_DEBUG
} LogLevel;

#define SELECTED_LOG_LEVEL LOG_INFO

void log_trace(LogLevel lvl, char *fmt, ...);


#endif /*ENDPROJECT_LOGGER_H*/