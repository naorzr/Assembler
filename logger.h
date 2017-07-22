typedef enum
{
    LOG_ERROR,
    LOG_INFO,
    LOG_DEBUG
} LOG_LEVEL;

#define SELECTED_LOG_LEVEL LOG_DEBUG

void LOG_TRACE(LOG_LEVEL lvl, char *fmt, ... );