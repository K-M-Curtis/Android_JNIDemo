#include <jni.h>
#include <string>
#include <android/log.h>

// Android打印Log
#define LOG_TAG "Native C++"
#define LOG(FORMAT, ...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,FORMAT,##__VA_ARGS__)

static jclass jClassInteger;
static jmethodID jMtdIntegerInit;

jobject getInteger(JNIEnv *pEnv, jobject pJobject, jint number) {
    // 调用FindClass()获得了java.lang.Integer类引用，然后把它保存在一个全局静态的变量中。然而，在第二次调用中这个引用却无效了（并不是NULL）.
    // 这是因为FindClass()返回的是本地类引用，一旦当getInteger函数返回的时候，这个局部引用就失效了.
    // 需要从局部引用中创建一个全局引用，然后再赋值给全局static变量.
    // 如果是jmethodID和jfieldID的话，不能创建为一个全局引用！因为它们并不是jobject
    if (jClassInteger == nullptr) {
        LOG("In Native, find java.lang.Integer");
        jclass classIntegerLocal = pEnv->FindClass("java/lang/Integer");
        jClassInteger = static_cast<jclass>(pEnv->NewGlobalRef(classIntegerLocal));
        pEnv->DeleteLocalRef(classIntegerLocal);
    }
    if (jClassInteger == nullptr)
        return nullptr;

    if (jMtdIntegerInit == nullptr) {
        LOG("In Native, getMethodID for java.lang.Integer's constructor");
        jMtdIntegerInit = pEnv->GetMethodID(jClassInteger, "<init>", "(I)V");
    }
    if (jMtdIntegerInit == nullptr)
        return nullptr;
    jobject newObj = pEnv->NewObject(jClassInteger, jMtdIntegerInit, number);
    LOG("In Native, consturcted java.lang.Integer with number %d", number);
    return newObj;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_jnidemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jdouble JNICALL
Java_com_example_jnidemo_Test_average(JNIEnv *env, jobject thiz, jint n1, jint n2) {
    jdouble result;
    LOG("In Native, the number are: %d  %d", n1, n2);
    result = ((jdouble) n1 + n2) / 2.0;
    return result;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_jnidemo_Test_printSomething(JNIEnv *env, jobject thiz, jstring msg) {
    const char *inStr = env->GetStringUTFChars(msg, nullptr);
    if (inStr == nullptr)
        return nullptr;
    LOG("In Native, the received string is: %s", inStr);
    env->ReleaseStringUTFChars(msg, inStr);

    char outBuf[128];
    int len = env->GetStringLength(msg);
    env->GetStringUTFRegion(msg, 0, len, outBuf);
    LOG("In Native, the received msg is: %s", outBuf);

    std::string outStr = "I am from Native...";
    return env->NewStringUTF(outStr.c_str());
}

extern "C"
JNIEXPORT jdoubleArray JNICALL
Java_com_example_jnidemo_Test_sumAndAverage(JNIEnv *env, jobject thiz, jintArray numbers) {
    jint *inArray = env->GetIntArrayElements(numbers, nullptr);
    if (inArray == nullptr)
        return nullptr;
    jsize length = env->GetArrayLength(numbers);

    jint sum = 0;
    for (int i = 0; i < length; i++) {
        sum += inArray[i];
    }

    jdouble average = (jdouble) sum / length;
    env->ReleaseIntArrayElements(numbers, inArray, 0);

    jdouble outArray[] = {(jdouble) sum, average};
    jdoubleArray outJniArray = env->NewDoubleArray(2);
    if (outJniArray == nullptr)
        return nullptr;
    env->SetDoubleArrayRegion(outJniArray, 0, 2, outArray);
    return outJniArray;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnidemo_Test_modifyInstanceVariable(JNIEnv *env, jobject thiz) {
    jclass testClass = env->GetObjectClass(thiz);
    jfieldID fieldNumber = env->GetFieldID(testClass, "number", "I");
    if (fieldNumber == nullptr)
        return;

    jint number = env->GetIntField(thiz, fieldNumber);
    LOG("In Native, the int is %d", number);

    number = 99;
    env->SetIntField(thiz, fieldNumber, number);

    jfieldID fieldMessage = env->GetFieldID(testClass, "message", "Ljava/lang/String;");
    if (fieldMessage == nullptr)
        return;
    jstring message = static_cast<jstring>(env->GetObjectField(thiz, fieldMessage));
    const char *cStr = env->GetStringUTFChars(message, nullptr);
    if (cStr == nullptr)
        return;
    LOG("In Native, the string is %s", cStr);
    env->ReleaseStringUTFChars(message, cStr);

    message = env->NewStringUTF("Hello from Native C++...");
    if (message == nullptr)
        return;
    env->SetObjectField(thiz, fieldMessage, message);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnidemo_Test_modifyStaticVariable(JNIEnv *env, jobject thiz) {
    jclass tClass = env->GetObjectClass(thiz);
    jfieldID fNumber = env->GetStaticFieldID(tClass, "num", "D");
    if (fNumber == nullptr)
        return;
    jdouble number = env->GetStaticDoubleField(tClass, fNumber);
    LOG("In Native, the double is %f", number);
    number = 77.88;
    env->SetStaticDoubleField(tClass, fNumber, number);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnidemo_Test_nativeMethod(JNIEnv *env, jobject thiz) {
    jclass thisClass = env->GetObjectClass(thiz);
    jmethodID mtdCallback = env->GetMethodID(thisClass, "callback", "()V");
    if (mtdCallback == nullptr)
        return;
    LOG("In Native, call back java's callback()");
    env->CallVoidMethod(thiz, mtdCallback);

    jmethodID mtdCallbackStr = env->GetMethodID(thisClass, "callback", "(Ljava/lang/String;)V");
    if (mtdCallbackStr == nullptr)
        return;
    LOG("In Native, call back java's callback(String)");
    jstring message = env->NewStringUTF("Method Callback Hello from C++...");
    env->CallVoidMethod(thiz, mtdCallbackStr, message);

    jmethodID mtdCallbackAverage = env->GetMethodID(thisClass, "callbackAverage", "(II)D");
    if (mtdCallbackAverage == nullptr)
        return;
    jdouble average = env->CallDoubleMethod(thiz, mtdCallbackAverage, 2, 7);
    LOG("In Native, the average is %f", average);

    jmethodID mtdCallbackStatic = env->GetStaticMethodID(thisClass, "callbackStatic",
                                                         "()Ljava/lang/String;");
    if (mtdCallbackStatic == nullptr)
        return;
    jstring resultJniStr = static_cast<jstring>(env->CallStaticObjectMethod(thisClass,
                                                                            mtdCallbackStatic));
    const char *resultStr = env->GetStringUTFChars(resultJniStr, nullptr);
    if (resultStr == nullptr)
        return;
    LOG("In Native, the returned string is %s", resultStr);
    env->ReleaseStringUTFChars(resultJniStr, resultStr);
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_example_jnidemo_Test_getInterObject(JNIEnv *env, jobject thiz, jint number) {
    jclass testClass = env->FindClass("java/lang/Integer");
    jmethodID mtdInit = env->GetMethodID(testClass, "<init>", "(I)V");
    if (mtdInit == nullptr)
        return nullptr;

    jobject newObj = env->NewObject(testClass, mtdInit, number);
    jmethodID mtdToString = env->GetMethodID(testClass, "toString", "()Ljava/lang/String;");
    if (mtdToString == nullptr)
        return nullptr;
    jstring resultStr = static_cast<jstring>(env->CallObjectMethod(newObj, mtdToString));
    const char *resultCStr = env->GetStringUTFChars(resultStr, nullptr);
    LOG("In Native, the number is %s", resultCStr);
    return newObj;
}

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_example_jnidemo_Test_callbackSumAndAverage(JNIEnv *env, jobject thiz,
                                                    jobjectArray numbers) {
    jclass classInteger = env->FindClass("java/lang/Integer");
    jmethodID mtdIntValue = env->GetMethodID(classInteger, "intValue", "()I");
    if (classInteger == nullptr)
        return nullptr;

    jsize length = env->GetArrayLength(numbers);
    jint sum = 0;
    for (int i = 0; i < length; i++) {
        jobject objInteger = env->GetObjectArrayElement(numbers, i);
        if (objInteger == nullptr)
            return nullptr;
        jint value = env->CallIntMethod(objInteger, mtdIntValue);
        sum += value;
    }
    double average = (double) sum / length;
    LOG("In Native, the sum is %d", sum);
    LOG("In Native, the average is %f", average);

    jclass classDouble = env->FindClass("java/lang/Double");
    jobjectArray outJniArray = env->NewObjectArray(2, classDouble, nullptr);
    jmethodID mtdDoubleInit = env->GetMethodID(classDouble, "<init>", "(D)V");
    if (mtdDoubleInit == nullptr)
        return nullptr;
    jobject objSum = env->NewObject(classDouble, mtdDoubleInit, (double) sum);
    jobject objAverage = env->NewObject(classDouble, mtdDoubleInit, average);
    env->SetObjectArrayElement(outJniArray, 0, objSum);
    env->SetObjectArrayElement(outJniArray, 1, objAverage);
    return outJniArray;
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_example_jnidemo_Test_getIntegerObject(JNIEnv *env, jobject thiz, jint number) {
    return getInteger(env, thiz, number);
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_example_jnidemo_Test_anotherGetIntegerObject(JNIEnv *env, jobject thiz, jint number) {
    return getInteger(env, thiz, number);
}

extern "C"
JNIEXPORT jintArray JNICALL
Java_com_example_jnidemo_Test_generateIntArray(JNIEnv *env, jobject thiz, jint number) {
    jintArray jarr = env->NewIntArray(number);
    int *arr = env->GetIntArrayElements(jarr, nullptr);
    for (int i = 0; i < number; i++) {
        arr[i] = i;
    }
    env->ReleaseIntArrayElements(jarr, arr, 0);
    return jarr;
}

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_example_jnidemo_Test_generateStringArray(JNIEnv *env, jobject thiz, jint number) {
    jclass jStringClazz = env->FindClass("java/lang/String");
    jstring inStr = env->NewStringUTF("测试");
    jobjectArray jarr = env->NewObjectArray(number, jStringClazz, inStr);
    for (int i = 0; i < number; i++) {
        char str[5];
        sprintf(str, "%s", &"haha"[i]);
        jstring jStr = env->NewStringUTF(str);
        env->SetObjectArrayElement(jarr, i, jStr);
    }
    return jarr;
}
