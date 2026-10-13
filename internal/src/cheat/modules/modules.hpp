#pragma once
#include <cheat/modules/misc/sprint/sprint.h>

class c_modules
{
public:
    void load( );
    void run( );
    void shutdown( );
};

extern c_modules* modules;