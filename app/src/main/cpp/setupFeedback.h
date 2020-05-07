#ifndef UDP_TOOLS_SETUPFEEDBACK_H
#define UDP_TOOLS_SETUPFEEDBACK_H

#include <jni.h>
#include <string>

/**
 * Setup JNIEnv for sending upload rate to Android
 */
void setupFeedbackUpload(JNIEnv *env, jobject activity);

/**
 * Setup JNIEnv for sending download rate to Android
 */
void setupFeedbackDownload(JNIEnv *env, jobject activity);

#endif //UDP_TOOLS_SETUPFEEDBACK_H
