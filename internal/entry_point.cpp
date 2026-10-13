#include <pch.h>

DWORD WINAPI MainThread( LPVOID lpParam )
{
	AllocConsole( );
	FILE* f = nullptr;
	freopen_s( &f , "CONOUT$" , "w" , stdout );

    core->load( );

    while ( !( GetAsyncKeyState( VK_END ) & 1 ) )
    {
        modules->run( );
    }
    
	core->shutdown( );

    if ( f )
        fclose( f );

    FreeConsole( );

    FreeLibraryAndExitThread( static_cast<HMODULE>( lpParam ) , 0 );

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD ul_reason_for_call,
                       LPVOID lpReserved )
{
    if ( ul_reason_for_call == DLL_PROCESS_ATTACH )
    {
        DisableThreadLibraryCalls( hModule );

        CreateThread
        (
            nullptr , 0 , MainThread , hModule , 0 , nullptr
        );
    }

    return TRUE;
}