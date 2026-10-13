#pragma once

class c_log4j
{
public:
    void load( );
    void disable( );

private:
    jfieldID f_off = nullptr;
    jmethodID m_set_root = nullptr;
};

extern c_log4j* log4j;