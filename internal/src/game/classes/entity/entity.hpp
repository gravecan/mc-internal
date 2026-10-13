#pragma once

#include <string>

class c_entity
{
public:
    void load( );

    bool is_sprinting( jobject player );
    bool is_on_ground( jobject player );
    void set_sprinting( jobject player , bool state );

private:
    jmethodID m_is_sprinting = nullptr;
    jmethodID m_set_sprinting = nullptr;
    jmethodID m_is_on_ground = nullptr;
};

extern c_entity* entity;