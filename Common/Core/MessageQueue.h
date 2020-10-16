#pragma once

#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace Realisim
{
namespace Core
{
    //-------------------------------------------------------------------------
    // This class represent a thread safe message queue (FIFO). It also
    // comes with a convenient built in threaded function to
    // process the messages in a different thread.
    //
    // Message: Messages posted to the queue must inherit
    //      MessageQueue::Message class. Messages posted must
    //      be allocated on the heap and they will be managed by
    //      MessageQueue class once posted.
    //
    //      Identical messages can be allowed/disallowed from being 
    //      inserted in the queue via method allowIdenticalContiguousMessage(bool).
    //      By default, identical messages are allowed. To prevent them, 
    //      invoke the method with 'false' as a parameter. Furthermore,
    //      you will have to override the method MessageQueue::Message::isEqual(const Message&)
    //      to specify how to determine when messages are identical.
    //
    //
    //      Messages posted to the message queue will be
    //      owned and deleted by the queue once they have been
    //      processed/handled. They can be handled in 3 different ways:
    //      1- processNextMessage() or processMessages() are called,
    //          this calls the processing function. Once
    //          the processing function returns, the message is
    //          delete.
    //      2- clear() is called, all messages are deleted.
    //      3- Message queue destructor is invoked, all messages
    //          are deleted.
    //
    // Processing a message from the queue:
    //      A processing function must be supplied, else the default
    //      one will be used. The default function prints a dummy
    //      message in the console...
    //      A processing function is supplied by calling
    //      setProcessingFunction(). When any of the process messages
    //      function will be called, the supplied processing function
    //      will be invoked.
    //
    //      processNextMessage() will invoke the processing function
    //      on the oldest message (FIFO) in the queue. Once the
    //      processing function returns, the message is deleted.
    //
    //      processMessages() will invoke processNextMessage() until
    //      the queue is empty.
    //
    // Usage as a thread safe message queue.
    //      Messages can be posted to the queue via post() method.
    //
    //      The number of messages in the queue can be queried via
    //      getNumberOfMessages().
    //
    //      Ex:
    //          MessageQueue sQueue;
    //
    //       //In thread 0
    //          Message *m = new Message();
    //          sQueue.post(m);
    //          ...
    //          Message *m = new Message();
    //          sQueue.post(m2);
    //
    //       //In thread 1
    //          // declare a processing function
    //          void doStuff(MessageQueue::Message* m)
    //          { ... do something useful... }
    //
    //          //bind the function to the queue
    //          using placeholders::_1;
    //          sQueue.setProcessingFunction(std::bind(doStuff, _1));
    //
    //          // process all messages prosted to the queue.
    //          sQueue.processMessages();
    //
    //
    // Launch the processing function in a/many thread(s):
    //      The following methods applies only when lauching the
    //      processing function in a thread:
    //
    //      setNumberOfThreads(): define how many threads will be consuming
    //          the messages in the queue. All threads will execute the
    //          same processing function.
    //
    //      getNumberOfThreads(): return how many threads would execute
    //          when startInThread is called.
    //
    //      startInThread(): will lauch a thread that will invoke
    //          the processing function as fast as possible as long
    //          as there are messages in the queue. When the queue is
    //          empty, the thread will wait (no cpu usage).
    //          The thread will wake up when a message is posted.
    //
    //      waitForThreadToFinish(); will stop receiving new messages
    //          and will invoke the processing function for all
    //          remaining messages in the queue. Then the thread
    //          terminates.
    //
    //      stopThread(); will delete all messages (via clear) from the
    //          queue and will try to terminate as fast as possible.
    //
    //  Usage when lauching the processing function in a thread.
    //      Ex:
    //
    //
    class MessageQueue
    {
    public:
        MessageQueue();
        MessageQueue(const MessageQueue&) = delete;
        MessageQueue& operator=(const MessageQueue&) = delete;
        ~MessageQueue();
        
        enum Behavior{bFifo, bLifo};
        enum state{sIdle, sRunning, sStopping};
        
        class Message
        {
        public:
            Message();
            explicit Message(void* ipSender);
            Message(const Message&) = default;
            Message& operator=(const Message&) = default;
            virtual ~Message() = default;

            virtual bool isEqual(const Message&) const {return false;} 
            bool operator==(const Message& iM) const {return isEqual(iM);}
            bool operator!=(const Message& iM) const {return !(this->operator==(iM));}
            
            void* mpSender;
        };
        
        void allowIdenticalContiguousMessage(bool iA);
        void clear();
        Behavior getBehavior() const;
        int getMaximumSize() const;
        int getNumberOfMessages() const;
        int getNumberOfThreads() const;
        state getState() const;
        bool hasLimitedSize() const;
        bool isEmpty() const;
        bool isIdenticalContiguousMessageAllowed() const;
        void post( Message* );
        void processNextMessage();
        void processMessages();
        void setBehavior(Behavior);
        void setMaximumSize(int);
        void setNumberOfThreads(int iN);
        void setProcessingFunction(std::function<void(Message*)>);
        void startInThread();
        void stopThread();
        void waitForThreadToFinish();

    protected:
        void dummyProcessingFunction(Message*);
        void threadLoop();
        void setState(state);
        
        std::vector<std::thread> mThreads;
        std::deque<Message*> mQueue;
        mutable std::recursive_mutex mMutex;
        std::condition_variable_any mQueueWaitCondition;
        state mState;
        std::function<void(Message*)> mProcessingFunction;
        int mMaximumSize;
        Behavior mBehavior;
        bool mIsIdenticalContiguousMessageAllowed;
    };

}
}

