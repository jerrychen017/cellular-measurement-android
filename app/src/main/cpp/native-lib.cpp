#include <jni.h>
#include <string>

extern "C" {
#include "old/echo_client.h"
#include "bandwidth_measurement/data_generator.h"
#include "bandwidth_measurement/controller.h"
#include "interactive/interactive_client.h"
#include "logger.h"
}

extern "C" char * stdout_buffer;

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_udp_1tools_MainActivity_interarrivalFromJNI(
        JNIEnv *env,
        jobject /* this */,
        jstring ip,
        jint port) {
    // convert jstring ip address to string
    jboolean isCopy;
    std::string address_c = env->GetStringUTFChars(ip, &isCopy);
    // convert jint to int
    int port_c = (int) port;
//    double msec = echo_client_start(port_c, address_c.c_str(), false);
    char * out = client_send(address_c.c_str(), port_c);
//    std::string output = "RTT is " + std::to_string(msec) + " ms";
    return env->NewStringUTF(std::string(out).c_str());
}

extern "C" JNIEXPORT jint JNICALL
Java_com_example_udp_1tools_MainActivity_bandwidthFromJNI(
        JNIEnv *env,
        jobject /* this */,
        jstring ip,
        jint port) {
//    std::string address =  "128.220.221.21";
    // convert jstring ip address to string
    start_logger("controller"); // starting logger
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
    char * out = echo_client(address_c.c_str(), port_c, seq_c);

    return env->NewStringUTF(std::string(out).c_str());
}

extern "C" JNIEXPORT jint JNICALL
Java_com_example_udp_1tools_InteractiveView_echoFromJNI(
        JNIEnv *env,
        jobject /* this */,
        jstring ip,
        jint port,
        jint seq) {
//    std::string address =  "128.220.221.21";
    // convert jstring ip address to string
    jboolean isCopy;
    std::string address_c = env->GetStringUTFChars(ip, &isCopy);
    // convert jint to int
    int port_c = (int) port;
    int seq_c = (int) seq;
    int result = draw_client(address_c.c_str(), port_c, seq_c);

    return result;
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
extern "C" JNIEXPORT jfloatArray JNICALL
Java_com_example_udp_1tools_InteractiveView_receiveInteractivePacket(
        JNIEnv *env,
        jobject /* this */) {
    jfloatArray result;
    result = env->NewFloatArray(3);
    if (result == NULL) {
        return NULL;
    }
    float * coord = receive_interactive_packet();
    env->SetFloatArrayRegion(result, 0, 3, coord);
    free(coord);
    return result;
}

extern "C" JNIEXPORT jfloatArray JNICALL
Java_com_example_udp_1tools_InteractiveView_sendAndReceiveInteractivePacket(
        JNIEnv *env,
        jobject /* this */,
        int sequence_num,
        float x,
        float y) {
    jfloatArray result;
    result = env->NewFloatArray(2);
    if (result == NULL) {
        return NULL;
    }
    float * coord = send_and_receive_interactive_packet(sequence_num, x, y);
//    printf("float coord x is %f", coord[0]);
    env->SetFloatArrayRegion(result, 0, 2, coord);
    free(coord);
    return result;
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_udp_1tools_InteractiveView_initSocket(
        JNIEnv *env,
        jobject /* this */,
        jstring address,
        jint port) {
    // convert jstring ip address to string
    jboolean isCopy;
    std::string address_c = env->GetStringUTFChars(address, &isCopy);
    // convert jint to int
    int port_c = (int) port;
    init_socket(address_c.c_str(), port_c);
    return;
}
