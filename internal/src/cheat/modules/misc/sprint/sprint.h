#pragma once

class c_sprint
{
public:
    void run( );
    void shutdown( );

    bool enabled = false;
    int key = 0;
};

extern c_sprint* sprint;