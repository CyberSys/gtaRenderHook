#include "DebugLogger.h"
#include <Windows.h>
#include <fstream>

using namespace rh::debug;

std::unique_ptr<rh::engine::OutFileStream> DebugLogger::m_pLogStream = nullptr;
rh::engine::String DebugLogger::m_sFileName = TEXT( "rhdebug.log" );
LogLevel DebugLogger::m_MinLogLevel = LogLevel::Info;

rh::engine::String ToRHString( const std::string &t_str )
{
#ifndef UNICODE
    return t_str;
#else
    rh::engine::String str;

    std::size_t res_size = MultiByteToWideChar( CP_ACP, 0, t_str.c_str(), t_str.size(), nullptr, 0 );

    str.resize( res_size );

    MultiByteToWideChar( CP_ACP, 0, t_str.c_str(), t_str.size(), str.data(), res_size );

    return str;
#endif
}

rh::engine::String ToRHString( const std::wstring &t_str )
{
#ifdef UNICODE
    return t_str;
#else
    rh::engine::String str;
    const auto res_size = WideCharToMultiByte( CP_ACP,
                                               0,
                                               t_str.c_str(),
                                               static_cast<int>( t_str.size() ),
                                               nullptr,
                                               0,
                                               nullptr,
                                               nullptr );
    str.resize( static_cast<std::size_t>( res_size ) );
    WideCharToMultiByte( CP_ACP,
                         0,
                         t_str.c_str(),
                         static_cast<int>( t_str.size() ),
                         str.data(),
                         static_cast<int>( res_size ),
                         nullptr,
                         nullptr );
    return str;
#endif
}

std::string FromRHString( const rh::engine::String &t_str )
{
#ifndef UNICODE
    return t_str;
#else
    rh::engine::String str;
    std::size_t res_size = WideCharToMultiByte( CP_ACP,
                                                0,
                                                t_str.c_str(),
                                                t_str.size(),
                                                nullptr,
                                                0,
                                                nullptr,
                                                nullptr );
    str.resize( res_size );
    WideCharToMultiByte( CP_ACP,
                         0,
                         t_str.c_str(),
                         t_str.size(),
                         str.data(),
                         res_size,
                         nullptr,
                         nullptr );
    return str;
#endif
}

std::wstring FromRHString_( const rh::engine::String &t_str )
{
#ifdef UNICODE
    return t_str;
#else
    std::wstring str;

    const auto res_size = MultiByteToWideChar( CP_ACP,
                                               0,
                                               t_str.c_str(),
                                               static_cast<int>( t_str.size() ),
                                               nullptr,
                                               0 );

    str.resize( static_cast<std::size_t>( res_size ) );

    MultiByteToWideChar( CP_ACP,
                         0,
                         t_str.c_str(),
                         static_cast<int>( t_str.size() ),
                         str.data(),
                         static_cast<int>( res_size ) );

    return str;
#endif
}

void DebugLogger::Init( const rh::engine::String &fileName, LogLevel minLogLevel )
{
    m_sFileName = fileName;
    m_MinLogLevel = minLogLevel;

    if ( !m_sFileName.empty() )
        m_pLogStream = std::make_unique<rh::engine::OutFileStream>( m_sFileName );
}

void DebugLogger::Log( const engine::String &msg, LogLevel logLevel )
{
    if ( logLevel >= m_MinLogLevel ) {
        if ( m_pLogStream ) {
            if ( !m_pLogStream->is_open() )
                m_pLogStream->open( m_sFileName, std::fstream::out | std::fstream::app );

            *m_pLogStream << TEXT( "LOG: " ) << msg << TEXT( "\n" );

            m_pLogStream->close();
        }

        OutputDebugString( ( msg + TEXT( "\n" ) ).c_str() );
    }
}

void DebugLogger::Error( const engine::String &msg )
{
    if ( m_pLogStream ) {
        if ( !m_pLogStream->is_open() )
            m_pLogStream->open( m_sFileName, std::fstream::out | std::fstream::app );

        *m_pLogStream << TEXT( "ERROR: " ) << msg << TEXT( "\n" );

        m_pLogStream->close();
    }

    OutputDebugString( ( msg + TEXT( "\n" ) ).c_str() );
}
