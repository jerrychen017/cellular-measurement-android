#include "client_android.h"
#include "./cellular-measurement/bidirectional/receive_bandwidth.h"
#include "./cellular-measurement/bidirectional/controller.h"
#include "./cellular-measurement/bidirectional/net_utils.h"


void android_start_controller(const char * address, struct parameters params) {
    int client_send_sk = setup_bound_socket(CLIENT_SEND_PORT);
    struct sockaddr_in client_send_addr = addrbyname(address, CLIENT_SEND_PORT);
    start_controller(true, client_send_addr, client_send_sk, params);
}

void android_receive_bandwidth(const char * address, struct parameters params) {
    int client_recv_sk = setup_bound_socket(CLIENT_RECEIVE_PORT);
    struct sockaddr_in client_recv_addr = addrbyname(address, CLIENT_RECEIVE_PORT);
    receive_bandwidth(client_recv_sk, client_recv_addr, params, true);
}

int start_client(const char *address, struct parameters params)
{
//     printf("burst size is %d\n", params.burst_size);
//     printf("interval_size is %d\n", params.interval_size);
//     printf("interval_time is %f\n", params.interval_time);
//     printf("instant_burst is %d\n", params.instant_burst);
//     printf("burst_factor is %d\n", params.burst_factor);
//     printf("min_speed is %f\n", params.min_speed);
//     printf("max_speed is %f\n", params.max_speed);
//     printf("start_speed is %f\n", params.start_speed);
//     printf("grace_period is %d\n", params.grace_period);
//     printf("threshold is %f\n", params.threshold);
//     printf("alpha is %f\n", params.alpha);
//     printf("pred_mode is %d\n", params.pred_mode);
//     printf("size of params is %d\n", sizeof(params));
//     printf("size of int is %d\n", sizeof(int));
//     printf("size of double is %d\n", sizeof(double));
//     printf("size of bool is %d\n", sizeof(bool));


    int client_send_sk = setup_bound_socket(CLIENT_SEND_PORT);
    int client_recv_sk = setup_bound_socket(CLIENT_RECEIVE_PORT);

    struct sockaddr_in client_send_addr = addrbyname(address, CLIENT_SEND_PORT);
    struct sockaddr_in client_recv_addr = addrbyname(address, CLIENT_RECEIVE_PORT);

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
            sendto_dbg(client_send_sk, buf, buf_len, 0,
                       (struct sockaddr *)&client_send_addr, sizeof(client_send_addr));
        }

        if (!got_recv_ack) {
            sendto_dbg(client_recv_sk, buf, buf_len, 0,
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
