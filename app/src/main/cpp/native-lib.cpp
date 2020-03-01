#include <jni.h>
#include <string>

extern "C" {
#include "echo_client.h"
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_udp_1tools_MainActivity_interarrivalFromJNI(
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
//    double msec = echo_client_start(port_c, address_c.c_str(), false);
    char * out = client_send(address_c.c_str(), port_c);
//    std::string output = "RTT is " + std::to_string(msec) + " ms";
    return env->NewStringUTF(std::string(out).c_str());
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
        jint port) {
//    std::string address =  "128.220.221.21";
    // convert jstring ip address to string
    jboolean isCopy;
    std::string address_c = env->GetStringUTFChars(ip, &isCopy);
    // convert jint to int
    int port_c = (int) port;
    double msec = echo_client(address_c.c_str(), port_c);
    std::string output = "RTT is " + std::to_string(msec) + " ms";

    return env->NewStringUTF(output.c_str());
}