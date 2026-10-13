#include <pch.h>

void c_classes::load( )
{
    mc_class = c_jni_ref<jclass>( classloader->find_class( "net.minecraft.class_310" ).get( ) , ref_mode::global );
    level_class = c_jni_ref<jclass>( classloader->find_class( "org.apache.logging.log4j.Level" ).get( ) , ref_mode::global );
    config_class = c_jni_ref<jclass>( classloader->find_class( "org.apache.logging.log4j.core.config.Configurator" ).get( ) , ref_mode::global );
    living_class = c_jni_ref<jclass>( classloader->find_class( "net.minecraft.class_1309" ).get( ) , ref_mode::global );
    text_class = c_jni_ref<jclass>( classloader->find_class( "net.minecraft.class_2561" ).get( ) , ref_mode::global );
    world_class = c_jni_ref<jclass>( classloader->find_class( "net.minecraft.class_638" ).get( ) , ref_mode::global );
    player_class = c_jni_ref<jclass>( classloader->find_class( "net.minecraft.class_1657" ).get( ) , ref_mode::global );
    rtc_class = c_jni_ref<jclass>( classloader->find_class( "net.minecraft.class_9779$class_9781" ).get( ) , ref_mode::global );

    minecraft->load( );
    log4j->load( );
	entity->load( );
	world->load( );

	log4j->disable( );
	classes->log( );


    uhhthisisfordebugging_thread = std::thread( [ ] ( )
        {
            while ( classes->uhhthisisfordebugging )
            {
                for ( int i = 0; i < 10 && classes->uhhthisisfordebugging; i++ )
                    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

                if ( !classes->uhhthisisfordebugging )
                    break;

                Sleep( 2500 );

                logger.log<level::info>( "screen -> %s" , minecraft->get_current_screen( ) ? "true" : "false" );
            }

            if ( c_jvm::vm )
                c_jvm::vm->DetachCurrentThread( );
        } );
}

void c_classes::log( )
{
    if ( mc_class )
        logger.log<level::info>( "mc class found -> %p" , static_cast<void*>( mc_class.get( ) ) );
    else
        logger.log<level::info>( "couldnt find mc class" );

    c_jni_ref<jobject> world = minecraft->get_world( );
    c_jni_ref<jobject> player = minecraft->get_player( );
    std::string version = minecraft->get_version( );
    jint fps = minecraft->get_fps( );

    if ( !version.empty( ) )
        logger.log<level::info>( "version -> %s" , version.c_str( ) );
    else
        logger.log<level::info>( "couldnt find version" );

    if ( fps > 0 )
    {
        logger.log<level::info>( "fps -> %d" , static_cast<int>( minecraft->get_fps( ) ) );

        /*
		while (fps > 0)
		{
			std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
			fps = minecraft->get_fps( );
			logger.log<level::info>( "fps -> %d" , static_cast<int>( fps ) );
		}
        */
    }
    else
    {
        logger.log<level::info>( "couldnt find fps" );
    }

    if ( world )
    {
        logger.log<level::info>( "world found -> %p" , static_cast<void*>( world.get( ) ) );
	}
	else
	{
		logger.log<level::info>( "couldnt find world" );
		//world.get( );
    }

    if ( player )
        logger.log<level::info>( "player found -> %p" , static_cast<void*>( player.get( ) ) );
    else
        logger.log<level::info>( "player is null" );

}

void c_classes::shutdown( )
{
    uhhthisisfordebugging = false;

    if (uhhthisisfordebugging_thread.joinable( ) )
        uhhthisisfordebugging_thread.join( );

    mc_class = c_jni_ref<jclass>( );
    level_class = c_jni_ref<jclass>( );
    config_class = c_jni_ref<jclass>( );
    living_class = c_jni_ref<jclass>( );
    text_class = c_jni_ref<jclass>( );
    world_class = c_jni_ref<jclass>( );
    player_class = c_jni_ref<jclass>( );
	rtc_class = c_jni_ref<jclass>( );
}

c_classes* classes = new c_classes( );