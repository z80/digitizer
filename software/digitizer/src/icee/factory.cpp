// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************
//
// Ice version 3.6.0
//
// <auto-generated>
//
// Generated from file `factory.ice'
//
// Warning: do not edit this file.
//
// </auto-generated>
//

#include <factory.h>
#include <IceUtil/PushDisableWarnings.h>
#include <Ice/LocalException.h>
#include <Ice/ObjectFactory.h>
#include <Ice/Outgoing.h>
#include <Ice/OutgoingAsync.h>
#include <Ice/BasicStream.h>
#include <IceUtil/Iterator.h>
#include <IceUtil/PopDisableWarnings.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 306
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 > 50
#       error Beta header file detected
#   endif
#   if ICE_INT_VERSION % 100 < 0
#       error Ice patch level mismatch!
#   endif
#endif

namespace
{

const ::std::string __Factory__DeviceAbs__status_name = "status";

const ::std::string __Factory__DeviceAbs__release_name = "release";

const ::std::string __Factory__DeviceFactory__interfaces_name = "interfaces";

const ::std::string __Factory__DeviceFactory__query_name = "query";

}
::IceProxy::Ice::Object* ::IceProxy::Factory::upCast(::IceProxy::Factory::DeviceAbs* p) { return p; }

void
::IceProxy::Factory::__read(::IceInternal::BasicStream* __is, ::IceInternal::ProxyHandle< ::IceProxy::Factory::DeviceAbs>& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::Factory::DeviceAbs;
        v->__copyFrom(proxy);
    }
}

::std::string
IceProxy::Factory::DeviceAbs::status(const ::Ice::Context* __ctx)
{
    __checkTwowayOnly(__Factory__DeviceAbs__status_name);
    ::IceInternal::Outgoing __og(this, __Factory__DeviceAbs__status_name, ::Ice::Normal, __ctx);
    __og.writeEmptyParams();
    if(!__og.invoke())
    {
        try
        {
            __og.throwUserException();
        }
        catch(const ::Ice::UserException& __ex)
        {
            ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
            throw __uue;
        }
    }
    ::std::string __ret;
    ::IceInternal::BasicStream* __is = __og.startReadParams();
    __is->read(__ret);
    __og.endReadParams();
    return __ret;
}

::Ice::AsyncResultPtr
IceProxy::Factory::DeviceAbs::begin_status(const ::Ice::Context* __ctx, const ::IceInternal::CallbackBasePtr& __del, const ::Ice::LocalObjectPtr& __cookie)
{
    __checkAsyncTwowayOnly(__Factory__DeviceAbs__status_name);
    ::IceInternal::OutgoingAsyncPtr __result = new ::IceInternal::OutgoingAsync(this, __Factory__DeviceAbs__status_name, __del, __cookie);
    try
    {
        __result->prepare(__Factory__DeviceAbs__status_name, ::Ice::Normal, __ctx);
        __result->writeEmptyParams();
        __result->invoke();
    }
    catch(const ::Ice::Exception& __ex)
    {
        __result->abort(__ex);
    }
    return __result;
}

#ifdef ICE_CPP11

::Ice::AsyncResultPtr
IceProxy::Factory::DeviceAbs::__begin_status(const ::Ice::Context* __ctx, const ::IceInternal::Function<void (const ::std::string&)>& __response, const ::IceInternal::Function<void (const ::Ice::Exception&)>& __exception, const ::IceInternal::Function<void (bool)>& __sent)
{
    class Cpp11CB : public ::IceInternal::Cpp11FnCallbackNC
    {
    public:

        Cpp11CB(const ::std::function<void (const ::std::string&)>& responseFunc, const ::std::function<void (const ::Ice::Exception&)>& exceptionFunc, const ::std::function<void (bool)>& sentFunc) :
            ::IceInternal::Cpp11FnCallbackNC(exceptionFunc, sentFunc),
            _response(responseFunc)
        {
            CallbackBase::checkCallback(true, responseFunc || exceptionFunc != nullptr);
        }

        virtual void completed(const ::Ice::AsyncResultPtr& __result) const
        {
            ::Factory::DeviceAbsPrx __proxy = ::Factory::DeviceAbsPrx::uncheckedCast(__result->getProxy());
            ::std::string __ret;
            try
            {
                __ret = __proxy->end_status(__result);
            }
            catch(const ::Ice::Exception& ex)
            {
                Cpp11FnCallbackNC::exception(__result, ex);
                return;
            }
            if(_response != nullptr)
            {
                _response(__ret);
            }
        }
    
    private:
        
        ::std::function<void (const ::std::string&)> _response;
    };
    return begin_status(__ctx, new Cpp11CB(__response, __exception, __sent));
}
#endif

