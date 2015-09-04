
#ifndef __THREAD_ICE_H_
#define __THREAD_ICE_H_

#include <IceUtil/Thread.h>
#include <IceUtil/Monitor.h>
#include "factory_ice.h"
#include <queue>
#include <string>

class MainWnd;

class ThreadIce: public IceUtil::Thread
{
public:
    ThreadIce( const std::string & listen  = "tcp -p 12345" );
    ~ThreadIce();

    void shutdown();
    std::string lastError();
    std::string status() const;

    bool listen( MainWnd * mw );

    void run();
protected:
    Ice::ObjectAdapterPtr adapter();

    Ice::CommunicatorPtr             m_comm;
    Ice::ObjectAdapterPtr            m_adapter;
    IceUtil::Mutex                   m_mutex;
    IceUtil::Monitor<IceUtil::Mutex> m_semaphore;
    
    std::queue< std::string >        m_errors;
    std::string                      m_status;
    
    std::string                 m_listen;
    IceUtil::Handle<FactoryIce> m_factory;

    MainWnd * m_mainWnd;

    friend class FactoryIce;
};

#endif







