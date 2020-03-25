//
// Created by Jerry Chen on 3/25/20.
//

#ifndef UDP_TOOLS_LOGGER_H
#define UDP_TOOLS_LOGGER_H

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "android/log.h"


int start_logger(const char *app_name);

static void *thread_func(void*);
#endif //UDP_TOOLS_LOGGER_H