::std::string
IceProxy::Factory::DeviceAbs::end_status(const ::Ice::AsyncResultPtr& __result)
{
    ::Ice::AsyncResult::__check(__result, this, __Factory__DeviceAbs__status_name);
    ::std::string __ret;
    if(!__result->__wait())
    {
        try
        {
            __result->__throwUserException();
        }
        catch(const ::Ice::UserException& __ex)
        {
            throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
        }
    }
    ::IceInternal::BasicStream* __is = __result->__startReadParams();
    __is->read(__ret);
    __result->__endReadParams();
    return __ret;
}

void
IceProxy::Factory::DeviceAbs::release(const ::Ice::Context* __ctx)
{
    ::IceInternal::Outgoing __og(this, __Factory__DeviceAbs__release_name, ::Ice::Normal, __ctx);
    __og.writeEmptyParams();
    __invoke(__og);
}

::Ice::AsyncResultPtr
IceProxy::Factory::DeviceAbs::begin_release(const ::Ice::Context* __ctx, const ::IceInternal::CallbackBasePtr& __del, const ::Ice::LocalObjectPtr& __cookie)
{
    ::IceInternal::OutgoingAsyncPtr __result = new ::IceInternal::OutgoingAsync(this, __Factory__DeviceAbs__release_name, __del, __cookie);
    try
    {
        __result->prepare(__Factory__DeviceAbs__release_name, ::Ice::Normal, __ctx);
        __result->writeEmptyParams();
        __result->invoke();
    }
    catch(const ::Ice::Exception& __ex)
    {
        __result->abort(__ex);
    }
    return __result;
}

void
IceProxy::Factory::DeviceAbs::end_release(const ::Ice::AsyncResultPtr& __result)
{
    __end(__result, __Factory__DeviceAbs__release_name);
}

const ::std::string&
IceProxy::Factory::DeviceAbs::ice_staticId()
{
    return ::Factory::DeviceAbs::ice_staticId();
}

::IceProxy::Ice::Object*
IceProxy::Factory::DeviceAbs::__newInstance() const
{
    return new DeviceAbs;
}
::IceProxy::Ice::Object* ::IceProxy::Factory::upCast(::IceProxy::Factory::DeviceFactory* p) { return p; }

void
::IceProxy::Factory::__read(::IceInternal::BasicStream* __is, ::IceInternal::ProxyHandle< ::IceProxy::Factory::DeviceFactory>& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::Factory::DeviceFactory;
        v->__copyFrom(proxy);
    }
}

::Factory::TStringArray
IceProxy::Factory::DeviceFactory::interfaces(const ::Ice::Context* __ctx)
{
    __checkTwowayOnly(__Factory__DeviceFactory__interfaces_name);
    ::IceInternal::Outgoing __og(this, __Factory__DeviceFactory__interfaces_name, ::Ice::Idempotent, __ctx);
    __og.writeEmptyParams();
    if(!__og.invoke())
    {
        try
        {
            __og.throwUserException();
        }
        catch(const ::Ice::UserException& __ex)
        {
            ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
            throw __uue;
        }
    }
    ::Factory::TStringArray __ret;
    ::IceInternal::BasicStream* __is = __og.startReadParams();
    __is->read(__ret);
    __og.endReadParams();
    return __ret;
}

::Ice::AsyncResultPtr
IceProxy::Factory::DeviceFactory::begin_interfaces(const ::Ice::Context* __ctx, const ::IceInternal::CallbackBasePtr& __del, const ::Ice::LocalObjectPtr& __cookie)
{
    __checkAsyncTwowayOnly(__Factory__DeviceFactory__interfaces_name);
    ::IceInternal::OutgoingAsyncPtr __result = new ::IceInternal::OutgoingAsync(this, __Factory__DeviceFactory__interfaces_name, __del, __cookie);
    try
    {
        __result->prepare(__Factory__DeviceFactory__interfaces_name, ::Ice::Idempotent, __ctx);
        __result->writeEmptyParams();
        __result->invoke();
    }
    catch(const ::Ice::Exception& __ex)
    {
        __result->abort(__ex);
    }
    return __result;
}

#ifdef ICE_CPP11

