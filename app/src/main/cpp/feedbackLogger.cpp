#include "cellular-measurement/bidirectional/feedbackLogger.h"
#include "setupFeedback.h"
#include <stdio.h>

static JNIEnv *upEnv;
static jobject upActivity;
static JNIEnv *downEnv;
static jobject downActivity;

/**
 * Setup JNIEnv for sending upload rate to Android
 */
void setupFeedbackUpload(JNIEnv *env, jobject activity)
{
    printf("setupFeedbackUpload called\n");

    upEnv = env;
    upActivity = activity;
}

/**
 * Setup JNIEnv for sending download rate to Android
 */
void setupFeedbackDownload(JNIEnv *env, jobject activity)
{
    printf("setupFeedbackDownload called\n");

    downEnv = env;
    downActivity = activity;
}

/**
 * Send upload rate to the Android
 */
void sendFeedbackUpload(double d)
{
    jclass cls = upEnv->GetObjectClass(upActivity);
    jmethodID methodId = upEnv->GetMethodID(cls, "sendFeedbackUpload", "(D)V");
    upEnv->CallVoidMethod(upActivity, methodId, d);
}

/**
 * Send download rate to the Android
 */
void sendFeedbackDownload(double d)
{
    jclass cls = downEnv->GetObjectClass(downActivity);
    jmethodID methodId = downEnv->GetMethodID(cls, "sendFeedbackDownload", "(D)V");
    downEnv->CallVoidMethod(downActivity, methodId, d);
}

/**
 * Dummy definition
 */
void sendFeedbackLatency(double d)
{
}

void clear_file_pointers() {

}