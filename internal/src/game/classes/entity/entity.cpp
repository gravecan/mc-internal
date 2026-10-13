#include <pch.h>

void c_entity::load( )
{
    JNIEnv* env = c_jvm::get_env( );

    if ( !env || !classes->mc_class )
        return;

    jclass mc = classes->mc_class.get( );
    jclass living = classes->living_class.get( );

    m_is_sprinting = env->GetMethodID( living , "method_5624" , "()Z" ); // is sprinting

    if ( !m_is_sprinting )
    {
        env->ExceptionClear( );
        logger.log<level::info>( "issprinting not found" );
    }

    m_set_sprinting = env->GetMethodID( living , "method_5728" , "(Z)V" ); // set sprinting

    if ( !m_set_sprinting )
    {
        env->ExceptionClear( );
        logger.log<level::info>( "setsprinting not found" );
    }

    m_is_on_ground = env->GetMethodID( living , "method_24828" , "()Z" ); // is on ground

    if ( !m_is_on_ground )
    {
        env->ExceptionClear( );
        logger.log<level::info>( "isonground not found" );
    }

    
}

bool c_entity::is_sprinting( jobject player )
{
    JNIEnv* env = c_jvm::get_env( );

    if ( !env || !player || !m_is_sprinting )
        return false;

    jboolean result = env->CallBooleanMethod( player , m_is_sprinting );

    if ( env->ExceptionCheck( ) )
    {
        env->ExceptionClear( );
        return false;
    }

    return result == JNI_TRUE;
}

bool c_entity::is_on_ground( jobject player )
{
    JNIEnv* env = c_jvm::get_env( );

    if ( !env || !player || !m_is_on_ground )
        return false;

    jboolean result = env->CallBooleanMethod( player , m_is_on_ground );

    if ( env->ExceptionCheck( ) )
    {
        env->ExceptionClear( );
        return false;
    }

    return result == JNI_TRUE;
}

void c_entity::set_sprinting( jobject player , bool state )
{
    JNIEnv* env = c_jvm::get_env( );

    if ( !env || !player || !m_set_sprinting )
        return;

    env->CallVoidMethod( player , m_set_sprinting , state ? JNI_TRUE : JNI_FALSE );

    if ( env->ExceptionCheck( ) )
        env->ExceptionClear( );
}

c_entity* entity = new c_entity( );