::Ice::AsyncResultPtr
IceProxy::Factory::DeviceFactory::__begin_interfaces(const ::Ice::Context* __ctx, const ::IceInternal::Function<void (const ::Factory::TStringArray&)>& __response, const ::IceInternal::Function<void (const ::Ice::Exception&)>& __exception, const ::IceInternal::Function<void (bool)>& __sent)
{
    class Cpp11CB : public ::IceInternal::Cpp11FnCallbackNC
    {
    public:

        Cpp11CB(const ::std::function<void (const ::Factory::TStringArray&)>& responseFunc, const ::std::function<void (const ::Ice::Exception&)>& exceptionFunc, const ::std::function<void (bool)>& sentFunc) :
            ::IceInternal::Cpp11FnCallbackNC(exceptionFunc, sentFunc),
            _response(responseFunc)
        {
            CallbackBase::checkCallback(true, responseFunc || exceptionFunc != nullptr);
        }

        virtual void completed(const ::Ice::AsyncResultPtr& __result) const
        {
            ::Factory::DeviceFactoryPrx __proxy = ::Factory::DeviceFactoryPrx::uncheckedCast(__result->getProxy());
            ::Factory::TStringArray __ret;
            try
            {
                __ret = __proxy->end_interfaces(__result);
            }
            catch(const ::Ice::Exception& ex)
            {
                Cpp11FnCallbackNC::exception(__result, ex);
                return;
            }
            if(_response != nullptr)
            {
                _response(__ret);
            }
        }
    
    private:
        
        ::std::function<void (const ::Factory::TStringArray&)> _response;
    };
    return begin_interfaces(__ctx, new Cpp11CB(__response, __exception, __sent));
}
#endif

::Factory::TStringArray
IceProxy::Factory::DeviceFactory::end_interfaces(const ::Ice::AsyncResultPtr& __result)
{
    ::Ice::AsyncResult::__check(__result, this, __Factory__DeviceFactory__interfaces_name);
    ::Factory::TStringArray __ret;
    if(!__result->__wait())
    {
        try
        {
            __result->__throwUserException();
        }
        catch(const ::Ice::UserException& __ex)
        {
            throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
        }
    }
    ::IceInternal::BasicStream* __is = __result->__startReadParams();
    __is->read(__ret);
    __result->__endReadParams();
    return __ret;
}

::Factory::DeviceAbsPrx
IceProxy::Factory::DeviceFactory::query(const ::std::string& __p_interfaceName, const ::Ice::Context* __ctx)
{
    __checkTwowayOnly(__Factory__DeviceFactory__query_name);
    ::IceInternal::Outgoing __og(this, __Factory__DeviceFactory__query_name, ::Ice::Normal, __ctx);
    try
    {
        ::IceInternal::BasicStream* __os = __og.startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_interfaceName);
        __og.endWriteParams();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    if(!__og.invoke())
    {
        try
        {
            __og.throwUserException();
        }
        catch(const ::Ice::UserException& __ex)
        {
            ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
            throw __uue;
        }
    }
    ::Factory::DeviceAbsPrx __ret;
    ::IceInternal::BasicStream* __is = __og.startReadParams();
    __is->read(__ret);
    __og.endReadParams();
    return __ret;
}

::Ice::AsyncResultPtr
IceProxy::Factory::DeviceFactory::begin_query(const ::std::string& __p_interfaceName, const ::Ice::Context* __ctx, const ::IceInternal::CallbackBasePtr& __del, const ::Ice::LocalObjectPtr& __cookie)
{
    __checkAsyncTwowayOnly(__Factory__DeviceFactory__query_name);
    ::IceInternal::OutgoingAsyncPtr __result = new ::IceInternal::OutgoingAsync(this, __Factory__DeviceFactory__query_name, __del, __cookie);
    try
    {
        __result->prepare(__Factory__DeviceFactory__query_name, ::Ice::Normal, __ctx);
        ::IceInternal::BasicStream* __os = __result->startWriteParams(::Ice::DefaultFormat);
        __os->write(__p_interfaceName);
        __result->endWriteParams();
        __result->invoke();
    }
    catch(const ::Ice::Exception& __ex)
    {
        __result->abort(__ex);
    }
    return __result;
}

#ifdef ICE_CPP11

