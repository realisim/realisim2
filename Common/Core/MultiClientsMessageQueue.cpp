
#include <cassert>
#include "MultiClientsMessageQueue.h"

using namespace Realisim;
    using namespace Core;
using namespace std;

//-----------------------------------------------------------------------------
MultiClientsMessageQueue::MultiClientsMessageQueue() :
    mRequestQueue()
{
    // configure threaded message queue
    using placeholders::_1;
    function<void(Realisim::Core::MessageQueue::Message*)> f =
        bind(&MultiClientsMessageQueue::processRequestMessage, this, _1);
    mRequestQueue.setOneByOneProcessingFunction(f);
    mRequestQueue.setBehavior(MessageQueue::bFifo);
}

//-----------------------------------------------------------------------------
MultiClientsMessageQueue::~MultiClientsMessageQueue()
{
    mRegisteredSenders.clear();
}

//-----------------------------------------------------------------------------
void MultiClientsMessageQueue::callRegisteredSenderFunction(MessageQueue::Message* ipM)
{
    auto it = mRegisteredSenders.find(ipM->mpSender);
    assert(it != mRegisteredSenders.end());

    if (it != mRegisteredSenders.end())
    {
        it->second(ipM);
    }
}

//-----------------------------------------------------------------------------
int MultiClientsMessageQueue::getMaximumNumberOfMessages() const
{ return mRequestQueue.getMaximumSize(); }

//-----------------------------------------------------------------------------
bool MultiClientsMessageQueue::isStarted() const
{ return mRequestQueue.getState() == MessageQueue::sRunning; }

//-----------------------------------------------------------------------------
void MultiClientsMessageQueue::post(Core::MessageQueue::Message* ipM)
{
    mRequestQueue.post(ipM);
}

//-----------------------------------------------------------------------------
void MultiClientsMessageQueue::processRequestMessage(MessageQueue::Message* ipM)
{
    callRegisteredSenderFunction(ipM);
}

//-----------------------------------------------------------------------------
void MultiClientsMessageQueue::registerAsSender(void* ipSender,
    std::function<void(MessageQueue::Message*)>& iFunction)
{
    mRegisteredSenders[ipSender] = iFunction;
}

//-----------------------------------------------------------------------------
void MultiClientsMessageQueue::setMaximumNumberOfMessages(int iN)
{ mRequestQueue.setMaximumSize(iN); }

//-----------------------------------------------------------------------------
void MultiClientsMessageQueue::start()
{
    if(mRequestQueue.getState() == MessageQueue::sIdle)
    {
        mRequestQueue.startInThread();   
    }
}

//-----------------------------------------------------------------------------
void MultiClientsMessageQueue::stop()
{
    mRequestQueue.stopThread();
}

//-----------------------------------------------------------------------------
void MultiClientsMessageQueue::unregisterAsSender(void* ipSender)
{
    auto it = mRegisteredSenders.find(ipSender);
    if (it != mRegisteredSenders.end())
    {
        mRegisteredSenders.erase(it);
    }
}

//-----------------------------------------------------------------------------
void MultiClientsMessageQueue::waitForThreadToFinish()
{
    mRequestQueue.waitForThreadToFinish();
}