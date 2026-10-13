#include <pch.h>

void c_world::load( )
{
    JNIEnv* env = c_jvm::get_env( );

    if ( !env || !classes->mc_class || !classes->world_class || !classes->living_class || !classes->text_class )
        return;

    jclass world = classes->world_class.get( );
    jclass living = classes->living_class.get( );

    m_get_players = env->GetMethodID( world , "method_18456" , "()Ljava/util/List;" ); // get players

    if ( !m_get_players )
    {
        env->ExceptionClear( );
        logger.log<level::info>( "world getPlayers not found" );
    }

    c_jni_ref<jclass> list_cls( env->FindClass( "java/util/List" ) ); // list x_x

    if ( list_cls )
    {
        m_list_size = env->GetMethodID( list_cls.get( ) , "size" , "()I" );
        m_list_get = env->GetMethodID( list_cls.get( ) , "get" , "(I)Ljava/lang/Object;" );

        if ( !m_list_size || !m_list_get )
        {
            env->ExceptionClear( );
            logger.log<level::info>( "list methods not found" );
        }
    }
    else
    {
        env->ExceptionClear( );
        logger.log<level::info>( "list class not found" );
    }



    m_get_name = env->GetMethodID( living , "method_5477" , "()Lnet/minecraft/class_2561;" ); // get name

    if ( !m_get_name )
    {
        env->ExceptionClear( );
        logger.log<level::info>( "entity getName not found" );
    }

    m_get_string = env->GetMethodID( classes->text_class.get( ) , "getString" , "()Ljava/lang/String;" ); // get sring

    if ( !m_get_string )
    {
        env->ExceptionClear( );
        logger.log<level::info>( "text getString not found" );
    }
}

std::vector<c_jni_ref<jobject>> c_world::get_players( )
{
    std::vector<c_jni_ref<jobject>> result;

    JNIEnv* env = c_jvm::get_env( );

    if ( !env || !m_get_players || !m_list_size || !m_list_get )
        return result;

    c_jni_ref<jobject> level = minecraft->get_world( );

    if ( !level )
        return result;

    c_jni_ref<jobject> list( env->CallObjectMethod( level.get( ) , m_get_players ) );

    if ( env->ExceptionCheck( ) )
    {
        env->ExceptionClear( );
        return result;
    }

    if ( !list )
        return result;

    jint size = env->CallIntMethod( list.get( ) , m_list_size );

    if ( env->ExceptionCheck( ) )
    {
        env->ExceptionClear( );
        return result;
    }

    result.reserve( static_cast<size_t>( size ) );

    for ( jint i = 0; i < size; i++ )
    {
        c_jni_ref<jobject> player( env->CallObjectMethod( list.get( ) , m_list_get , i ) );

        if ( env->ExceptionCheck( ) )
        {
            env->ExceptionClear( );
            break;
        }

        if ( player )
            result.push_back( std::move( player ) );
    }

    return result;
}

std::string c_world::get_player_name( jobject player )
{
    JNIEnv* env = c_jvm::get_env( );

    if ( !env || !player || !m_get_name || !m_get_string )
        return std::string( );

    c_jni_ref<jobject> text( env->CallObjectMethod( player , m_get_name ) );

    if ( env->ExceptionCheck( ) )
    {
        env->ExceptionClear( );
        return std::string( );
    }

    if ( !text )
        return std::string( );

    c_jni_ref<jstring> str( static_cast<jstring>( env->CallObjectMethod( text.get( ) , m_get_string ) ) );

    if ( env->ExceptionCheck( ) )
    {
        env->ExceptionClear( );
        return std::string( );
    }

    if ( !str )
        return std::string( );

    const char* utf = env->GetStringUTFChars( str.get( ) , nullptr );

    if ( !utf )
        return std::string( );

    std::string name( utf );
    env->ReleaseStringUTFChars( str.get( ) , utf );

    return name;
}

c_world* world = new c_world( );