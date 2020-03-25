#include "echo_client.h"

static TimingPacket timing, *recvTiming;
static ReportPacket report, *recvReport;
static Packet pack, *packet = &pack;


/* Sends NUM_SEND ~MTU packets
 *
 */

void send_timing_packets(int s, struct sockaddr_in send_addr)
{
    int seq = 0;
    for (int i = 0; i < NUM_SEND; i++) {
        timing.type = 0;
        timing.seq = seq;
        sendto(s, &timing, sizeof(timing), 0,
               (struct sockaddr*)&send_addr, sizeof(send_addr));
        seq++;
    }
}

/* Sends a stream of ~MTU (1400 bytes) packets to echo server
 * to test latency, after 5 seconds in order to ignore lost packets.
 *
 * delay is ms between packets, n is total num. Timeouts
 * s is the socekt, send_addr is the address of echo server
 *
 * Should report delay of each and avg delay
 */
char * compute_bandwidth(int s, struct sockaddr_in send_addr, int delay)
{
    char * report = malloc(sizeof(char) * NUM_SEND * 200);
    fd_set mask;
    fd_set read_mask;
    int num;

    FD_ZERO(&mask);
    FD_SET(s, &mask);

    // Compute interarrival time on client side as well
    //Using Monotonically increasing clock
    struct timespec ts_curr;
    struct timespec ts_prev;

    struct timespec ts_start;
    struct timespec ts_end;

    struct timeval timeout;

    double avg_interarrival = 0;
    int count = 0;
    int next_num = 0;
    int dropped = 0;
    double min_time = DBL_MAX;
    double max_time = 0;

    for (;;) {
        read_mask = mask;
        //Double timeout limit
        timeout.tv_sec = TIMEOUT_SEC*2;
        timeout.tv_usec = TIMEOUT_USEC;

        num = select(FD_SETSIZE, &read_mask, NULL, NULL, &timeout);

        if (num > 0) {
            if (FD_ISSET(s, &read_mask)) {
                recv(s, &pack, sizeof(Packet), 0);
                if(packet->type == 0){
                    recvTiming = (TimingPacket *) packet;
                }
                else{
                    recvReport = (ReportPacket *) packet;
                }
                //interarrival computation
                if(next_num == 0){
                    clock_gettime(CLOCK_MONOTONIC, &ts_start);
                }
                if(next_num == NUM_SEND - 1){
                    clock_gettime(CLOCK_MONOTONIC, &ts_end);
                }
                clock_gettime(CLOCK_MONOTONIC, &ts_curr);
                if (next_num != recvTiming->seq){
                    sprintf(report + strlen(report), "UNEXPECTED SEQUENCE NUMBER, EXITING...%d %d\n", next_num, recvTiming->seq);
                    dropped += recvTiming->seq - next_num;
                    next_num = recvTiming->seq;
                }
                if (next_num != 0) {
                    // Trying and formatting new timer
                    struct timespec ts_diff;
                    ts_diff.tv_sec = ts_curr.tv_sec - ts_prev.tv_sec;
                    ts_diff.tv_nsec = ts_curr.tv_nsec - ts_prev.tv_nsec;
                    while (ts_diff.tv_nsec > 1000000000) {
                        ts_diff.tv_sec++;
                        ts_diff.tv_nsec -= 1000000000;
                    }
                    while (ts_diff.tv_nsec < 0) {
                        ts_diff.tv_sec--;
                        ts_diff.tv_nsec += 1000000000;
                    }

                    if (ts_diff.tv_sec < 0) {
                        sprintf(report + strlen(report), "NEGATIVE TIME\n", 1);
//                        exit(1);
                        return report;
                    }
                    double msec = ts_diff.tv_sec * 1000 + ((double) ts_diff.tv_nsec) / 1000000;
                    if(msec < min_time){
                        min_time = msec;
                    }
                    if(msec > max_time){
                        max_time = msec;
                    }
                    // Skip first few packets due to start up costs
                    if(next_num > 0){
                        avg_interarrival += msec;
                        count += 1;
                    }
                    sprintf(report + strlen(report), "%.5f ms interarrival time\n", msec);
                }
                next_num += 1;
                ts_prev = ts_curr;
            }
        } else {
            if(count > 0){
                sprintf(report + strlen(report), "Dropped packets: %d\n", dropped);
                sprintf(report + strlen(report), "Average Interarrival time %.5f ms\n", avg_interarrival/count);
                float avg = avg_interarrival/count;
                float throughput = ((1400.0*8)/(1024*1024))/(avg/1000);
                sprintf(report + strlen(report), "Computed Throughput %f Mbps\n", throughput);

                //Compute with start -> end

                // Trying and formatting new timer
                struct timespec ts_diff;
                int size = sizeof(Packet);
                ts_diff.tv_sec = ts_end.tv_sec - ts_start.tv_sec;
                ts_diff.tv_nsec = ts_end.tv_nsec - ts_start.tv_nsec;
                while (ts_diff.tv_nsec > 1000000000) {
                    ts_diff.tv_sec++;
                    ts_diff.tv_nsec -= 1000000000;
                }
                while (ts_diff.tv_nsec < 0) {
                    ts_diff.tv_sec--;
                    ts_diff.tv_nsec += 1000000000;
                }

                if (ts_diff.tv_sec < 0) {
                    sprintf(report + strlen(report), "NEGATIVE TIME\n", 1);
//                    exit(1);
                    return report;
                }
                double s = ts_diff.tv_sec + ((double) ts_diff.tv_nsec) / 1000000000;
                sprintf(report + strlen(report), "Coarse bandwidth calculation %f Mbps\n", (size*8*(NUM_SEND-1)/1024.0/1024)/s);


            }
            // printf("Haven't heard response for over %d seconds, timeout!\n", TIMEOUT_SEC*2);
            fflush(0);
            avg_interarrival = 0;
            count = 0;
            next_num = 0;
            dropped = 0;
            min_time = DBL_MAX;
            max_time = 0;
            return report;
        }
    }

}

