QT += quick
QT += core
QT += core-private
QT += network

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        connectiontoplayer.cpp \
        detectball.cpp \
        gameinfo.cpp \
        main.cpp \
        manageplayer.cpp \
        receiverdata.cpp \
        sendersignals.cpp \
        socket.cpp \
        videocamcapture.cpp

RESOURCES += qml.qrc


OPENCV_ANDROID = C:\Users\misha\Documents\PingPong_Mobile\OpenCV-android-sdk

android {
    contains(ANDROID_TARGET_ARCH,arm64-v8a) {
        isEmpty(OPENCV_ANDROID) {
            error("Let OPENCV_ANDROID point to the opencv-android-sdk, recommended: v4.0")
        }
        INCLUDEPATH += "$$OPENCV_ANDROID/sdk/native/jni/include"
        LIBS += \
            -L"$$OPENCV_ANDROID/sdk/native/libs/arm64-v8a" \
            -L"$$OPENCV_ANDROID/sdk/native/3rdparty/libs/arm64-v8a" \
            -llibtiff \
            -llibjpeg-turbo \
            -llibpng \
            -lIlmImf \
            -ltegra_hal \
            -lade \
            -lcpufeatures \
            -lopencv_java4 \

            ANDROID_EXTRA_LIBS = $$OPENCV_ANDROID/sdk/native/libs/arm64-v8a/libopencv_java4.so
    } else {
        error("Unsupported architecture: $$ANDROID_TARGET_ARCH")
    }

    #include(C:\Users\misha\AppData\Local\Android\Sdk\android_openssl\openssl.pri)

    #ANDROID_EXTRA_LIBS += \
        #C:\Users\misha\AppData\Local\Android\Sdk\android_openssl\ssl_3\arm64-v8a\libcrypto_3.so \
        #C:\Users\misha\AppData\Local\Android\Sdk\android_openssl\ssl_3\arm64-v8a\libssl_3.so
}



HEADERS += \
    connectiontoplayer.h \
    detectball.h \
    gameinfo.h \
    manageplayer.h \
    receiverdata.h \
    sendersignals.h \
    socket.h \
    videocamcapture.h

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
OTHER_FILES += \
    android/src/org/qtproject/example/pingpongmobile/WiFiApi.java \
    android/AndroidManifest.xml
android: include(C:/Users/misha/AppData/Local/Android/Sdk/android_openssl/openssl.pri)
