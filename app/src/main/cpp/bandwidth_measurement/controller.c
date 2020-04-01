#include "controller.h"
/**
    Controller 
*/
int start_controller(const char* address, int port)
{

//    // args error checking
//    if (argc != 3)
//    {
//        printf("controller usage: controller <host_address> <port>\n");
//        exit(1);
//    }

    // port
//    int port = atoi(argv[2]);
    // address
//    char *address = argv[1];

    int s_server = setup_server_socket(port);
    struct sockaddr_in send_addr = addrbyname(address, port);
    int s_data = setup_data_socket();

    startup(s_server, s_data, send_addr);
    control(s_server, s_data, send_addr);

    return 0;
}

/* Pings data generator to check that it is there and then starts data stream */
void startup(int s_server, int s_data, struct sockaddr_in send_addr)
{
    typed_packet pkt;
    pkt.type = LOCAL_START;
    send(s_data, &pkt, sizeof(pkt.type), 0);
    printf("control: sent LOCAL_START");
}

/* Main event loop */
void control(int s_server, int s_data, struct sockaddr_in send_addr)
{
    struct sockaddr_in server_addr;
    socklen_t server_addr_len;

    fd_set mask;
    fd_set read_mask;
    int num;

    FD_ZERO(&mask);
    FD_SET(s_server, &mask);
    FD_SET(s_data, &mask);

    struct timeval timeout;
    data_packet data_pkt;
    data_packet recv_pkt;

    // Varibales to deal with burst
    int burst_seq_recv = -1; // index of burst we have received from data
    int burst_seq_send = -1; // index of burst we have sent
    data_packet pkt_buffer[BURST_SIZE];

    int seq = 0;
    double rate = 1.0;



    for (;;) {
        read_mask = mask;

        num = select(FD_SETSIZE, &read_mask, NULL, NULL, &timeout);

        if (num > 0) {
            if (FD_ISSET(s_data, &read_mask)) {
                int len = recv(s_data, data_pkt.data, sizeof(data_pkt.data), 0);
                if (len == 0) {
                    printf("data stream ended, exiting...\n");
                    close(s_data);
                    close(s_server);
                    exit(0);
                }

                // Start burst
                if (seq % INTERVAL_SIZE == INTERVAL_SIZE - BURST_SIZE) {
                    printf("starting burst at seq %d\n", seq);
                    burst_seq_recv = 0; 
                }

                data_pkt.hdr.type = burst_seq_recv == -1 ? NETWORK_DATA : NETWORK_PROBE;
                data_pkt.hdr.seq_num = seq;
                data_pkt.hdr.rate = burst_seq_recv == -1 ? rate : rate * BURST_FACTOR;


                if (burst_seq_recv == -1) {
                    printf("Sending to server \n");

                    // Pass to server during normal operation
                    sendto(s_server, &data_pkt, sizeof(data_pkt), 0,
                            (struct sockaddr *) &send_addr, sizeof(send_addr));
                } else {
                    printf("storing packet seq %d, in spot %d\n", seq, burst_seq_recv);
                    pkt_buffer[burst_seq_recv] = data_pkt;
                    
                    burst_seq_recv++;

                    // We have recieved all the packets in the burst, so we can stop storing
                    if (burst_seq_recv == BURST_SIZE) {
                        burst_seq_recv = -1;
                    }
                    // After receiving half of the packets, we can start sending
                    if (burst_seq_recv == BURST_SIZE / 2) {
                        burst_seq_send = 0;
                        timeout = speed_to_interval(rate * BURST_FACTOR);
                    }
                }
                seq++;
            }
            if (FD_ISSET(s_server, &read_mask)) {
                int len = recvfrom(s_server, &recv_pkt, sizeof(data_packet), 0, (struct sockaddr *)&server_addr, &server_addr_len);
                if (len <= 0) {
                    printf("data stream ended, exiting...\n");
                    close(s_data);
                    close(s_server);
//                    exit(0);
                    return;
                }
                if(recv_pkt.hdr.type == NETWORK_REPORT){
                    printf("controller: received network report packet \n");
                }
            }
        } else {
            // timeout
            if (burst_seq_send == -1) {
                printf("timeout?\n");
            }
            else {  
                printf("sending packet %d of burst\n", burst_seq_send);
                sendto(s_server, &pkt_buffer[burst_seq_send], sizeof(data_pkt), 0,
                        (struct sockaddr *) &send_addr, sizeof(send_addr));
                burst_seq_send++;
                timeout = speed_to_interval(rate * BURST_FACTOR);
                if (burst_seq_send == BURST_SIZE) {
                    burst_seq_send = -1;
                }
            }
        }

        // if we are not sending at burst, we can just send when the data
        // source gives us a packet. However, if we are sending a burst,
        // we want to timeout to send at a certain rate
        if (burst_seq_send == -1) {
            timeout.tv_sec = TIMEOUT_SEC;
            timeout.tv_usec = TIMEOUT_USEC;
        }
    }
}

