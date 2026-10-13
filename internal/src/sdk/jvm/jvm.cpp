#include <pch.h>

void c_jvm::load( )
{
    JNI_GetCreatedJavaVMs( &vm , 1 , nullptr );

    if ( !vm )
        return;

    env = get_env( );
}

JNIEnv* c_jvm::get_env( )
{
    if ( !vm )
        return nullptr;

    if ( env )
        return env;

    JNIEnv* e = nullptr;

    jint result = vm->GetEnv( reinterpret_cast<void**>( &e ) , JNI_VERSION_1_6 );

    if ( result == JNI_EDETACHED )
    {
        if ( vm->AttachCurrentThreadAsDaemon( reinterpret_cast<void**>( &e ) , nullptr ) != JNI_OK )
            return nullptr;
    }
    else if ( result != JNI_OK )
    {
        return nullptr;
    }

    env = e;

    return env;
}

c_jvm* jvm = new c_jvm( );