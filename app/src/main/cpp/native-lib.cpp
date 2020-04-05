#include <jni.h>
#include <string>

extern "C" {
#include "cellular-measurement/echo_client/echo_client.h"
#include "cellular-measurement/bandwidth_measurement/data_generator.h"
#include "cellular-measurement/bandwidth_measurement/controller.h"
#include "cellular-measurement/interactive_server/interactive_client.h"
#include "logger.h"
}

extern "C" char * stdout_buffer;

extern "C" JNIEXPORT jint JNICALL
Java_com_example_udp_1tools_MainActivity_bandwidthFromJNI(
        JNIEnv *env,
        jobject /* this */,
        jstring ip,
        jint port) {
//    std::string address =  "128.220.221.21";
    // convert jstring ip address to string
//    start_logger("controller"); // starting logger
    jboolean isCopy;
    std::string address_c = env->GetStringUTFChars(ip, &isCopy);
    // convert jint to int
    int port_c = (int) port;
//    double msec = echo_client_start(port_c, address_c.c_str(), false);
    int result = start_controller(address_c.c_str(), port_c, true);
//    std::string output = "RTT is " + std::to_string(msec) + " ms";
    return result;
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_udp_1tools_MainActivity_generateDataFromJNI(
        JNIEnv *env,
        jobject /* this */
        ) {
    start_logger("data_generator"); // starting logger
    start_generator(true);
    return;
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
