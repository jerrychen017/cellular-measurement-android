#include "data_generator.h"
/**
    Data generator
*/
int start_generator()
{

    int s = setup_socket();

    // Select loop stuff
    fd_set mask;
    fd_set read_mask;

    struct timeval timeout;
    int num;

    FD_ZERO(&mask);
    FD_SET(s, &mask);

    int seq = 0;
    bool start = false; // change to true for testing
    typed_packet pkt;
    char buffer[DATA_SIZE];
    double speed = 1.0;

    // Timeout before start is TIMEOUT_SEC
    printf("data_generator: start sending pkts\n");

    for (;;)
    {
        read_mask = mask;

        // timeout limit
        if (!start) {
            timeout.tv_sec = TIMEOUT_SEC;
            timeout.tv_usec = TIMEOUT_USEC;
        }
        else {
            timeout = speed_to_interval(speed); 
        }

        num = select(FD_SETSIZE, &read_mask, NULL, NULL, &timeout);

        if (num > 0) {
            if (FD_ISSET(s, &read_mask))
            {
                int len = recv(s, &pkt, sizeof(typed_packet), 0);
                if (len <= 0) {
                    printf("controller disconnected, exiting\n");
                    exit(0);
                }
                if (pkt.type == LOCAL_START)
                {
                    printf("Starting data stream\n");
                    start = true;
                }
                else if (pkt.type == LOCAL_CONTROL)
                {
                    // adjust speed n
                    printf("received LOCAL_CONTROL message\n");
                    speed = *((double *) pkt.data);
                    if (speed <= 0) {
                        printf("negative speed\n");
//                        exit(1);
                        return 1;
                    }
                    if (speed > MAX_SPEED) {
                        printf("exceed max speed\n");
//                        exit(1);
                        return 1;
                    }
                }
            }
        }
        else {
            //timeout
            if (!start)
            {
                printf("Waiting to start\n");
            }
            else
            {
                send(s, buffer, DATA_SIZE , 0);
//                printf("data_generator: sent data pkt to controller\n");
                seq++;
            }
        }

    }

    return 0;
}


int setup_socket()
{
    int s;
    int len;

    struct sockaddr_un controller;



    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("data_generator: socket error\n");
        exit(1);
    }

    printf("data_generator: Trying to connect...\n");

    memset(&controller, 0, sizeof(controller)); // fix
    controller.sun_family = AF_UNIX;
    // fixing socket error
    const char name[] = "\0my.local.socket.address";
// size-1 because abstract socket names are *not* null terminated
    memcpy(controller.sun_path, name, sizeof(name) - 1);
//    strcpy(controller.sun_path, SOCK_PATH);
//    len = strlen(controller.sun_path) + sizeof(controller.sun_family);
    len = strlen(controller.sun_path) + sizeof(name); // fix
    controller.sun_path[0] = 0;


    if (connect(s, (struct sockaddr *)&controller, len) == -1)
    {
        printf("data_generator: connect error in setup_socket()\n");
//        exit(1);
        return 1;
    }

    printf("data_generator: Connected.\n");
    return s;
}