int client_bind(const char* address, int port) {

    // server socket address
    struct sockaddr_in server_addr;

    // socket both for sending and receiving
    int sk = socket(AF_INET, SOCK_DGRAM, 0);
    if (sk < 0) {
        printf("echo_client: socket error\n");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // bind socket with port
    if (bind(sk, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("echo_client: bind error\n"); // prints a warning but don't exit.
        // we can still send since it's already binded
        return 1;
    }
    return 0;
}

char * client_send(const char* address, int port) {
    char * out = malloc(sizeof(char) * 1500);

    // initialize starting packet and echo_packet
    char init_packet[BUFF_SIZE];
    char echo_packet[BUFF_SIZE];

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
        sprintf(out, "echo_client: socket error with sk==%d\n", sk);
        return out;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // binding was done in client_bind

    // get server host address
    struct hostent* server_name;
    struct hostent server_name_copy;
    int server_fd;
    server_name = gethostbyname(address);
    if (server_name == NULL) {
        sprintf(out, "echo_client: invalid server address with address %s\n", server_name);
        return out;
    }

    memcpy(&server_name_copy, server_name, sizeof(server_name_copy));
    memcpy(&server_fd, server_name_copy.h_addr_list[0], sizeof(server_fd));

    // send echo_pac_addr to be server address
    echo_pac_addr.sin_family = AF_INET;
    echo_pac_addr.sin_addr.s_addr = server_fd;
    echo_pac_addr.sin_port = htons(port);

    send_timing_packets(sk, echo_pac_addr);

    free(out);
    char * report = compute_bandwidth(sk, echo_pac_addr, 100);
    return report;
}

char * echo_client(const char* address, int port, int sequence) {
    char * out = malloc(sizeof(char) * 300);
// initialize starting packet and echo_packet
EchoPacket init_packet, echo_packet;
init_packet.type = ECHO;
init_packet.seq = sequence;

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
    exit(1);
}

server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = INADDR_ANY;
server_addr.sin_port = htons(port);

// binding was done in client_bind

// get server host address
struct hostent* server_name;
struct hostent server_name_copy;
int server_fd;
server_name = gethostbyname(address);
if (server_name == NULL) {
    perror("echo_client: invalid server address\n");
    exit(1);
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
sendto(sk, (EchoPacket *)&init_packet, sizeof(init_packet), 0,
(struct sockaddr*)&echo_pac_addr, sizeof(echo_pac_addr));
// record starting time
gettimeofday(&start_time, NULL);
gettimeofday(&last_time, NULL);

for (;;) {
     read_mask = mask;
     timeout.tv_sec = TIMEOUT_SEC;
     timeout.tv_usec = TIMEOUT_USEC;
     num = select(FD_SETSIZE, &read_mask, NULL, NULL, &timeout);
    if (num > 0) {
        if (FD_ISSET(sk, &read_mask)) {
            client_len = sizeof(client_addr);
            recvfrom(sk, &echo_packet, sizeof(echo_packet), 0,
                                 (struct sockaddr*)&client_addr, &client_len);
            int server_ip = client_addr.sin_addr.s_addr;

            // record current time and report
            struct timeval current_time;
            gettimeofday(&current_time, NULL);
            struct timeval diff_time = diffTime(current_time, last_time);
            double msec = diff_time.tv_sec * 1000 + ((double) diff_time.tv_usec) / 1000;
            sprintf(out, "Report: RTT of a UDP packet is %f ms with sequence number %d\n", msec, echo_packet.seq);
            last_time = current_time;

            return out; // terminate after report
        }
    } else {
        sprintf(out, "Haven't heard response for over %d seconds, timeout!\n", TIMEOUT_SEC);
        return out;
    }
}
return 0;
}

int draw_client(const char* address, int port, int sequence) {
    char * out = malloc(sizeof(char) * 300);
// initialize starting packet and echo_packet
    EchoPacket init_packet, echo_packet;
    init_packet.type = ECHO;
    init_packet.seq = sequence;

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
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

// binding was done in client_bind

// get server host address
    struct hostent* server_name;
    struct hostent server_name_copy;
    int server_fd;
    server_name = gethostbyname(address);
    if (server_name == NULL) {
        perror("echo_client: invalid server address\n");
        exit(1);
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
    sendto(sk, (EchoPacket *)&init_packet, sizeof(init_packet), 0,
           (struct sockaddr*)&echo_pac_addr, sizeof(echo_pac_addr));
// record starting time
    gettimeofday(&start_time, NULL);
    gettimeofday(&last_time, NULL);

    for (;;) {
        read_mask = mask;
        timeout.tv_sec = 0;
        timeout.tv_usec = 500000; // 500 ms
        num = select(FD_SETSIZE, &read_mask, NULL, NULL, &timeout);
        if (num > 0) {
            if (FD_ISSET(sk, &read_mask)) {
                client_len = sizeof(client_addr);
                recvfrom(sk, &echo_packet, sizeof(echo_packet), 0,
                         (struct sockaddr*)&client_addr, &client_len);
                int server_ip = client_addr.sin_addr.s_addr;

                // record current time and report
                struct timeval current_time;
                gettimeofday(&current_time, NULL);
                struct timeval diff_time = diffTime(current_time, last_time);
                double msec = diff_time.tv_sec * 1000 + ((double) diff_time.tv_usec) / 1000;
                sprintf(out, "Report: RTT of a UDP packet is %f ms with sequence number %d\n", msec, echo_packet.seq);
                last_time = current_time;
                free(out);
                return 0; // terminate after report
            }
        } else {
            sprintf(out, "Haven't heard response for over %d seconds, timeout!\n", TIMEOUT_SEC);
            free(out);
            return 1;
        }
    }
    return 0;
}
