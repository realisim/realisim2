
#include <algorithm>
#include <cassert>
#include "MessageQueue.h"

using namespace Realisim;
    using namespace Core;
using namespace std;

//------------------------------------------------------------------------------
MessageQueue::MessageQueue() :
mThreads(),
mQueue(),
mMutex(),
mState(sIdle),
mMaximumSize(-1),
mBehavior(bFifo),
mIsIdenticalContiguousMessageAllowed(true)

{
    using std::placeholders::_1;
    mProcessingFunction = std::bind(&MessageQueue::dummyProcessingFunction, this, _1);

    // by default, on thread available, thread is not started...
    mThreads.resize(1);
}

//------------------------------------------------------------------------------
MessageQueue::~MessageQueue()
{
    // clear the queue prior to calling stop thread in case the queue
    // is not threaded...
    // Stop thread will also clear the queue but only if it was launch as a
    // thread
    //
    clear();
    stopThread();
    assert(mQueue.empty());
}


//------------------------------------------------------------------------------
void MessageQueue::allowIdenticalContiguousMessage(bool iA)
{
    mIsIdenticalContiguousMessageAllowed = iA;
}

//------------------------------------------------------------------------------
// clears all message in queue without processing them.
// 
void MessageQueue::clear()
{
    mMutex.lock();

    //delete all message in queue
    for (size_t i = 0; i < mQueue.size(); ++i)
    {
        delete mQueue[i];
    }

    mQueue.clear();

    mQueue.resize(0);

    mMutex.unlock();
}

//------------------------------------------------------------------------------
void MessageQueue::dummyProcessingFunction(Message* iM)
{
    (void)iM; //suppress warning
    printf("dummy processing function\n");
}

//------------------------------------------------------------------------------
MessageQueue::Behavior MessageQueue::getBehavior() const
{ return mBehavior; }

//------------------------------------------------------------------------------
int MessageQueue::getMaximumSize() const
{ return mMaximumSize; }

//------------------------------------------------------------------------------
int MessageQueue::getNumberOfMessages() const
{
    int r = 0;
    mMutex.lock();
    r = (int)mQueue.size();
    mMutex.unlock();
    return r;
}

//------------------------------------------------------------------------------
int MessageQueue::getNumberOfThreads() const
{
    return (int)mThreads.size();
}

//------------------------------------------------------------------------------
MessageQueue::state MessageQueue::getState() const
{
    return mState;
}

//------------------------------------------------------------------------------
bool MessageQueue::hasLimitedSize() const
{ return getMaximumSize() != -1; }

//------------------------------------------------------------------------------
bool MessageQueue::isEmpty() const
{
    bool r = true;
    mMutex.lock();
    r = mQueue.empty();
    mMutex.unlock();
    return r;
}

//------------------------------------------------------------------------------
bool MessageQueue::isIdenticalContiguousMessageAllowed() const
{
    return mIsIdenticalContiguousMessageAllowed;
}

//------------------------------------------------------------------------------
void MessageQueue::post( Message* iM )
{
    // the queue will not accept messages when a request to stop
    // has been issued. Else it would never terminate when stopThread() is called...
    //
    if(getState() != sStopping)
    {
        mMutex.lock();

        // When the queue has a limited size and that size has been reached, it
        // drops the oldest message.
        // the oldest message in both behavior is at the front of the queue,
        // since we always push backed...
        //
        if (hasLimitedSize() && mQueue.size() == getMaximumSize())
        {
            Message *m = nullptr;
            m = mQueue.front();
            mQueue.pop_front();
            delete m;
        }

        //before inserting the message, let's verify if we enabled contiguous identical message
        if (isIdenticalContiguousMessageAllowed() || 
            mQueue.size() == 0 ||
            *(mQueue.front()) != *iM)
        {
            mQueue.push_back(iM);
        }

        mMutex.unlock();
        mQueueWaitCondition.notify_one();
    }
    else
    {
        // while stopping message are not accepted
        // delete right away
        //
        delete iM;
        iM = nullptr;
    }
}

//------------------------------------------------------------------------------
void MessageQueue::processMessages()
{
    while( getNumberOfMessages() > 0 )
    {
        processNextMessage();
    }
}

//------------------------------------------------------------------------------
void MessageQueue::processNextMessage()
{
    // pop first message and process it!
    mMutex.lock();
    if( mQueue.size() > 0 )
    {
        Message *m = nullptr;
        switch (getBehavior())
        {
        case bFifo:
            m = mQueue.front();
            mQueue.pop_front();
            break;
        case bLifo:
            m = mQueue.back();
            mQueue.pop_back();
            break;
        default: assert(0); break;
        }
        mMutex.unlock();

        mProcessingFunction(m);

        delete m;
    }
    else
    {
        mMutex.unlock();
    }
}

