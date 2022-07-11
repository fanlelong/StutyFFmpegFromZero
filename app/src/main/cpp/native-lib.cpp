#include <jni.h>
#include <string>
#include <android/log.h> // NDK里的打印库

#define TAG "JNI_TEST"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

// 先声明  get()
extern "C" {
// 由于库是C写的
#include "libget/getutil.h"
int get();
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_ancely_jni_ffmpeg_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}