#pragma once

class c_wglswapbuffers
{
public:
    void load( );
    void shutdown( );

private:
    static BOOL WINAPI swap_hook( HDC hdc );
    static LRESULT CALLBACK wndproc_hook( HWND hwnd , UINT msg , WPARAM wparam , LPARAM lparam );

    void init( HDC hdc );
    void cleanup( );
    void draw( );

    inline static bool m_menu_open = false;

    inline static bool m_initialized = false;
    inline static std::atomic<bool> m_unload = false;
    inline static std::atomic<bool> m_cleaned = false;
    inline static HWND m_window = nullptr;
    inline static WNDPROC m_original_wndproc = nullptr;
    inline static BOOL( WINAPI* m_original_swap )( HDC ) = nullptr;
    inline static void* m_target = nullptr;
};

extern c_wglswapbuffers* wglswapbuffers;