//------------------------------------------------------------------------------
// Sets the behavior of the queue
//
// NOTE: Calling this function will delete the content of the queue, it is meant
//      to be called as an initialized not at runtime...
//
void MessageQueue::setBehavior(Behavior iB)
{
    clear();
    mBehavior = iB;
}

//------------------------------------------------------------------------------
// Sets the maximal size of the queue. 
// One the queue as reach the size limit, any new message comming in will cause
// the last message to be deleted (the last message depends on the behavior 
// fifo/lifo)

// A value of -1 will indicate that the queue is infinite, thus messages are never
// lost.
//
// NOTE: Calling this function will delete the content of the queue, it is meant
//      to be called as an initialized not at runtime...
//
void MessageQueue::setMaximumSize(int iSize)
{
    clear();
    mMaximumSize = iSize;
}

//------------------------------------------------------------------------------
void MessageQueue::setNumberOfThreads(int iN)
{
    const int n = std::max(iN, 1); // cannot assign less than 1 thread.
    if (getState() != sIdle)
    {
        waitForThreadToFinish();
        mThreads.resize(n);
        startInThread();
    }
    else
    {
        mThreads.resize(n);
    }
}

//------------------------------------------------------------------------------
void MessageQueue::setProcessingFunction(std::function<void(Message*)> iFunction)
{
    if(iFunction)
    {
        mProcessingFunction = iFunction;
    }
    else
    {
        using std::placeholders::_1;
        mProcessingFunction = std::bind(&MessageQueue::dummyProcessingFunction, this, _1);
    }
}

//------------------------------------------------------------------------------
void MessageQueue::setState(MessageQueue::state iState)
{
    mMutex.lock();
    if(mState != iState)
    {
        mState = iState;
    }
    mMutex.unlock();
}

//------------------------------------------------------------------------------
void MessageQueue::startInThread()
{
#ifndef MESSAGE_QUEUE_NO_THREADING
    //early out
    if(getState() != sIdle) return;
    
    mMutex.lock();
    setState(sRunning);
    for (int i = 0; i < getNumberOfThreads(); ++i)
    {
        mThreads[i] = std::thread(&MessageQueue::threadLoop, this);
    }
    mMutex.unlock();
    
#endif // !MESSAGE_QUEUE_NO_THREADING
}

//------------------------------------------------------------------------------
// This will delete all message in the queue and stop the thread as soon
// as possible. It will definitely not process remaining messages.
// see waitForThreadToFinish to terminate the thread while processing
// all messages in queue.
//
void MessageQueue::stopThread()
{
    // there is always at least one thread.
    if (mThreads[0].joinable())
    {
        assert(getState() == sRunning && "If we are not in running state, it means we have a state issue...");
        setState(sStopping);

        //delete everything in the queue
        clear();

        //wake threadloop whaterver he is doing
        mQueueWaitCondition.notify_all();

        //wait till end of execution
        for (auto &t : mThreads)
        {
            if(t.joinable())
            { t.join(); }
        }
    }
}

//------------------------------------------------------------------------------
// This is the function executed in a thread when method start() is called
void MessageQueue::threadLoop()
{
    while( getState() != sIdle  )
    {
        // The queue is empty...
        // wait until someonce post a message
        //
        // stopping was requested, wake the queue to enable stopThread
        // and waitForThreadTofinish to be able to join() this thread
        // event if the queue is empty.
        //
        std::unique_lock<std::recursive_mutex> lk(mMutex);
        mQueueWaitCondition.wait(lk, [this]()
            {return mQueue.size() > 0 || getState() != sRunning;}) ;
        lk.unlock();
        
        processNextMessage();
        
        // if we are stopping and the queue is empty, then we are
        // done, set state to idle, this will exit the while loop
        // of ::threadLoop and the thread will end.
        if(getState() == sStopping && mQueue.empty())
        { setState(sIdle); }
    }
}

//------------------------------------------------------------------------------
// This will stop the thread but will treat all message still in the queue
// before stopping the thread
//
void MessageQueue::waitForThreadToFinish()
{
    // there is always at least one thread.
    if (mThreads[0].joinable())
    {
        assert(getState() == sRunning && "If we are not in running state, it means we have a state issue...");

        setState(sStopping);

        //wake threadloop whaterver he is doing
        mQueueWaitCondition.notify_all();

        //wait till end of execution
        for(auto &t : mThreads)
        { 
            if(t.joinable())
            { 
                t.join();
            }
        }
    }
}

//------------------------------------------------------------------------------
//--- MessageQueue::Message
//------------------------------------------------------------------------------
MessageQueue::Message::Message():
mpSender(nullptr)
{}

MessageQueue::Message::Message(void *ipSender):
mpSender(ipSender)
{}
