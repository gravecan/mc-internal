#pragma once

class c_minecraft
{
public:
    void load( );

    c_jni_ref<jobject> get_instance( );
    c_jni_ref<jobject> get_world( );
    c_jni_ref<jobject> get_player( );
    c_jni_ref<jobject> get_crosshair( );
    c_jni_ref<jobject> get_manager( );
    std::string get_version( );
    bool get_current_screen( );
    jint get_fps( );
    float get_tick_delta( );
    void update_tick( );
    bool is_new_tick( );
    unsigned long long get_ticks( );

private:
    jfieldID f_world = nullptr;
    jfieldID f_player = nullptr;
    jfieldID f_crosshair = nullptr;
    jfieldID f_manager = nullptr;
    jfieldID f_version = nullptr;
    jfieldID f_currentscreen = nullptr;
    jfieldID f_fps = nullptr;
    jmethodID m_get_instance = nullptr;
    jfieldID f_rtc = nullptr;
    jmethodID m_tick_progress = nullptr;
    float m_last_delta = 1.f;
    bool m_new_tick = false;
    unsigned long long m_ticks = 0;
};

extern c_minecraft* minecraft;