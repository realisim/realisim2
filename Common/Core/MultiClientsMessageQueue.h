#pragma once

#include "Core/MessageQueue.h"
#include <map>

namespace Realisim
{
namespace Core
{
    //-------------------------------------------------------------------------
    // This class is used to post request/task to be executed in a separated
    // thread. The difference with messageQueue is that multiple clients can
    // use the same thread. The typical usage would be to share a common
    // service/resource between multiple clients. 
    //
    // For example, we have multiple clients that wants to load data from the disk.
    // Having multiple thread reading from the disk is not particularly a good idea.
    // MultiClientsMessageQueue will permit those multiple clients to send messages
    // to the messageQueue (threaded) and ease the management related to such
    // use case.
    //
    // It is possible to set a maximum number of message to the queues, refer to
    // messageQueue class for more info on that behavior.
    //
    // It wraps a messageQueue and present the post() function to send requests/tasks.
    // 
    // Typically, to use MultiClientsMessageQueue:
    //  1- register using registerAsSender()
    //      The registering process requires a void* to the sender and a callback 
    //      function. The function will be called from the thread when the message
    //      posted via post is being treated. This idea is that the sender is
    //      responsible for the treatment of his message.
    //  2- post custom messages to the queue. Make sure your message are initialized
    //      with the sender. See MessageQueue::Message(void* ipSender) constructor.
    //  3- treat your message in your registered function.
    //
    class MultiClientsMessageQueue
    {
    public:
        MultiClientsMessageQueue();
        MultiClientsMessageQueue(const MultiClientsMessageQueue&) = delete;
        MultiClientsMessageQueue& operator=(const MultiClientsMessageQueue&) = delete;
        ~MultiClientsMessageQueue();

        int getMaximumNumberOfMessages() const;
        bool isStarted() const;
        void post(Core::MessageQueue::Message*);
        void registerAsSender(void* ipSender, std::function<void(Core::MessageQueue::Message*)>& iFunction );
        void setMaximumNumberOfMessages(int);
        void start();
        void stop();
        void unregisterAsSender(void* ipSender);
        void waitForThreadToFinish();

    protected:
        void callRegisteredSenderFunction(Core::MessageQueue::Message*);
        void processRequestMessage(Core::MessageQueue::Message*);

        Core::MessageQueue mRequestQueue;
        std::map<void*, std::function<void(Core::MessageQueue::Message*)>> mRegisteredSenders;
    };
}
}