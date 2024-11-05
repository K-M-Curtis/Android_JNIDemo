package com.example.jnidemo;

import android.util.Log;

public class Test {
    static {
        System.loadLibrary("jnidemo");
    }

    private static final String TAG = "======Test";

    //1. 在Java和Native代码之间传递参数和返回值
    // 基本类型传递
    private native double average(int n1, int n2);

    // 传递字符串
    private native String printSomething(String msg);

    private native int[] generateIntArray(int number);

    private native String[] generateStringArray(int number);

    // 传递基本类型的数组
    private native double[] sumAndAverage(int[] numbers);

    // 访问Java对象实例的变量
    private int number = 88;
    private String message = "Hello from Java...";

    private native void modifyInstanceVariable();

    // 访问类中的static变量
    private static double num = 55.66;

    private native void modifyStaticVariable();

    // 回调实例的普通和static方法
    private native void nativeMethod();

    private void callback() {
        Log.d(TAG, "In Java...");
        Log.d(TAG, "-----------------------------------------------");
    }

    private void callback(String message) {
        Log.d(TAG, "In Java with " + message);
        Log.d(TAG, "-----------------------------------------------");
    }

    private double callbackAverage(int n1, int n2) {
        return ((double) n1 + n2) / 2.0;
    }

    private static String callbackStatic() {
        return "From static Java Method...";
    }

    // 回调Java构造器来创建一个新的java对象
    // 回调一个构造器和回调其他的方法是类似的，首先通过init作为方法名，V作为返回值来获得Method ID，然后通过NewObject()函数来构建一个java类对象。
    private native Integer getInterObject(int number);

    // 回调Java对象（object）的array
    private native Double[] callbackSumAndAverage(Integer[] numbers);

    // 本地和全局引用
    private native Integer getIntegerObject(int number);

    private native Integer anotherGetIntegerObject(int number);

    // Test Driver
    public static void printMessage() {
        // 基本类型传递
        Log.d(TAG, "printMessage: In Java, the average is: " + new Test().average(3, 2));
        Log.d(TAG, "-----------------------------------------------");
        // 传递字符串
        String result = new Test().printSomething("I am from Java...");
        Log.d(TAG, "printMessage: In Java, the returned String is: " + result);
        Log.d(TAG, "-----------------------------------------------");
        // 传递基本类型的数组
        int[] numbers = {22, 33, 33};
        double[] results = new Test().sumAndAverage(numbers);
        Log.d(TAG, "printMessage: In Java, the sum is: " + results[0]);
        Log.d(TAG, "printMessage: In Java, the average is: " + results[1]);
        Log.d(TAG, "-----------------------------------------------");
        // 访问Java对象实例的变量
        Test test = new Test();
        test.modifyInstanceVariable();
        Log.d(TAG, "printMessage: In Java, the int is: " + test.number);
        Log.d(TAG, "printMessage: In Java, the string is: " + test.message);
        Log.d(TAG, "-----------------------------------------------");
        // 访问类中的static变量
        test.modifyStaticVariable();
        Log.d(TAG, "printMessage: In Java, the double is: " + num);
        Log.d(TAG, "-----------------------------------------------");
        test.nativeMethod();
        // 回调Java构造器来创建一个新的java对象
        Log.d(TAG, "printMessage: In Java, the number is: " + test.getInterObject(9999));
        Log.d(TAG, "-----------------------------------------------");
        // 回调Java对象（object）的array
        Integer[] nums = {11, 22, 33};
        Double[] res = new Test().callbackSumAndAverage(nums);
        Log.d(TAG, "printMessage: In Java, the callback sum is: " + res[0]);
        Log.d(TAG, "printMessage: In Java, the callback average is: " + res[1]);
        Log.d(TAG, "-----------------------------------------------");
        // 本地和全局引用
        Log.d(TAG, "printMessage: In Java, the num is: " + test.getIntegerObject(1));
        Log.d(TAG, "printMessage: In Java, the num is: " + test.getIntegerObject(2));
        Log.d(TAG, "printMessage: In Java, the num is: " + test.anotherGetIntegerObject(11));
        Log.d(TAG, "printMessage: In Java, the num is: " + test.anotherGetIntegerObject(22));
        Log.d(TAG, "printMessage: In Java, the num is: " + test.getIntegerObject(3));
        Log.d(TAG, "printMessage: In Java, the num is: " + test.anotherGetIntegerObject(13));
        Log.d(TAG, "-----------------------------------------------");
        int[] arr = test.generateIntArray(3);
        for (int i : arr) {
            Log.d(TAG, "printMessage: In Java, the Int array is: " + i);
        }
        Log.d(TAG, "-----------------------------------------------");
        String[] strRes = test.generateStringArray(3);
        for (String value : strRes) {
            Log.d(TAG, "printMessage: In Java, the String array is: " + value);
        }
        Log.d(TAG, "-----------------------------------------------");
    }

}
