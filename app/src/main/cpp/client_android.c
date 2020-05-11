#include "client_android.h"
#include "./cellular-measurement/bidirectional/receive_bandwidth.h"
#include "./cellular-measurement/bidirectional/send_bandwidth.h"
#include "./cellular-measurement/bidirectional/controller.h"
#include "./cellular-measurement/bidirectional/net_utils.h"

/**
 * Starts controller on the Android side
 */
void android_start_controller(const char * address, struct parameters params, int port) {
    if (params.use_tcp) {
        int client_send_sk  = setup_tcp_socket_send(address, port);
        client_send_bandwidth_tcp(client_send_sk);
    } else {
        int client_send_sk = setup_bound_socket(port);
        struct sockaddr_in client_send_addr = addrbyname(address, port);
        start_controller(true, client_send_addr, client_send_sk, params);
    }
}

/**
 * Receives bandwidth measurement on the Android side
 */
void android_receive_bandwidth(const char * address, struct parameters params, int port) {
    if (params.use_tcp) {
        int client_recv_sk  = setup_tcp_socket_send(address, port);
        client_receive_bandwidth_tcp(client_recv_sk);
    } else {
        int client_recv_sk = setup_bound_socket(port);
        struct sockaddr_in client_recv_addr = addrbyname(address, port);

        receive_bandwidth(client_recv_sk, client_recv_addr, params, true);
    }
}

/**
 * Sends START packet to the server with all parameters and wait for ACKs.
 * Once connected with the server, return 0.
 */
int start_client(const char *address, struct parameters params, int client_send_port, int client_recv_port)
{
    int client_send_sk = setup_bound_socket(client_send_port);
    int client_recv_sk = setup_bound_socket(client_recv_port);

    struct sockaddr_in client_send_addr = addrbyname(address, client_send_port);
    struct sockaddr_in client_recv_addr = addrbyname(address, client_recv_port);

    // select loop
    fd_set mask;
    fd_set read_mask;
    FD_ZERO(&mask);
    FD_SET(client_send_sk, &mask);
    FD_SET(client_recv_sk, &mask);
    struct timeval timeout;
    int num, len;

    start_packet start_pkt;
    data_packet data_pkt;
    struct sockaddr_in from_addr;
    socklen_t from_len = sizeof(from_addr);
    char buf[sizeof(start_pkt)]; //buffer to serialize struct

    bool got_send_ack = false;
    bool got_recv_ack = false;

    // initiate handshake packet and send to the server
    start_pkt.type = NETWORK_START;
    start_pkt.params = params;

    int buf_len = serializeStruct(&start_pkt, buf);

    for (;;)
    {
        read_mask = mask;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        // re-send NETWORK_START packets when timeout
        if (!got_send_ack) {
            sendto(client_send_sk, buf, buf_len, 0,
                       (struct sockaddr *)&client_send_addr, sizeof(client_send_addr));
        }

        if (!got_recv_ack) {
            sendto(client_recv_sk, buf, buf_len, 0,
                       (struct sockaddr *)&client_recv_addr, sizeof(client_recv_addr));
        }

        printf("re-sending NETWORK_START\n");

        num = select(FD_SETSIZE, &read_mask, NULL, NULL, &timeout);

        if (num > 0)
        {
            if (FD_ISSET(client_send_sk, &read_mask))
            {
                len = recvfrom(client_send_sk, &data_pkt, sizeof(data_packet), 0,
                               (struct sockaddr *)&from_addr, &from_len);
                if (len < 0)
                {
                    perror("socket error");
                    return 1;
                }

                if (data_pkt.hdr.type == NETWORK_START_ACK)
                {
                    printf("got send ack\n");
                    got_send_ack = true;
                    close(client_send_sk);
                    FD_CLR(client_send_sk, &mask);
                }

                if (data_pkt.hdr.type == NETWORK_BUSY)
                {
                    printf("server is busy\n");
                    close(client_send_sk);
                    close(client_recv_sk);
                    return 2;
                }
            }
            if (FD_ISSET(client_recv_sk, &read_mask))
            {
                len = recvfrom(client_recv_sk, &data_pkt, sizeof(data_packet), 0,
                               (struct sockaddr *)&from_addr, &from_len);
                if (len < 0)
                {
                    perror("socket error");
                    exit(1);
                }

                if (data_pkt.hdr.type == NETWORK_START_ACK)
                {
                    printf("got recv ack\n");
                    got_recv_ack = true;
                    close(client_recv_sk);
                    FD_CLR(client_recv_sk, &mask);
                }

                if (data_pkt.hdr.type == NETWORK_BUSY)
                {
                    printf("server is busy\n");
                    close(client_recv_sk);
                    close(client_send_sk);
                    return 2;
                }
            }
        }
        else
        {
            printf(".");
            fflush(0);

            if (num < 0) {
                perror("num is negative\n");
                exit(1);
            }
        }

        if (got_recv_ack && got_send_ack)
        {
            return 0;
        }
    }
}
