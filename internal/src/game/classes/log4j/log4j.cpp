#include <pch.h>

void c_log4j::load( )
{
    JNIEnv* env = c_jvm::get_env( );

    if (!env || !classes->level_class || !classes->config_class)
        return;

    f_off = env->GetStaticFieldID( classes->level_class.get( ) , "OFF" , "Lorg/apache/logging/log4j/Level;" ); // off 

    if ( !f_off )
    {
        env->ExceptionClear( );
        logger.log<level::info>( "off not found" );
    }

    m_set_root = env->GetStaticMethodID( classes->config_class.get( ) , "setRootLevel" , "(Lorg/apache/logging/log4j/Level;)V" ); // sroot

    if ( !m_set_root )
    {
        env->ExceptionClear( );
        logger.log<level::info>( "setrootlevel not found" );
    }
}

void c_log4j::disable( )
{
    JNIEnv* env = c_jvm::get_env( );

    if ( !env )
    {
        logger.log<level::info>( "log4j disable -> env null" );
        return;
    }

    if ( !f_off || !m_set_root )
    {
        logger.log<level::info>( "log4j disable -> ids null -> f_off %p , m_set_root %p" , static_cast<void*>( f_off ) , static_cast<void*>( m_set_root ) );
        return;
    }

    c_jni_ref<jobject> off( env->GetStaticObjectField( classes->level_class.get( ) , f_off ) );

    if ( !off )
    {
        logger.log<level::info>( "level.off is null" );
        return;
    }

    env->CallStaticVoidMethod( classes->config_class.get( ) , m_set_root , off.get( ) );

    if ( env->ExceptionCheck( ) )
    {
        env->ExceptionClear( );
        logger.log<level::info>( "log4j threw an exception" );
        return;
    }

    logger.log<level::info>( "brutally mogged log4j" );
}

c_log4j* log4j = new c_log4j( );