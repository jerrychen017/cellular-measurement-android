#include <jni.h>
#include <string>

#include "setupFeedback.h"
#include "cellular-measurement/bidirectional/net_include.h"

extern "C" {
#include "cellular-measurement/interactive_client/echo_client.h"
#include "cellular-measurement/bidirectional/client_android.h"
#include "cellular-measurement/bidirectional/data_generator.h"
#include "cellular-measurement/bidirectional/controller.h"
#include "cellular-measurement/bidirectional/receive_bandwidth.h"
#include "cellular-measurement/bidirectional/net_utils.h"
#include "cellular-measurement/interactive_client/interactive_client.h"
#include "logger.h"
}

extern "C" char * stdout_buffer;

/**
 * Useful debugging functions:
 * setupFeedback(env, activity);
 * start_logger("controller"); // starting logger
 */

extern "C" JNIEXPORT jint JNICALL
Java_com_example_udp_1tools_MainActivity_bandwidthFromJNI(
        JNIEnv *env,
        jobject activity /* this */,
        jstring ip,
        jint port) {
//    std::string address =  "128.220.221.21";
    // convert jstring ip address to string
//    start_logger("controller"); // starting logger
    jboolean isCopy;
    std::string address_c = env->GetStringUTFChars(ip, &isCopy);

//    setupFeedback(env, activity);
    // convert jint to int
    int port_c = (int) port;

//    start_client(address_c.c_str(), 1, true); // TODO: change 1 to the parameter in Config page
//    std::string output = "RTT is " + std::to_string(msec) + " ms";
    return 0;
}

/**
 * start android client and complete handshake/ack process
 */
extern "C" JNIEXPORT void JNICALL
Java_com_example_udp_1tools_MainActivity_startClientAndroidFromJNI(
        JNIEnv *env,
        jobject activity,
        jstring ip,
        jint sk) {
    jboolean isCopy;
    std::string address_c = env->GetStringUTFChars(ip, &isCopy);
    start_logger("client_android");
    start_client(address_c.c_str(), (int) sk);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_udp_1tools_MainActivity_startControllerFromJNI(
        JNIEnv *env,
        jobject activity,
        jstring ip) {
    int sk = setup_bound_socket(CLIENT_SEND_PORT);
    jboolean isCopy;
    std::string ip_cpp = env->GetStringUTFChars(ip, &isCopy);
    struct sockaddr_in send_addr = addrbyname(ip_cpp.c_str(), CLIENT_SEND_PORT);
    start_controller(true, send_addr, (int) sk);
}

/**
 * Start Data Generator
 */
extern "C" JNIEXPORT void JNICALL
Java_com_example_udp_1tools_MainActivity_startDataGeneratorFromJNI(
        JNIEnv *env,
        jobject activity) {
    start_generator(true);
}

/**
 * call receive_bandwidth() in C
 */
extern "C" JNIEXPORT void JNICALL
Java_com_example_udp_1tools_MainActivity_receiveBandwidthFromJNI(
        JNIEnv *env,
        jobject activity,
        jint sk,
        jint pred_mode) {
    receive_bandwidth((int) sk, (int) pred_mode);
}

extern "C" JNIEXPORT jint JNICALL
Java_com_example_udp_1tools_MainActivity_bindRecvBandwidthFromJNI(
        JNIEnv *env,
        jobject activity) {
    setupFeedback(env, activity);
    int sk = setup_bound_socket(CLIENT_RECEIVE_PORT);
    return sk;
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_udp_1tools_MainActivity_stopDataGeneratorThreadFromJNI(
        JNIEnv *env,
        jobject activity /* this */) {
    stop_data_generator_thread();
}
extern "C" JNIEXPORT void JNICALL
Java_com_example_udp_1tools_MainActivity_stopControllerThreadFromJNI(
        JNIEnv *env,
        jobject activity /* this */) {
    stop_controller_thread();
}

extern "C" JNIEXPORT jint JNICALL
Java_com_example_udp_1tools_MainActivity_bindFromJNI(
        JNIEnv *env,
        jobject /* this */,
        jstring ip,
        jint port) {
//    std::string address =  "128.220.221.21";
    // convert jstring ip address to string
    jboolean isCopy;
    std::string address_c = env->GetStringUTFChars(ip, &isCopy);
    // convert jint to int
    int port_c = (int) port;

    int status = client_bind(address_c.c_str(), port_c);

    return status;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_udp_1tools_MainActivity_echoFromJNI(
        JNIEnv *env,
        jobject /* this */,
        jstring ip,
        jint port,
        jint seq) {
//    std::string address =  "128.220.221.21";
    // convert jstring ip address to string
//    start_logger("echo"); // starting logger
    jboolean isCopy;
    std::string address_c = env->GetStringUTFChars(ip, &isCopy);
    // convert jint to int
    int port_c = (int) port;
    int seq_c = (int) seq;
    char * out = echo_send(address_c.c_str(), port_c, seq_c);

    return env->NewStringUTF(std::string(out).c_str());
}

/**
    * sends an interactive packet with coordinate x and y
    * @return sequence number of the packet
    */
extern "C" JNIEXPORT jint JNICALL
Java_com_example_udp_1tools_InteractiveView_sendInteractivePacket(
        JNIEnv *env,
        jobject /* this */,
        int seq_num,
        float x,
        float y) {

    int ret = send_interactive_packet(seq_num, x, y);

    return ret;
}

/**
 *
 * @param sequence_num the sequence number of the packet to be received
 * @return an array of [x_coor, y_coor, sequence_num]
 */
extern "C" JNIEXPORT jobject JNICALL
Java_com_example_udp_1tools_InteractiveView_receiveInteractivePacket(
        JNIEnv *env,
        jobject /* this */) {
    InteractivePacket echoPacket = receive_interactive_packet();
    jobject echo_java_obj;
    jclass interactive_pkt_class;
    jmethodID constructor;
    interactive_pkt_class = env->FindClass("com/example/udp_tools/InteractivePacket");
    constructor = env->GetMethodID(interactive_pkt_class, "<init>", "(IFFIDLjava/lang/String;)V");
    jvalue args[6];
    args[0].i = echoPacket.seq;
    args[1].f = echoPacket.x;
    args[2].f = echoPacket.y;
    args[3].i = echoPacket.id;
    args[4].d = echoPacket.latency;
    args[5].l = env->NewStringUTF(std::string(echoPacket.name).c_str());

    echo_java_obj = env->NewObjectA(interactive_pkt_class, constructor, args);
    return echo_java_obj;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_example_udp_1tools_InteractiveView_initInteractive(
        JNIEnv *env,
        jobject /* this */,
        jstring address,
        jint port,
        jstring name) {
    start_logger("interactive"); // starting logger
    // convert jstring ip address to string
    jboolean isCopy;
    std::string address_c = env->GetStringUTFChars(address, &isCopy);
    // convert jint to int
    int port_c = (int) port;
    init_socket(address_c.c_str(), port_c);

    // send connect message to the server
    std::string name_c = env->GetStringUTFChars(name, &isCopy);
    int id = interactive_connect(name_c.c_str());
    return id;
}
