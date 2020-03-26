//
// Created by Jerry Chen on 2020-02-12.
//

#ifndef UDP_TOOLS_INTERACTIVE_CLIENT_H
#define UDP_TOOLS_INTERACTIVE_CLIENT_H

#include "net_include.h"

/**
 * send an interactive packet to the server and returns the sequence number
 * @param x x_coordinate
 * @param y y_coordinate
 * @return status code
 */
int send_interactive_packet(int seq_num, float x, float y);

/**
 * receive an interactive packet with a certain sequence number
 * @return float array [x_coord, y_coord, sequence_number]
 */
float * receive_interactive_packet();

float * send_and_receive_interactive_packet(int seq_num, float x, float y);
void init_socket(const char * address, int port);

#endif //UDP_TOOLS_INTERACTIVE_CLIENT_H
