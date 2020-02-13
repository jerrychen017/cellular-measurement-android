#include <jni.h>
#include <string>

extern "C" {
#include "echo_client.h"
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_udp_1tools_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string address =  "54.193.124.120";
    double msec = echo_client_start(9008, address.c_str());
    std::string output = "RTT is " + std::to_string(msec) + " ms";
    return env->NewStringUTF(output.c_str());
}
