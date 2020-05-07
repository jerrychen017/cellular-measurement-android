#include <jni.h>
#include <string>

#include "setupFeedback.h"
#include "cellular-measurement/bidirectional/net_include.h"

extern "C" {
#include "cellular-measurement/interactive_client/echo_client.h"
#include "client_android.h"
#include "cellular-measurement/bidirectional/data_generator.h"
#include "cellular-measurement/bidirectional/controller.h"
#include "cellular-measurement/bidirectional/receive_bandwidth.h"
#include "cellular-measurement/bidirectional/send_bandwidth.h"
#include "cellular-measurement/bidirectional/net_utils.h"
#include "cellular-measurement/interactive_client/interactive_client.h"
#include "cellular-measurement/interactive_client/interactive_net_include.h"
#include "logger.h"
}

extern "C" char * stdout_buffer;

/**
 * Useful debugging functions:
 * setupFeedback(env, activity);
 * start_logger("controller"); // starting logger
 */

/**
 *  Get parameters from JNI and construct a struct parameter
 */
struct parameters get_parameters(JNIEnv *env, jobject paramsObj) {
    jclass paramsClass = env->FindClass("com/example/udp_tools/Parameters");;
    struct parameters params;
    jmethodID getBurstSize = env->GetMethodID(paramsClass, "getBurstSize", "()I");
    jmethodID getIntervalSize = env->GetMethodID(paramsClass, "getIntervalSize", "()I");
    jmethodID getIntervalTime = env->GetMethodID(paramsClass, "getIntervalTime", "()D");
    jmethodID getInstantBurst = env->GetMethodID(paramsClass, "getInstantBurst", "()I");
    jmethodID getMinSpeed = env->GetMethodID(paramsClass, "getMinSpeed", "()D");
    jmethodID getMaxSpeed = env->GetMethodID(paramsClass, "getMaxSpeed", "()D");
    jmethodID getStartSpeed = env->GetMethodID(paramsClass, "getStartSpeed", "()D");
    jmethodID getGracePeriod = env->GetMethodID(paramsClass, "getGracePeriod", "()I");
    jmethodID getPredMode = env->GetMethodID(paramsClass, "getPredMode", "()I");
    jmethodID getUseTCP = env->GetMethodID(paramsClass, "getUseTCP", "()I");
    jmethodID getAlpha = env->GetMethodID(paramsClass, "getAlpha", "()D");
    jmethodID getThreshold = env->GetMethodID(paramsClass, "getThreshold", "()D");

    params.burst_size = env->CallIntMethod(paramsObj, getBurstSize);
    params.interval_size = env->CallIntMethod(paramsObj, getIntervalSize);
    params.interval_time = env->CallDoubleMethod(paramsObj, getIntervalTime);
    params.instant_burst = env->CallIntMethod(paramsObj, getInstantBurst);
    params.min_speed = env->CallDoubleMethod(paramsObj, getMinSpeed);
    params.max_speed = env->CallDoubleMethod(paramsObj, getMaxSpeed);
    params.start_speed = env->CallDoubleMethod(paramsObj, getStartSpeed);
    params.grace_period = env->CallIntMethod(paramsObj, getGracePeriod);
    params.pred_mode = env->CallIntMethod(paramsObj, getPredMode);
    params.use_tcp = env->CallIntMethod(paramsObj, getUseTCP);
    params.alpha = env->CallDoubleMethod(paramsObj, getAlpha);
    params.threshold = env->CallDoubleMethod(paramsObj, getThreshold);

    return params;
}


/**
 * start android client and complete handshake/ack process
 */
extern "C" JNIEXPORT jint JNICALL
Java_com_example_udp_1tools_MainActivity_startClientAndroidFromJNI(
        JNIEnv *env,
        jobject activity,
        jstring ip,
        jobject paramsObj) {
    jboolean isCopy;
    std::string address_c = env->GetStringUTFChars(ip, &isCopy);
    start_logger("client_android");

    struct parameters params = get_parameters(env, paramsObj);

    int status = start_client(address_c.c_str(), params);
    return status;
}

/**
 * Starts controller
 */
extern "C" JNIEXPORT void JNICALL
Java_com_example_udp_1tools_MainActivity_startControllerFromJNI(
        JNIEnv *env,
        jobject activity,
        jstring ip,
        jobject paramsObj) {
    jboolean isCopy;
    std::string ip_cpp = env->GetStringUTFChars(ip, &isCopy);
    setupFeedbackUpload(env, activity);

    struct parameters params = get_parameters(env, paramsObj);

    android_start_controller(ip_cpp.c_str(), params);
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
        jstring ip,
        jobject paramsObj) {
    jboolean isCopy;
    std::string ip_cpp = env->GetStringUTFChars(ip, &isCopy);
    setupFeedbackDownload(env, activity);

    struct parameters params = get_parameters(env, paramsObj);

    android_receive_bandwidth(ip_cpp.c_str(), params);
}

/**
 * Kills data generator thread
 */
extern "C" JNIEXPORT void JNICALL
Java_com_example_udp_1tools_MainActivity_stopDataGeneratorThreadFromJNI(
        JNIEnv *env,
        jobject activity /* this */) {
    stop_data_generator_thread();
}

/**
 * Kills controller thread
 */
extern "C" JNIEXPORT void JNICALL
Java_com_example_udp_1tools_MainActivity_stopControllerThreadFromJNI(
        JNIEnv *env,
        jobject activity /* this */) {
    stop_controller_thread();
}

/**
 * Kills send bandwidth thread running on TCP
 */
extern "C" JNIEXPORT void JNICALL
Java_com_example_udp_1tools_MainActivity_stopTCPSendThreadFromJNI(
        JNIEnv *env,
        jobject activity /* this */) {
    stop_tcp_send_thread();
}

/**
 * Kills receive bandwidth thread running on TCP
 */
extern "C" JNIEXPORT void JNICALL
Java_com_example_udp_1tools_MainActivity_stopTCPRecvThreadFromJNI(
        JNIEnv *env,
        jobject activity /* this */) {
    stop_tcp_recv_thread();
}

/**
 * Kills receive_bandwidth thread
 */
extern "C" JNIEXPORT void JNICALL
Java_com_example_udp_1tools_MainActivity_stopReceivingThreadFromJNI(
        JNIEnv *env,
        jobject activity  /* this */) {
    stop_receiving_thread();
}

/**
 * Sends a UDP echo to the server and returns RTT or timeout
 */
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_udp_1tools_MainActivity_echoFromJNI(
        JNIEnv *env,
        jobject /* this */,
        jstring ip,
        jint port,
        jint seq) {
    // convert jstring ip address to string
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
 * Receives interactive packet, running on a separate thread
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

/**
 * Connect to interactive server
 */
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
