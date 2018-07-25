
#include <chrono>
#include "Core/MultiClientsMessageQueue.h"
#include "gtest/gtest.h"
#include <thread>

using namespace Realisim;
    using namespace Core;
using namespace std;

namespace
{
    // global queue shared between the main thread and the
    // working thread.
    MessageQueue gDoneQueue;
    
    //-------------------------------------------------------------------------
    class DoneMessage : public MessageQueue::Message
    {
    public:
        DoneMessage() : MessageQueue::Message() {}
        DoneMessage(const DoneMessage&) = default;
        DoneMessage& operator=(const DoneMessage&) = default;
        virtual ~DoneMessage() = default;
        
        std::string mString;
    };
    
    //-------------------------------------------------------------------------
    class RequestMessage : public MessageQueue::Message
    {
    public:
        RequestMessage() = delete;
        explicit RequestMessage(void* ipSender) : MessageQueue::Message(ipSender) {}
        RequestMessage(const RequestMessage&) = default;
        RequestMessage& operator=(const RequestMessage&) = default;
        virtual ~RequestMessage() = default;
        
        std::string mString;
        int mSleepTimeInMsec;
    };
    
    //-------------------------------------------------------------------------
    class Client0
    {
    public:
        Client0() = default;
        ~Client0() {}

        RequestMessage* makeMessage(const std::string& iM)
        {
            RequestMessage *m = new RequestMessage(this);
            m->mString = iM;
            m->mSleepTimeInMsec = 15;
            return m;
        }
        
        void processRequest(MessageQueue::Message* ipM)
        {
            RequestMessage *rm = dynamic_cast<RequestMessage*>(ipM);
            if(rm)
            {
                DoneMessage *dm = new DoneMessage();
                dm->mString = rm->mString;
                
                // simulate load...
                this_thread::sleep_for(chrono::milliseconds(rm->mSleepTimeInMsec));
                
                gDoneQueue.post(dm);
            }
        }
        
    protected:
    };
    
    //-------------------------------------------------------------------------
    class Client1
    {
    public:
        Client1() = default;
        ~Client1() {}
        
        RequestMessage* makeMessage(const std::string& iM)
        {
            RequestMessage *m = new RequestMessage(this);
            m->mString = iM;
            m->mSleepTimeInMsec = 30;
            return m;
        }
        
        void processRequest(MessageQueue::Message* ipM)
        {
            RequestMessage *rm = dynamic_cast<RequestMessage*>(ipM);
            if(rm)
            {
                DoneMessage *dm = new DoneMessage();
                dm->mString = rm->mString;
                
                // simulate load...
                this_thread::sleep_for(chrono::milliseconds(rm->mSleepTimeInMsec));
                
                gDoneQueue.post(dm);
            }
        }
    protected:
    };
    
    //-------------------------------------------------------------------------
    class Client2
    {
    public:
        Client2() = default;
        ~Client2() {}
        
        RequestMessage* makeMessage(const std::string& iM)
        {
            RequestMessage *m = new RequestMessage(this);
            m->mString = iM;
            m->mSleepTimeInMsec = 45;
            return m;
        }
        
        void processRequest(MessageQueue::Message* ipM)
        {
            RequestMessage *rm = dynamic_cast<RequestMessage*>(ipM);
            if(rm)
            {
                DoneMessage *dm = new DoneMessage();
                dm->mString = rm->mString;
                
                // simulate load...
                this_thread::sleep_for(chrono::milliseconds(rm->mSleepTimeInMsec));
                
                gDoneQueue.post(dm);
            }
        }
    protected:
    };
    
    //-------------------------------------------------------------------------
    void processDoneQueue(MessageQueue::Message* ipMessage)
    {
        DoneMessage *dm = dynamic_cast<DoneMessage*>(ipMessage);
        if(dm)
        {
            printf("received in main thread: %s\n", dm->mString.c_str());
        }
        
    }
}

// This test will post 4 messages with 1 sec processing time
// and will call stopThread before any message could be processed
// this validates we can terminate a thread even it still has
// messages in the queue. It checks that the queue is empty at the
// end, because stopThread is suppose to delete all messages.
//
TEST(MultiClientsMessageQueue, firstTest)
{
    MultiClientsMessageQueue qThreaded;
    Client0 c0;
    Client1 c1;
    Client2 c2;
    
    using placeholders::_1;
    gDoneQueue.setProcessingFunction(bind(processDoneQueue, _1));
    
    function<void(MessageQueue::Message*)> f = bind(&Client0::processRequest, &c0, _1);
    qThreaded.registerAsSender(&c0, f);
    
    f = bind(&Client1::processRequest, &c1, _1);
    qThreaded.registerAsSender(&c1, f);
    
    f = bind(&Client2::processRequest, &c2, _1);
    qThreaded.registerAsSender(&c2, f);
    
    qThreaded.start();
    
    
    qThreaded.post( c0.makeMessage("c0 m0") );
    qThreaded.post( c1.makeMessage("c1 m0") );
    qThreaded.post( c2.makeMessage("c2 m0") );
    
    qThreaded.post( c0.makeMessage("c0 m1") );
    qThreaded.post( c1.makeMessage("c1 m1") );
    qThreaded.post( c2.makeMessage("c2 m1") );
    
    qThreaded.post( c0.makeMessage("c0 m2") );
    qThreaded.post( c1.makeMessage("c1 m2") );
    qThreaded.post( c2.makeMessage("c2 m2") );
    
    qThreaded.waitForThreadToFinish();
    gDoneQueue.processMessages();
}
