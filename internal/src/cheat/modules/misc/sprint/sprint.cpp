#include <pch.h>
#include <cheat/modules/misc/sprint/sprint.h>

void c_sprint::run( )
{
    if ( !enabled )
        return;

    if ( minecraft->get_current_screen( ) )
        return;

    if ( key > 0 && ( GetAsyncKeyState( key ) & 1 ) )
    {
        enabled = !enabled;
        logger.log<level::info>( "sprint toggled -> %d" , enabled );
    }

    if ( !enabled )
        return;

    if ( !( GetAsyncKeyState( 'W' ) & 0x8000 ) )
        return;

    c_jni_ref<jobject> player = minecraft->get_player( );

    if ( !player )
        return;

    if ( entity->is_sprinting( player.get( ) ) )
        return;

    if ( !entity->is_on_ground( player.get( ) ) )
        return;

    logger.log<level::info>( "sprint loop" );

    entity->set_sprinting( player.get( ) , true );
}

void c_sprint::shutdown( )
{
    enabled = false;
    logger.log<level::info>( "sprint shutdown" );
}


c_sprint* sprint = new c_sprint( );