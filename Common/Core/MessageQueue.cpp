
#include <cassert>
#include "MessageQueue.h"


using namespace Realisim;
    using namespace Core;
using namespace std;

//------------------------------------------------------------------------------
MessageQueue::MessageQueue() :
mThread(),
mQueue(),
mMutex(),
mState(sIdle)
{
    using std::placeholders::_1;
    mProcessingFunction = std::bind(&MessageQueue::dummyProcessingFunction, this, _1);
}

//------------------------------------------------------------------------------
MessageQueue::~MessageQueue()
{
    stopThread();
    assert(mQueue.empty());
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
    mMutex.unlock();
}

//------------------------------------------------------------------------------
void MessageQueue::dummyProcessingFunction(Message* iM)
{
    printf("dummy processing function\n" );
}

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
MessageQueue::state MessageQueue::getState() const
{
    return mState;
}

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
void MessageQueue::post( Message* iM )
{
    // the queue will not accept messages when a request to stop
    // has been issued. Else it would never terminate when stopThread() is called...
    //
    if(getState() != sStopping)
    {
        mMutex.lock();
        mQueue.push_back(iM);
        mMutex.unlock();
        mQueueWaitCondition.notify_one();
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
    if( getNumberOfMessages() > 0 )
    {
        mMutex.lock();
        Message *m = mQueue.front();
        mQueue.pop_front();
        mMutex.unlock();

        mProcessingFunction(m);

        delete m;
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
    mThread = std::thread(&MessageQueue::threadLoop, this);
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
    if(mThread.joinable())
    {
        assert(getState() == sRunning && "If we are not in running state, it means we have a state issue...");
        setState(sStopping);

        //delete everything in the queue
        clear();
        
        //wake threadloop whaterver he is doing
        mQueueWaitCondition.notify_one();
        mThread.join(); //wait till end of execution
    }
}

//------------------------------------------------------------------------------
// This is the function executed in a thread when method start() is called
void MessageQueue::threadLoop()
{
    while( getState() != sIdle )
    {
        // The queue is empty...
        // wait until someonce post a message
        //
        // stopping was requested, wake the queue to enable stopThread
        // and waitForThreadTofinish to be able to join() this thread
        // event if the queue is empty.
        //
        std::unique_lock<std::mutex> lk(mWaitConditionMutex);
        mQueueWaitCondition.wait(lk, [this]()
            {return !mQueue.empty() || getState() == sStopping;}) ;
        
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
    if(mThread.joinable())
    {
        assert(getState() == sRunning && "If we are not in running state, it means we have a state issue...");
        setState(sStopping);
        
        //wake the thread so he can finish
        mQueueWaitCondition.notify_one();
        mThread.join(); //wait till end of execution
    }
}

//------------------------------------------------------------------------------
//--- MessageQueue::Message
//------------------------------------------------------------------------------
MessageQueue::Message::Message(void *ipSender):
mpSender(ipSender)
{}