::Ice::AsyncResultPtr
IceProxy::Factory::DeviceFactory::__begin_query(const ::std::string& __p_interfaceName, const ::Ice::Context* __ctx, const ::IceInternal::Function<void (const ::Factory::DeviceAbsPrx&)>& __response, const ::IceInternal::Function<void (const ::Ice::Exception&)>& __exception, const ::IceInternal::Function<void (bool)>& __sent)
{
    class Cpp11CB : public ::IceInternal::Cpp11FnCallbackNC
    {
    public:

        Cpp11CB(const ::std::function<void (const ::Factory::DeviceAbsPrx&)>& responseFunc, const ::std::function<void (const ::Ice::Exception&)>& exceptionFunc, const ::std::function<void (bool)>& sentFunc) :
            ::IceInternal::Cpp11FnCallbackNC(exceptionFunc, sentFunc),
            _response(responseFunc)
        {
            CallbackBase::checkCallback(true, responseFunc || exceptionFunc != nullptr);
        }

        virtual void completed(const ::Ice::AsyncResultPtr& __result) const
        {
            ::Factory::DeviceFactoryPrx __proxy = ::Factory::DeviceFactoryPrx::uncheckedCast(__result->getProxy());
            ::Factory::DeviceAbsPrx __ret;
            try
            {
                __ret = __proxy->end_query(__result);
            }
            catch(const ::Ice::Exception& ex)
            {
                Cpp11FnCallbackNC::exception(__result, ex);
                return;
            }
            if(_response != nullptr)
            {
                _response(__ret);
            }
        }
    
    private:
        
        ::std::function<void (const ::Factory::DeviceAbsPrx&)> _response;
    };
    return begin_query(__p_interfaceName, __ctx, new Cpp11CB(__response, __exception, __sent));
}
#endif

::Factory::DeviceAbsPrx
IceProxy::Factory::DeviceFactory::end_query(const ::Ice::AsyncResultPtr& __result)
{
    ::Ice::AsyncResult::__check(__result, this, __Factory__DeviceFactory__query_name);
    ::Factory::DeviceAbsPrx __ret;
    if(!__result->__wait())
    {
        try
        {
            __result->__throwUserException();
        }
        catch(const ::Ice::UserException& __ex)
        {
            throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
        }
    }
    ::IceInternal::BasicStream* __is = __result->__startReadParams();
    __is->read(__ret);
    __result->__endReadParams();
    return __ret;
}

const ::std::string&
IceProxy::Factory::DeviceFactory::ice_staticId()
{
    return ::Factory::DeviceFactory::ice_staticId();
}

::IceProxy::Ice::Object*
IceProxy::Factory::DeviceFactory::__newInstance() const
{
    return new DeviceFactory;
}

::Ice::Object* Factory::upCast(::Factory::DeviceAbs* p) { return p; }

namespace
{
const ::std::string __Factory__DeviceAbs_ids[2] =
{
    "::Factory::DeviceAbs",
    "::Ice::Object"
};

}

bool
Factory::DeviceAbs::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__Factory__DeviceAbs_ids, __Factory__DeviceAbs_ids + 2, _s);
}

::std::vector< ::std::string>
Factory::DeviceAbs::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__Factory__DeviceAbs_ids[0], &__Factory__DeviceAbs_ids[2]);
}

const ::std::string&
Factory::DeviceAbs::ice_id(const ::Ice::Current&) const
{
    return __Factory__DeviceAbs_ids[0];
}

const ::std::string&
Factory::DeviceAbs::ice_staticId()
{
#ifdef ICE_HAS_THREAD_SAFE_LOCAL_STATIC
    static const ::std::string typeId = "::Factory::DeviceAbs";
    return typeId;
#else
    return __Factory__DeviceAbs_ids[0];
#endif
}

