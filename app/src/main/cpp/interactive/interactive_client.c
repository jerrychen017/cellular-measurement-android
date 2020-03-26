#include "interactive_client.h"

// server socket address
static struct sockaddr_in server_addr;
// address of incoming packets
static struct sockaddr_in echo_pac_addr;
// client socket address
static struct sockaddr_in client_addr;
static socklen_t client_len;
static int sk;
// get server host address
static struct hostent *server_name;
static struct hostent server_name_copy;
static int server_fd;

static fd_set mask;
static fd_set read_mask;
static int num;

int send_interactive_packet(int seq_num, float x, float y) {

    // initialize starting packet and echo_packet
    EchoPacket sent_packet;
    sent_packet.type = ECHO;
    sent_packet.seq = seq_num;
    sent_packet.x = x;
    sent_packet.y = y;

// send init packet to rcv
    sendto(sk, (EchoPacket *) &sent_packet, sizeof(sent_packet), 0,
           (struct sockaddr *) &echo_pac_addr, sizeof(echo_pac_addr));
    printf("interactive packet is sent\n");
    return 0;
}

/**
 * receive an interactive packet with a certain sequence number
 * @return float array [x_coord, y_coord, sequence_number]
 */
float *receive_interactive_packet() {
    float *coord = malloc(3 * sizeof(float));
// initialize packet to be received
    EchoPacket received_packet;

    read_mask = mask;

    num = select(FD_SETSIZE, &read_mask, NULL, NULL, NULL);
    if (num > 0) {
        if (FD_ISSET(sk, &read_mask)) {
            client_len = sizeof(client_addr);
            recvfrom(sk, &received_packet, sizeof(received_packet), 0,
                     (struct sockaddr *) &client_addr, &client_len);
            coord[0] = received_packet.x;
            coord[1] = received_packet.y;
            coord[2] = received_packet.seq;
            return coord;
        }
    } else {
        printf("receive_interactive_packet timeout error!");
        coord[0] = -1;
        coord[1] = -1;
        coord[2] = -1;
        return coord;
    }
}

float *send_and_receive_interactive_packet(int seq_num, float x, float y) {
    float *coord = malloc(2 * sizeof(float));
    char *address = "128.220.221.21";
    int port = 4579;

// initialize starting packet and echo_packet
    EchoPacket init_packet, echo_packet;
    init_packet.type = ECHO;
    init_packet.seq = seq_num;
    init_packet.x = x;
    init_packet.y = y;

// server socket address
    struct sockaddr_in server_addr;
// address of incoming packets
    struct sockaddr_in echo_pac_addr;
// client socket address
    struct sockaddr_in client_addr;
    socklen_t client_len;

// socket both for sending and receiving
    int sk = socket(AF_INET, SOCK_DGRAM, 0);
    if (sk < 0) {
        perror("echo_client: socket error\n");
//        exit(1);
        coord[0] = -3;
        coord[1] = -3;
        return coord;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

// binding was done in client_bind

// get server host address
    struct hostent *server_name;
    struct hostent server_name_copy;
    int server_fd;
    server_name = gethostbyname(address);
    if (server_name == NULL) {
        perror("echo_client: invalid server address\n");
//        exit(1);
        coord[0] = -2;
        coord[1] = -2;
        return coord;
    }

    memcpy(&server_name_copy, server_name, sizeof(server_name_copy));
    memcpy(&server_fd, server_name_copy.h_addr_list[0], sizeof(server_fd));

// send echo_pac_addr to be server address
    echo_pac_addr.sin_family = AF_INET;
    echo_pac_addr.sin_addr.s_addr = server_fd;
    echo_pac_addr.sin_port = htons(port);

    fd_set mask;
    fd_set read_mask;
    int num;

    FD_ZERO(&mask);
    FD_SET(sk, &mask);

// timer for retransmission
    struct timeval timeout;

// record starting time of transfer
    struct timeval start_time;
    struct timeval last_time;

// send init packet to rcv
    sendto(sk, (EchoPacket *) &init_packet, sizeof(init_packet), 0,
           (struct sockaddr *) &echo_pac_addr, sizeof(echo_pac_addr));
// record starting time
    gettimeofday(&start_time, NULL);
    gettimeofday(&last_time, NULL);

    for (;;) {
        read_mask = mask;
        timeout.tv_sec = TIMEOUT_SEC;
        timeout.tv_usec = TIMEOUT_USEC; // 500 ms
        num = select(FD_SETSIZE, &read_mask, NULL, NULL, &timeout);
        if (num > 0) {
            if (FD_ISSET(sk, &read_mask)) {
                client_len = sizeof(client_addr);
                recvfrom(sk, &echo_packet, sizeof(echo_packet), 0,
                         (struct sockaddr *) &client_addr, &client_len);

                if (echo_packet.seq == seq_num) {
                    coord[0] = echo_packet.x;
                    coord[1] = echo_packet.y;

                    return coord; // terminate after report
                } else {
                    coord[0] = -4;
                    coord[1] = -4;
                    return coord;
                }

            }
        } else {
            printf("Haven't heard response for over %d seconds, timeout!\n", TIMEOUT_SEC);
            coord[0] = -1;
            coord[1] = -1;
            return coord;
        }
    }
}

void init_socket(const char *address, int port) {


// socket both for sending and receiving
    sk = socket(AF_INET, SOCK_DGRAM, 0);
    if (sk < 0) {
        perror("echo_client: socket error\n");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

// binding was done in client_bind


    server_name = gethostbyname(address);
    if (server_name == NULL) {
        perror("echo_client: invalid server address\n");
        return;
    }

    memcpy(&server_name_copy, server_name, sizeof(server_name_copy));
    memcpy(&server_fd, server_name_copy.h_addr_list[0], sizeof(server_fd));

    // send echo_pac_addr to be server address
    echo_pac_addr.sin_family = AF_INET;
    echo_pac_addr.sin_addr.s_addr = server_fd;
    echo_pac_addr.sin_port = htons(port);


    FD_ZERO(&mask);
    FD_SET(sk, &mask);
}
