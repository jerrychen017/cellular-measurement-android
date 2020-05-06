#include "cellular-measurement/bidirectional/feedbackLogger.h"
#include "setupFeedback.h"
#include <stdio.h>

static JNIEnv *upEnv;
static jobject upActivity;
static JNIEnv *downEnv;
static jobject downActivity;

void setupFeedbackUpload(JNIEnv *env, jobject activity)
{
    printf("setupFeedbackUpload called\n");

    upEnv = env;
    upActivity = activity;
}

void setupFeedbackDownload(JNIEnv *env, jobject activity)
{
    printf("setupFeedbackDownload called\n");

    downEnv = env;
    downActivity = activity;
}

void sendFeedbackUpload(double d)
{
    jclass cls = upEnv->GetObjectClass(upActivity);
    jmethodID methodId = upEnv->GetMethodID(cls, "sendFeedbackUpload", "(D)V");
    upEnv->CallVoidMethod(upActivity, methodId, d);
}

void sendFeedbackDownload(double d)
{
    jclass cls = downEnv->GetObjectClass(downActivity);
    jmethodID methodId = downEnv->GetMethodID(cls, "sendFeedbackDownload", "(D)V");
    downEnv->CallVoidMethod(downActivity, methodId, d);
}

void sendFeedbackLatency(double d)
{
}

void clear_file_pointers() {

}