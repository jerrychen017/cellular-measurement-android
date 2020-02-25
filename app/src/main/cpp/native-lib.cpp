#include <jni.h>
#include <string>

extern "C" {
#include "echo_client.h"
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_udp_1tools_MainActivity_initSendFromJNI(
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
    double msec = echo_client_start(port_c, address_c.c_str(), false);
    std::string output = "RTT is " + std::to_string(msec) + " ms";
    return env->NewStringUTF(output.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_udp_1tools_MainActivity_resendFromJNI(
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
    double msec = echo_client_start(port_c, address_c.c_str(), true);
    std::string output = "RTT is " + std::to_string(msec) + " ms";
    return env->NewStringUTF(output.c_str());
}