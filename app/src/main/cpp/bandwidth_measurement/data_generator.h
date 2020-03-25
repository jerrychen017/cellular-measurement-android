#ifndef BANDWIDTH_DATA_GENERATOR_H
#define BANDWIDTH_DATA_GENERATOR_H
#include "net_include.h"
#include "bandwith_utils.h"

#define DATA_SIZE (PACKET_SIZE - sizeof(data_header))

int setup_socket();
int start_generator();

#endif
