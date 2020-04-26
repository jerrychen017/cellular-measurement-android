#ifndef UDP_TOOLS_CLIENT_ANDROID_H
#define UDP_TOOLS_CLIENT_ANDROID_H
#include "./cellular-measurement/bidirectional/net_include.h"

void start_client(const char *address, struct parameters params);
void android_start_controller(const char * address, struct parameters params);
void android_receive_bandwidth(const char * address, struct parameters params);
#endif //UDP_TOOLS_CLIENT_ANDROID_H
