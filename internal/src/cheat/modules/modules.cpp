#include <pch.h>

void c_modules::load( )
{
    logger.log<level::info>( "modules initialized" );
}
    
void c_modules::run( )
{
    minecraft->update_tick( );

    sprint->run( );
}

void c_modules::shutdown( )
{
    sprint->shutdown( );
}

c_modules* modules = new c_modules( );