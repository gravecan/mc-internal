#pragma once
#include <jni/jni.h>

enum class ref_mode
{
    local,
    global,
    weak
};

class c_jvm
{
public:
    void load( );

    static JNIEnv* get_env( );

    inline static JavaVM* vm = nullptr;
    inline static thread_local JNIEnv* env = nullptr;
};

template<typename T>
class c_jni_ref
{
public:
    c_jni_ref( T obj = nullptr , ref_mode mode = ref_mode::local )
        : m_obj( obj ) , m_mode( mode )
    {
        if ( !m_obj || m_mode == ref_mode::local )
            return;

        JNIEnv* e = c_jvm::get_env( );

        if ( !e )
        {
            m_obj = nullptr;
            return;
        }

        if ( m_mode == ref_mode::global )
            m_obj = static_cast<T>( e->NewGlobalRef( static_cast<jobject>( obj ) ) );
        else
            m_obj = static_cast<T>( e->NewWeakGlobalRef( static_cast<jobject>( obj ) ) );
    }

    c_jni_ref( const c_jni_ref& ) = delete;
    c_jni_ref& operator=( const c_jni_ref& ) = delete;

    c_jni_ref( c_jni_ref&& other ) noexcept
        : m_obj( other.m_obj ) , m_mode( other.m_mode )
    {
        other.m_obj = nullptr;
    }

    c_jni_ref& operator=( c_jni_ref&& other ) noexcept
    {
        if ( this != &other )
        {
            cleanup( );

            m_obj = other.m_obj;
            m_mode = other.m_mode;

            other.m_obj = nullptr;
        }

        return *this;
    }

    ~c_jni_ref( )
    {
        cleanup( );
    }

    T get( ) const
    {
        return m_obj;
    }

    ref_mode get_mode( ) const
    {
        return m_mode;
    }

    explicit operator bool( ) const
    {
        return is_valid( );
    }

    bool is_valid( ) const
    {
        if ( !m_obj )
            return false;

        if ( m_mode == ref_mode::weak )
        {
            JNIEnv* e = c_jvm::get_env( );

            if ( !e )
                return false;

            return !e->IsSameObject( static_cast<jobject>( m_obj ) , nullptr );
        }

        return true;
    }

    bool is_collected( ) const
    {
        if ( m_mode != ref_mode::weak || !m_obj )
            return false;

        JNIEnv* e = c_jvm::get_env( );

        if ( !e )
            return false;

        return e->IsSameObject( static_cast<jobject>( m_obj ) , nullptr );
    }

    c_jni_ref lock( ) const
    {
        if ( !m_obj )
            return c_jni_ref( );

        JNIEnv* e = c_jvm::get_env( );

        if ( !e )
            return c_jni_ref( );

        jobject strong = e->NewLocalRef( static_cast<jobject>( m_obj ) );

        return c_jni_ref( static_cast<T>( strong ) , ref_mode::local );
    }

    T release( )
    {
        T tmp = m_obj;
        m_obj = nullptr;
        return tmp;
    }

    void abandon( )
    {
        m_obj = nullptr;
    }

private:
    void cleanup( )
    {
        if ( !m_obj )
            return;

        JNIEnv* e = c_jvm::get_env( );

        if ( e )
        {
            if ( m_mode == ref_mode::global )
                e->DeleteGlobalRef( static_cast<jobject>( m_obj ) );
            else if ( m_mode == ref_mode::weak )
                e->DeleteWeakGlobalRef( static_cast<jweak>( m_obj ) );
            else
                e->DeleteLocalRef( static_cast<jobject>( m_obj ) );
        }

        m_obj = nullptr;
    }

    T m_obj;
    ref_mode m_mode;
};

extern c_jvm* jvm;