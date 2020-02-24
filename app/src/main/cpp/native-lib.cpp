#include <jni.h>
#include <string>

extern "C" {
#include "echo_client.h"
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_udp_1tools_MainActivity_initSendFromJNI(
        JNIEnv *env,
        jobject /* this */) {
//    std::string address =  "54.193.124.120";
    std::string address = "128.220.221.21";
    double msec = echo_client_start(4579, address.c_str(), false);
    std::string output = "RTT is " + std::to_string(msec) + " ms";
    return env->NewStringUTF(output.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_udp_1tools_MainActivity_resendFromJNI(
        JNIEnv *env,
        jobject /* this */) {
//    std::string address =  "54.193.124.120";
    std::string address = "128.220.221.21";
    double msec = echo_client_start(4579, address.c_str(), true);
    std::string output = "RTT is " + std::to_string(msec) + " ms";
    return env->NewStringUTF(output.c_str());
}