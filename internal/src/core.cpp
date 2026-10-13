#include <pch.h>

void c_core::load( )
{
	jvm->load( );
	classloader->load( );
	classes->load( );
	modules->load( );
	wglswapbuffers->load( );
}

void c_core::shutdown( )
{
	wglswapbuffers->shutdown( );
	modules->shutdown( );
    classes->shutdown( );

    c_classloader::class_loader = c_jni_ref<jobject>( );

    if ( c_jvm::vm )
        c_jvm::vm->DetachCurrentThread( );
}

c_core* core = new c_core();