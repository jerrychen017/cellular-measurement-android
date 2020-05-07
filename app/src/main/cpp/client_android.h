#ifndef UDP_TOOLS_CLIENT_ANDROID_H
#define UDP_TOOLS_CLIENT_ANDROID_H
#include "./cellular-measurement/bidirectional/net_include.h"

/**
 * Sends START packet to the server with all parameters and wait for ACKs.
 * Once connected with the server, return 0.
 */
int start_client(const char *address, struct parameters params);

/**
 * Starts controller on the Android side
 */
void android_start_controller(const char * address, struct parameters params);

/**
 * Receives bandwidth measurement on the Android side
 */
void android_receive_bandwidth(const char * address, struct parameters params);

#endif //UDP_TOOLS_CLIENT_ANDROID_H
