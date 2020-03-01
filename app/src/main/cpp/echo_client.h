//
// Created by Jerry Chen on 2020-02-12.
//

#ifndef UDP_TOOLS_ECHO_CLIENT_H
#define UDP_TOOLS_ECHO_CLIENT_H
#include "net_include.h"
#include "utils.h"

double echo_client_start(int port, const char* address, bool resend);
int client_bind(const char* address, int port);
char * client_send(const char* address, int port);
#endif //UDP_TOOLS_ECHO_CLIENT_H
