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
    jmethodID methodId = fbEnv->GetMethodID(cls, "feedbackMessage", "()V");
    fbEnv->CallVoidMethod(fbActivity, methodId);
}