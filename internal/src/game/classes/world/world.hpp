#pragma once

class c_world
{
public:
    void load( );

    std::vector<c_jni_ref<jobject>> get_players( );
    std::string get_player_name( jobject player );

private:
    jmethodID m_get_players = nullptr;
    jmethodID m_list_size = nullptr;
    jmethodID m_list_get = nullptr;
    jmethodID m_get_name = nullptr;
    jmethodID m_get_string = nullptr;
};

extern c_world* world;