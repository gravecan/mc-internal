#include <pch.h>

void c_minecraft::load( )
{
    JNIEnv* env = c_jvm::get_env( );

    if ( !env || !classes->mc_class )
        return;

    jclass mc = classes->mc_class.get( );
    jclass living = classes->living_class.get( );

    f_world = env->GetFieldID( mc , "field_1687" , "Lnet/minecraft/class_638;" ); // world

    if ( !f_world )
    {
        env->ExceptionClear( );
        logger.log<level::info>( "world not found" );
    }

    f_player = env->GetFieldID( mc , "field_1724" , "Lnet/minecraft/class_746;" ); // player

    if ( !f_player )
    {
        env->ExceptionClear( );
        logger.log<level::info>( "player not found" );
    }

    f_version = env->GetFieldID( mc , "field_1711" , "Ljava/lang/String;" ); // version

    if ( !f_version )
    {
        env->ExceptionClear( );
        logger.log<level::info>( "version not found" );
    }

    f_currentscreen = env->GetFieldID( mc , "field_1755" , "Lnet/minecraft/class_437;" ); // current screen

    if ( !f_currentscreen )
    {
        env->ExceptionClear( );
        logger.log<level::info>( "current screen not found" );
    }

    f_fps = env->GetStaticFieldID( mc , "field_1738" , "I" ); // fps

    if ( !f_fps )
    {
        env->ExceptionClear( );
        logger.log<level::info>( "fps not found" );
    }

    m_get_instance = env->GetStaticMethodID( mc , "method_1551" , "()Lnet/minecraft/class_310;" ); // get instance

    if ( !m_get_instance )
    {
        env->ExceptionClear( );
        logger.log<level::info>( "getins not found" );
    }

    if ( !classes->living_class )
        return;

    f_crosshair = env->GetFieldID( mc , "field_1765" , "Lnet/minecraft/class_239;" );

    if ( !f_crosshair )
    {
        env->ExceptionClear( );
        logger.log<level::info>( "crosshair not found" );
    }

    f_manager = env->GetFieldID( mc , "field_1761" , "Lnet/minecraft/class_636;" );

    if ( !f_manager )
    {
        env->ExceptionClear( );
        logger.log<level::info>( "interaction manager not found" );
    }

    f_rtc = env->GetFieldID( mc , "field_52750" , "Lnet/minecraft/class_9779$class_9781;" );

    if ( !f_rtc )
    {
        env->ExceptionClear( );
        logger.log<level::info>( "tick counter field not found" );
    }

    if ( classes->rtc_class )
    {
        m_tick_progress = env->GetMethodID( classes->rtc_class.get( ) , "method_60637" , "(Z)F" );

        if ( !m_tick_progress )
        {
            env->ExceptionClear( );
            logger.log<level::info>( "tick progress not found" );
        }
    }
    else
    {
        logger.log<level::info>( "tick counter class not found" );
    }

}

c_jni_ref<jobject> c_minecraft::get_instance( )
{
    JNIEnv* env = c_jvm::get_env( );

    if ( !env || !classes->mc_class || !m_get_instance )
        return c_jni_ref<jobject>( );

    return c_jni_ref<jobject>( env->CallStaticObjectMethod( classes->mc_class.get( ) , m_get_instance ) );
}

c_jni_ref<jobject> c_minecraft::get_world( )
{
    JNIEnv* env = c_jvm::get_env( );

    if ( !env )
    {
        logger.log<level::info>( "env null" );
        return c_jni_ref<jobject>( );
    }

    if ( !f_world || !m_get_instance )
    {
        logger.log<level::info>( "ids null -> f_world %p , m_get_instance %p" , static_cast<void*>( f_world ) , static_cast<void*>( m_get_instance ) );
        return c_jni_ref<jobject>( );
    }

    c_jni_ref<jobject> instance = get_instance( );

    if ( !instance )
    {
        logger.log<level::info>( "instance null" );
        return c_jni_ref<jobject>( );
    }

    jobject world = env->GetObjectField( instance.get( ) , f_world );

    if ( !world )
        logger.log<level::info>( "world field is null" );

    return c_jni_ref<jobject>( world );
}

