#pragma once
#include <sdk/jvm/jvm.hpp>

class c_classloader
{
public:
    bool load( );

    c_jni_ref<jclass> find_class( const char* name );

    inline static c_jni_ref<jobject> class_loader;

private:
    inline static jmethodID m_load_class = nullptr;
};

extern c_classloader* classloader;