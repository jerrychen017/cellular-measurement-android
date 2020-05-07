#ifndef UDP_TOOLS_LOGGER_H
#define UDP_TOOLS_LOGGER_H

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include "android/log.h"

/**
 * Starts logger to redirect stdout to Android debug log
 */
int start_logger(const char *app_name);

/**
 * redirect stdout to Android debug log
 */
static void *thread_func(void*);

#endif