c_jni_ref<jobject> c_minecraft::get_player( )
{
    JNIEnv* env = c_jvm::get_env( );

    if ( !env )
    {
        logger.log<level::info>( "env null" );
        return c_jni_ref<jobject>( );
    }

    if ( !f_player || !m_get_instance )
    {
        logger.log<level::info>( "ids null -> f_player %p , m_get_instance %p" , static_cast<void*>( f_player ) , static_cast<void*>( m_get_instance ) );
        return c_jni_ref<jobject>( );
    }

    c_jni_ref<jobject> instance = get_instance( );

    if ( !instance )
    {
        logger.log<level::info>( "instance null" );
        return c_jni_ref<jobject>( );
    }

    jobject player = env->GetObjectField( instance.get( ) , f_player );

    if ( !player )
        logger.log<level::info>( "player field is null" );

    return c_jni_ref<jobject>( player );
}

std::string c_minecraft::get_version( )
{
    JNIEnv* env = c_jvm::get_env( );

    if ( !env || !f_version || !m_get_instance )
        return std::string( );

    c_jni_ref<jobject> instance = get_instance( );

    if ( !instance )
        return std::string( );

    c_jni_ref<jstring> str( static_cast<jstring>( env->GetObjectField( instance.get( ) , f_version ) ) );

    if ( !str )
        return std::string( );

    const char* utf = env->GetStringUTFChars( str.get( ) , nullptr );

    if ( !utf )
        return std::string( );

    std::string result( utf );

    env->ReleaseStringUTFChars( str.get( ) , utf );

    return result;
}

bool c_minecraft::get_current_screen( )
{
    JNIEnv* env = c_jvm::get_env( );

    if ( !env || !f_currentscreen )
        return false;

    c_jni_ref<jobject> instance = get_instance( );

    if ( !instance )
        return false;

    c_jni_ref<jobject> screen( env->GetObjectField( instance.get( ) , f_currentscreen ) );

    return static_cast<bool>( screen );
}

jint c_minecraft::get_fps( )
{
    JNIEnv* env = c_jvm::get_env( );

    if ( !env || !classes->mc_class || !f_fps )
        return 0;

    return env->GetStaticIntField( classes->mc_class.get( ) , f_fps );
}

c_jni_ref<jobject> c_minecraft::get_crosshair( )
{
    JNIEnv* env = c_jvm::get_env( );

    if ( !env || !f_crosshair )
        return c_jni_ref<jobject>( );

    c_jni_ref<jobject> instance = get_instance( );

    if ( !instance )
        return c_jni_ref<jobject>( );

    return c_jni_ref<jobject>( env->GetObjectField( instance.get( ) , f_crosshair ) );
}

c_jni_ref<jobject> c_minecraft::get_manager( )
{
    JNIEnv* env = c_jvm::get_env( );

    if ( !env || !f_manager )
        return c_jni_ref<jobject>( );

    c_jni_ref<jobject> instance = get_instance( );

    if ( !instance )
        return c_jni_ref<jobject>( );

    return c_jni_ref<jobject>( env->GetObjectField( instance.get( ) , f_manager ) );
}

float c_minecraft::get_tick_delta( )
{
    JNIEnv* env = c_jvm::get_env( );

    if ( !env || !f_rtc || !m_tick_progress )
        return -1.f;

    c_jni_ref<jobject> instance = get_instance( );

    if ( !instance )
        return -1.f;

    c_jni_ref<jobject> counter( env->GetObjectField( instance.get( ) , f_rtc ) );

    if ( env->ExceptionCheck( ) )
    {
        env->ExceptionClear( );
        return -1.f;
    }

    if ( !counter )
        return -1.f;

    jfloat value = env->CallFloatMethod( counter.get( ) , m_tick_progress , JNI_FALSE );

    if ( env->ExceptionCheck( ) )
    {
        env->ExceptionClear( );
        return -1.f;
    }

    if ( value < 0.f || value > 1.f )
        return -1.f;

    return value;
}

void c_minecraft::update_tick( )
{
    m_new_tick = false;

    float delta = get_tick_delta( );

    if ( delta < 0.f )
    {
        unsigned long long ticks = GetTickCount64( ) / 50;

        if ( ticks != m_ticks )
        {
            m_ticks = ticks;
            m_new_tick = true;
        }

        return;
    }

    if ( delta < m_last_delta )
    {
        m_ticks++;
        m_new_tick = true;
    }

    m_last_delta = delta;
}

bool c_minecraft::is_new_tick( )
{
    return m_new_tick;
}

unsigned long long c_minecraft::get_ticks( )
{
    return m_ticks;
}

c_minecraft* minecraft = new c_minecraft( );