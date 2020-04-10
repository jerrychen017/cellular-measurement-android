#include "cellular-measurement/bandwidth_measurement/feedbackLogger.h"
#include "setupFeedback.h"
#include <stdio.h>

JNIEnv *fbEnv;
jobject fbActivity;

void setupFeedback(JNIEnv *env, jobject activity)
{
    printf("setupfeedback called\n");

    fbEnv = env;
    fbActivity = activity;
}

void sendFeedbackMessage(char* str)
{
    jclass cls = fbEnv->GetObjectClass(fbActivity);
    jstring java_str = fbEnv->NewStringUTF(std::string(str).c_str());
    jmethodID methodId = fbEnv->GetMethodID(cls, "feedbackMessage", "(Ljava/lang/String;)V");
    fbEnv->CallVoidMethod(fbActivity, methodId, java_str);
    fbEnv->DeleteLocalRef(java_str);
}