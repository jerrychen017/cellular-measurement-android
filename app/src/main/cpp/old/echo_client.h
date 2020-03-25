//
// Created by Jerry Chen on 2020-02-12.
//

#ifndef UDP_TOOLS_ECHO_CLIENT_H
#define UDP_TOOLS_ECHO_CLIENT_H
#include "net_include.h"
#include "old_utils.h"

char * echo_client(const char* address, int port, int seq);
int client_bind(const char* address, int port);
char * client_send(const char* address, int port);
int draw_client(const char* address, int port, int sequence);
#endif //UDP_TOOLS_ECHO_CLIENT_H
