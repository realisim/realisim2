#pragma once

#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <string>
#include <thread>

namespace Realisim
{
namespace Core
{
    //-----------------------------------------------------------------
    // This class represent a thread safe message queue (FIFO). It also
    // comes with a convenient built in threaded function to
    // process the messages in a different thread.
    //
    // Message: Messages posted to the queue must inherit
    //      MessageQueue::Message class. Messages posted must
    //      be allocated on the heap and they will be managed by
    //      MessageQeeue class once posted.
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
    // Launch the processing function in a thread:
    //      The following methods applies only when lauching the
    //      processing function in a thread:
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
        virtual ~MessageQueue();
        
        enum state{sIdle, sRunning, sStopping};
        
        class Message
        {
        public:
            Message() = delete;
            explicit Message(void* ipSender = nullptr);
            Message(const Message&) = default;
            Message& operator=(const Message&) = default;
            virtual ~Message() = default;
            
            void* mpSender;
        };
        
        void clear();
        int getNumberOfMessages() const;
        state getState() const;
        void post( Message* );
        void processNextMessage();
        void processMessages();
        void setProcessingFunction(std::function<void(Message*)>);
        void startInThread();
        void waitForThreadToFinish();
        void stopThread();

    protected:
        void dummyProcessingFunction(Message*);
        void threadLoop();
        void setState(state);
        
        std::thread mThread;
        std::deque<Message*> mQueue;
        mutable std::recursive_mutex mMutex;
        std::mutex mWaitConditionMutex;
        std::condition_variable mQueueWaitCondition;
        state mState;
        std::function<void(Message*)> mProcessingFunction;
    };

}
}

