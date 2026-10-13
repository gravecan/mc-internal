#include <pch.h>

bool c_classloader::load( )
{
    JNIEnv* env = c_jvm::get_env( );

    if ( !env )
        return false;

    c_jni_ref<jclass> thread_cls( env->FindClass( "java/lang/Thread" ) );

    if ( !thread_cls )
        return false;

    jmethodID get_all = env->GetStaticMethodID( thread_cls.get( ) , "getAllStackTraces" , "()Ljava/util/Map;" );
    jmethodID get_name = env->GetMethodID( thread_cls.get( ) , "getName" , "()Ljava/lang/String;" );
    jmethodID get_ctx = env->GetMethodID( thread_cls.get( ) , "getContextClassLoader" , "()Ljava/lang/ClassLoader;" );

    c_jni_ref<jobject> map( env->CallStaticObjectMethod( thread_cls.get( ) , get_all ) );

    if ( !map )
        return false;

    c_jni_ref<jclass> map_cls( env->GetObjectClass( map.get( ) ) );
    jmethodID key_set = env->GetMethodID( map_cls.get( ) , "keySet" , "()Ljava/util/Set;" );

    c_jni_ref<jobject> set( env->CallObjectMethod( map.get( ) , key_set ) );

    c_jni_ref<jclass> set_cls( env->GetObjectClass( set.get( ) ) );
    jmethodID to_array = env->GetMethodID( set_cls.get( ) , "toArray" , "()[Ljava/lang/Object;" );

    c_jni_ref<jobjectArray> threads( static_cast<jobjectArray>( env->CallObjectMethod( set.get( ) , to_array ) ) );

    if ( !threads )
        return false;

    jsize count = env->GetArrayLength( threads.get( ) );

    for ( jsize i = 0; i < count; i++ )
    {
        c_jni_ref<jobject> thread( env->GetObjectArrayElement( threads.get( ) , i ) );
        c_jni_ref<jstring> name( static_cast<jstring>( env->CallObjectMethod( thread.get( ) , get_name ) ) );

        if ( !name )
            continue;

        const char* utf = env->GetStringUTFChars( name.get( ) , nullptr );
        bool match = utf && strcmp( utf , "Render thread" ) == 0;

        if ( utf )
            env->ReleaseStringUTFChars( name.get( ) , utf );

        if ( !match )
            continue;

        c_jni_ref<jobject> loader( env->CallObjectMethod( thread.get( ) , get_ctx ) );

        if ( !loader )
            return false;

        class_loader = c_jni_ref<jobject>( loader.get( ) , ref_mode::global );

        return class_loader.get( ) != nullptr;
    }

    return false;
}

c_jni_ref<jclass> c_classloader::find_class( const char* name )
{
    JNIEnv* env = c_jvm::get_env( );

    if ( !env || !class_loader.get( ) )
        return c_jni_ref<jclass>( );

    if ( !m_load_class )
    {
        c_jni_ref<jclass> cl_cls( env->FindClass( "java/lang/ClassLoader" ) );

        m_load_class = env->GetMethodID( cl_cls.get( ) , "loadClass" , "(Ljava/lang/String;)Ljava/lang/Class;" );
    }

    c_jni_ref<jstring> jname( env->NewStringUTF( name ) );

    jclass cls = static_cast<jclass>( env->CallObjectMethod( class_loader.get( ) , m_load_class , jname.get( ) ) );

    if ( env->ExceptionCheck( ) )
    {
        env->ExceptionClear( );
        return c_jni_ref<jclass>( );
    }

    return c_jni_ref<jclass>( cls );
}

c_classloader* classloader = new c_classloader( );