::Ice::DispatchStatus
Factory::DeviceAbs::___status(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.readEmptyParams();
    ::std::string __ret = status(__current);
    ::IceInternal::BasicStream* __os = __inS.__startWriteParams(::Ice::DefaultFormat);
    __os->write(__ret);
    __inS.__endWriteParams(true);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
Factory::DeviceAbs::___release(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.readEmptyParams();
    release(__current);
    __inS.__writeEmptyParams();
    return ::Ice::DispatchOK;
}

namespace
{
const ::std::string __Factory__DeviceAbs_all[] =
{
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "release",
    "status"
};

}

::Ice::DispatchStatus
Factory::DeviceAbs::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< const ::std::string*, const ::std::string*> r = ::std::equal_range(__Factory__DeviceAbs_all, __Factory__DeviceAbs_all + 6, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __Factory__DeviceAbs_all)
    {
        case 0:
        {
            return ___ice_id(in, current);
        }
        case 1:
        {
            return ___ice_ids(in, current);
        }
        case 2:
        {
            return ___ice_isA(in, current);
        }
        case 3:
        {
            return ___ice_ping(in, current);
        }
        case 4:
        {
            return ___release(in, current);
        }
        case 5:
        {
            return ___status(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
Factory::DeviceAbs::__writeImpl(::IceInternal::BasicStream* __os) const
{
    __os->startWriteSlice(ice_staticId(), -1, true);
    __os->endWriteSlice();
}

void
Factory::DeviceAbs::__readImpl(::IceInternal::BasicStream* __is)
{
    __is->startReadSlice();
    __is->endReadSlice();
}

void 
Factory::__patch(DeviceAbsPtr& handle, const ::Ice::ObjectPtr& v)
{
    handle = ::Factory::DeviceAbsPtr::dynamicCast(v);
    if(v && !handle)
    {
        IceInternal::Ex::throwUOE(::Factory::DeviceAbs::ice_staticId(), v);
    }
}

::Ice::Object* Factory::upCast(::Factory::DeviceFactory* p) { return p; }

namespace
{
const ::std::string __Factory__DeviceFactory_ids[2] =
{
    "::Factory::DeviceFactory",
    "::Ice::Object"
};

}

bool
Factory::DeviceFactory::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__Factory__DeviceFactory_ids, __Factory__DeviceFactory_ids + 2, _s);
}

::std::vector< ::std::string>
Factory::DeviceFactory::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__Factory__DeviceFactory_ids[0], &__Factory__DeviceFactory_ids[2]);
}

const ::std::string&
Factory::DeviceFactory::ice_id(const ::Ice::Current&) const
{
    return __Factory__DeviceFactory_ids[0];
}

const ::std::string&
Factory::DeviceFactory::ice_staticId()
{
#ifdef ICE_HAS_THREAD_SAFE_LOCAL_STATIC
    static const ::std::string typeId = "::Factory::DeviceFactory";
    return typeId;
#else
    return __Factory__DeviceFactory_ids[0];
#endif
}

::Ice::DispatchStatus
Factory::DeviceFactory::___interfaces(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Idempotent, __current.mode);
    __inS.readEmptyParams();
    ::Factory::TStringArray __ret = interfaces(__current);
    ::IceInternal::BasicStream* __os = __inS.__startWriteParams(::Ice::DefaultFormat);
    __os->write(__ret);
    __inS.__endWriteParams(true);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
Factory::DeviceFactory::___query(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.startReadParams();
    ::std::string __p_interfaceName;
    __is->read(__p_interfaceName);
    __inS.endReadParams();
    ::Factory::DeviceAbsPrx __ret = query(__p_interfaceName, __current);
    ::IceInternal::BasicStream* __os = __inS.__startWriteParams(::Ice::DefaultFormat);
    __os->write(__ret);
    __inS.__endWriteParams(true);
    return ::Ice::DispatchOK;
}

namespace
{
const ::std::string __Factory__DeviceFactory_all[] =
{
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "interfaces",
    "query"
};

}

::Ice::DispatchStatus
Factory::DeviceFactory::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< const ::std::string*, const ::std::string*> r = ::std::equal_range(__Factory__DeviceFactory_all, __Factory__DeviceFactory_all + 6, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __Factory__DeviceFactory_all)
    {
        case 0:
        {
            return ___ice_id(in, current);
        }
        case 1:
        {
            return ___ice_ids(in, current);
        }
        case 2:
        {
            return ___ice_isA(in, current);
        }
        case 3:
        {
            return ___ice_ping(in, current);
        }
        case 4:
        {
            return ___interfaces(in, current);
        }
        case 5:
        {
            return ___query(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
Factory::DeviceFactory::__writeImpl(::IceInternal::BasicStream* __os) const
{
    __os->startWriteSlice(ice_staticId(), -1, true);
    __os->endWriteSlice();
}

void
Factory::DeviceFactory::__readImpl(::IceInternal::BasicStream* __is)
{
    __is->startReadSlice();
    __is->endReadSlice();
}

void 
Factory::__patch(DeviceFactoryPtr& handle, const ::Ice::ObjectPtr& v)
{
    handle = ::Factory::DeviceFactoryPtr::dynamicCast(v);
    if(v && !handle)
    {
        IceInternal::Ex::throwUOE(::Factory::DeviceFactory::ice_staticId(), v);
    }
}
