#include <pch.h>
#include <cheat/modules/misc/sprint/sprint.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND , UINT , WPARAM , LPARAM );

void c_wglswapbuffers::load( )
{
    HMODULE gl = GetModuleHandleA( "opengl32.dll" );

    if ( !gl )
    {
        logger.log<level::info>( "opengl32 not found" );
        return;
    }

    m_target = reinterpret_cast<void*>( GetProcAddress( gl , "wglSwapBuffers" ) );

    if ( !m_target )
    {
        logger.log<level::info>( "wglSwapBuffers not found" );
        return;
    }

    if ( MH_Initialize( ) != MH_OK )
    {
        logger.log<level::info>( "minhook init failed" );
        return;
    }

    if ( MH_CreateHook( m_target , reinterpret_cast<void*>( &swap_hook ) , reinterpret_cast<void**>( &m_original_swap ) ) != MH_OK )
    {
        logger.log<level::info>( "minhook create failed" );
        return;
    }

    if ( MH_EnableHook( m_target ) != MH_OK )
    {
        logger.log<level::info>( "minhook enable failed" );
        return;
    }

    logger.log<level::info>( "minhook -> %p" , m_target );
}

void c_wglswapbuffers::shutdown( )
{
    if ( !m_target )
        return;

    m_unload = true;

    for ( int i = 0; i < 100 && m_initialized && !m_cleaned; i++ )
        Sleep( 10 );

    MH_DisableHook( m_target );
    MH_RemoveHook( m_target );
    MH_Uninitialize( );
}

void c_wglswapbuffers::init( HDC hdc )
{
    m_window = WindowFromDC( hdc );

    if ( !m_window )
        return;

    ImGui::CreateContext( );
    ImGui::StyleColorsDark( );
    ImGui::GetIO( ).IniFilename = nullptr;

    ImGui_ImplWin32_Init( m_window );
    ImGui_ImplOpenGL3_Init( "#version 150" );

    m_original_wndproc = reinterpret_cast<WNDPROC>( SetWindowLongPtrW( m_window , GWLP_WNDPROC , reinterpret_cast<LONG_PTR>( wndproc_hook ) ) );

    m_initialized = true;

    logger.log<level::info>( "wglswapbuffers initialized" );
}

void c_wglswapbuffers::cleanup( )
{
    SetWindowLongPtrW( m_window , GWLP_WNDPROC , reinterpret_cast<LONG_PTR>( m_original_wndproc ) );

    ImGui_ImplOpenGL3_Shutdown( );
    ImGui_ImplWin32_Shutdown( );
    ImGui::DestroyContext( );

    m_initialized = false;
    m_cleaned = true;
}

void c_wglswapbuffers::draw( )
{
    ImGui_ImplOpenGL3_NewFrame( );
    ImGui_ImplWin32_NewFrame( );
    ImGui::NewFrame( );

    if ( m_menu_open )
    {
        ImGui::SetNextWindowSize( ImVec2( 240.f , 0.f ) , ImGuiCond_FirstUseEver );

        if ( ImGui::Begin( "Modules" ) )
        {
            bool sprint_enabled = sprint->enabled;

            if ( ImGui::Checkbox( "Sprint" , &sprint_enabled ) )
                sprint->enabled = sprint_enabled;
        }
        ImGui::End( );
    }

    ImGui::EndFrame( );
    ImGui::Render( );
    ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData( ) );
}

BOOL WINAPI c_wglswapbuffers::swap_hook( HDC hdc )
{
    if ( m_unload )
    {
        if ( m_initialized )
            wglswapbuffers->cleanup( );

        return m_original_swap( hdc );
    }

    if ( !m_initialized )
        wglswapbuffers->init( hdc );

    if ( m_initialized )
        wglswapbuffers->draw( );

    return m_original_swap( hdc );
}

LRESULT CALLBACK c_wglswapbuffers::wndproc_hook( HWND hwnd , UINT msg , WPARAM wparam , LPARAM lparam )
{
    if ( msg == WM_KEYDOWN && wparam == VK_INSERT )
        m_menu_open = !m_menu_open;

    if ( m_menu_open && ImGui_ImplWin32_WndProcHandler( hwnd , msg , wparam , lparam ) )
        return TRUE;

    return CallWindowProcW( m_original_wndproc , hwnd , msg , wparam , lparam );
}

c_wglswapbuffers* wglswapbuffers = new c_wglswapbuffers( );