int setup_data_socket()
{
    int s, s2;
    socklen_t len, len2;

    struct sockaddr_un addr1, addr2;



    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        printf("socket error\n");
        exit(1);
    }

    printf("Trying to connect...\n");

    memset(&addr1, 0, sizeof(addr1)); // fix
    addr1.sun_family = AF_UNIX;

    // fixing socket error
    const char name[] = "\0my.local.socket.address";
// size-1 because abstract socket names are *not* null terminated
    memcpy(addr1.sun_path, name, sizeof(name) - 1);

//    strcpy(addr1.sun_path, SOCK_PATH);
//    len = strlen(addr1.sun_path) + sizeof(addr1.sun_family);
    len = strlen(addr1.sun_path) + sizeof(name); // fix

    addr1.sun_path[0] = 0;

    unlink(addr1.sun_path);
    if (bind(s, (struct sockaddr *)&addr1, len) == -1) {
        perror("bind error in setup_data_socket");
        exit(1);
        return 1;
    }

    if (listen(s, 5) == -1) {
        perror("listen");
        exit(1);
    }

    // fixing socket error
    const char name2[] = "\0my2.local.socket.address";
// size-1 because abstract socket names are *not* null terminated
    memcpy(addr2.sun_path, name2, sizeof(name2) - 1);

    printf("Waiting for a connection...\n");
//    len2 = sizeof(addr1);
    len2 = strlen(addr2.sun_path) + sizeof(name); // fix
    addr2.sun_path[0] = 0;
    if ((s2 = accept(s, (struct sockaddr *)&addr2, &len2)) == -1) {
        printf("controller: accept error\n");
        exit(1);
    }

    printf("Connected.\n");

    return s2;
}

int setup_server_socket(int port)
{
    struct sockaddr_in name;

    int s_recv = socket(AF_INET, SOCK_DGRAM, 0);  /* socket for receiving (udp) */
    if (s_recv < 0) {
        printf("socket recv error\n");
        exit(1);
    }

    name.sin_family = AF_INET;
    name.sin_addr.s_addr = INADDR_ANY;
    name.sin_port = htons(port);

//    if (bind( s_recv, (struct sockaddr *)&name, sizeof(name) ) < 0 ) {
//        printf("bind error\n");
//        exit(1);
//    }

    return s_recv;
}

struct sockaddr_in addrbyname(const char *hostname, int port)
{
    int host_num;
    struct hostent h_ent, *p_h_ent;

    struct sockaddr_in addr;

    p_h_ent = gethostbyname(hostname);
    if (p_h_ent == NULL) {
        printf("gethostbyname error.\n");
        exit(1);
    }

    memcpy( &h_ent, p_h_ent, sizeof(h_ent));
    memcpy( &host_num, h_ent.h_addr_list[0], sizeof(host_num) );

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = host_num;
    addr.sin_port = htons(port);

    return addr;
}

double estimate_change(double rate){
    return 0;
}