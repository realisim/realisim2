#pragma once

#include "Core/MessageQueue.h"
#include <map>
#include <vector>

namespace Realisim
{
namespace Core
{
    //-------------------------------------------------------------------------
    // This class is used to post request/task to be executed in separated
    // threads. The difference with messageQueue is that multiple clients can
    // use a pool of threads. The idea, is to have a pool of threads to execute
    // task that are triggered by a message.
    //
    // Prior to calling start, it is advised to set the number of threads in the
    // pool with a call to setNumberOfQueues(int iN);
    //
    // after calling start, all threads will be ready to use.
    //
    // simply register to via registerAsSender(...) to hook your callback.
    // then post to the next available queue via post(Core::MessageQueue::Message*)
    // result from the thread should be posted back to the main thread via another
    // MessageQueue in your sender's class (see examples or unit tests.)
    //
    //
    // It is possible to set a maximum number of message to the queues, refer to
    // messageQueue class for more info on that behavior.
    //
    // It wraps a messageQueue and present the post() function to send requests/tasks.
    // 
    // Typically, to use MultiClientsMultiMessageQueues:
    //  1- register using registerAsSender()
    //      The registering process requires a void* to the sender and a callback 
    //      function. The function will be called from the thread when the message
    //      posted via post is being treated. This idea is that the sender is
    //      responsible for the treatment of his message.
    //  2- post custom messages to the queue. Make sure your message are initialized
    //      with the sender. See MessageQueue::Message(void* ipSender) constructor.
    //  3- treat your message in your registered function.
    //
    class MultiClientsMultiMessageQueues
    {
    public:
        MultiClientsMultiMessageQueues();
        MultiClientsMultiMessageQueues(const MultiClientsMultiMessageQueues&) = delete;
        MultiClientsMultiMessageQueues& operator=(const MultiClientsMultiMessageQueues&) = delete;
        ~MultiClientsMultiMessageQueues();

        void clear();
        int getNumberOfQueues() const;
        int getMaximumNumberOfMessages() const;
        bool isStarted() const;
        void post(Core::MessageQueue::Message*);
        void registerAsSender(void* ipSender, std::function<void(Core::MessageQueue::Message*)>& iFunction );
        void setMaximumNumberOfMessages(int);
        void setNumberOfQueues(int iN);
        void start();
        void stop();
        void unregisterAsSender(void* ipSender);
        void waitForThreadsToFinish();

    protected:
        void callRegisteredSenderFunction(Core::MessageQueue::Message*);
        void processRequestMessage(Core::MessageQueue::Message*);

        std::vector<Core::MessageQueue*> mRequestQueues;
        std::map<void*, std::function<void(Core::MessageQueue::Message*)>> mRegisteredSenders;
    };
}
}