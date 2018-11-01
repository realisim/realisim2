
#include <cassert>
#include "MultiClientsMultiMessageQueues.h"

using namespace Realisim;
    using namespace Core;
using namespace std;

//-----------------------------------------------------------------------------
MultiClientsMultiMessageQueues::MultiClientsMultiMessageQueues() :
    mRequestQueues()
{
}

//-----------------------------------------------------------------------------
MultiClientsMultiMessageQueues::~MultiClientsMultiMessageQueues()
{
}

//-----------------------------------------------------------------------------
void MultiClientsMultiMessageQueues::clear()
{
    mRegisteredSenders.clear();
    
    for(size_t i = 0; i < mRequestQueues.size(); ++i)
    {
        delete mRequestQueues[i];
    }
    mRequestQueues.clear();
}

//-----------------------------------------------------------------------------
void MultiClientsMultiMessageQueues::callRegisteredSenderFunction(MessageQueue::Message* ipM)
{
    auto it = mRegisteredSenders.find(ipM->mpSender);
    assert(it != mRegisteredSenders.end());

    if (it != mRegisteredSenders.end())
    {
        it->second(ipM);
    }
}

//-----------------------------------------------------------------------------
int MultiClientsMultiMessageQueues::getMaximumNumberOfMessages() const
{ 
    int r = 0;

    // we must have at least one queue, all queues have same size
    if(getNumberOfQueues() > 0)
    { r = mRequestQueues[0]->getMaximumSize(); }

    return r;
}

//-----------------------------------------------------------------------------
int MultiClientsMultiMessageQueues::getNumberOfQueues() const
{
    return (int)mRequestQueues.size();
}

//-----------------------------------------------------------------------------
bool MultiClientsMultiMessageQueues::isStarted() const
{ 
    bool r = false;

    //we must have at least one queue
    if(getNumberOfQueues() > 0)
    {
        r = mRequestQueues[0]->getState() == MessageQueue::sRunning;
    }
    return r;
}

//-----------------------------------------------------------------------------
void MultiClientsMultiMessageQueues::post(Core::MessageQueue::Message* ipM)
{
    //post in the queue with the least amount of messages
    int r = -1;
    int minMessage = std::numeric_limits<int>::max();
    bool stop = false;
    for(int i = 0; i < (int)mRequestQueues.size() && !stop; ++i)
    {
        const int n = mRequestQueues[i]->getNumberOfMessages();
        if(n == 0)
        {
            r = i;
            stop = true;
        }
        else if(n < minMessage)
        {
            minMessage = n;
            r = i;
        }
        else{}
    }

    if(r != -1)
    { mRequestQueues[r]->post(ipM); }
}

//-----------------------------------------------------------------------------
void MultiClientsMultiMessageQueues::processRequestMessage(MessageQueue::Message* ipM)
{
    callRegisteredSenderFunction(ipM);
}

//-----------------------------------------------------------------------------
void MultiClientsMultiMessageQueues::registerAsSender(void* ipSender,
    std::function<void(MessageQueue::Message*)>& iFunction)
{
    mRegisteredSenders[ipSender] = iFunction;
}

//-----------------------------------------------------------------------------
// Resize the request queue to iN. It has a side effect of stopping the queues
// if they were started. This is to prevent memory. Ideally the number of queues
// should be set prior to starting the queues.
//
void MultiClientsMultiMessageQueues::setNumberOfQueues(int iN)
{
    clear();
    mRequestQueues.resize(iN);
    mRequestQueues.shrink_to_fit();
    
    for(int i = 0; i < iN; ++i)
    {
        mRequestQueues[i] = new MessageQueue();
    }
}

//-----------------------------------------------------------------------------
void MultiClientsMultiMessageQueues::setMaximumNumberOfMessages(int iN)
{ 
    for(size_t i = 0; i < mRequestQueues.size(); ++i)
    {
        mRequestQueues[i]->setMaximumSize(iN);
    }

}

//-----------------------------------------------------------------------------
void MultiClientsMultiMessageQueues::start()
{
    for(size_t i = 0; i < mRequestQueues.size(); ++i)
    {
        if(mRequestQueues[i]->getState() == MessageQueue::sIdle)
        {
            // configure threaded message queue
            using placeholders::_1;
            function<void(Realisim::Core::MessageQueue::Message*)> f =
               bind(&MultiClientsMultiMessageQueues::processRequestMessage, this, _1);
            mRequestQueues[i]->setProcessingFunction(f);
            mRequestQueues[i]->setBehavior(MessageQueue::bFifo);

            mRequestQueues[i]->startInThread();
        }
    }
    
    
}

//-----------------------------------------------------------------------------
void MultiClientsMultiMessageQueues::stop()
{
    for(size_t i = 0; i < mRequestQueues.size(); ++i)
    {
        mRequestQueues[i]->stopThread();
    }
}

//-----------------------------------------------------------------------------
void MultiClientsMultiMessageQueues::unregisterAsSender(void* ipSender)
{
    auto it = mRegisteredSenders.find(ipSender);
    if (it != mRegisteredSenders.end())
    {
        mRegisteredSenders.erase(it);
    }
}

//-----------------------------------------------------------------------------
void MultiClientsMultiMessageQueues::waitForThreadsToFinish()
{
    for(size_t i = 0; i < mRequestQueues.size(); ++i)
    {
        mRequestQueues[i]->waitForThreadToFinish();
    }
}