
#ifndef ENDPROJECT_LOGGER_H
#define ENDPROJECT_LOGGER_H

#endif //ENDPROJECT_LOGGER_H

typedef enum
{
    LOG_ERROR,
    LOG_INFO,
    LOG_DEBUG
} LOG_LEVEL;

#define SELECTED_LOG_LEVEL LOG_INFO

void LOG_TRACE(LOG_LEVEL lvl, char *fmt, ... );