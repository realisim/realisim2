
#include <chrono>
#include "Core/MessageQueue.h"
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
    int gIndexOfProcessedMessage = 0;
}

// Inherit MessageQueue::Message to send a custom message in
// the queue.
//
class OneToOneMessage : public MessageQueue::Message
{
public:
    OneToOneMessage(void *ipSender = nullptr) :
        MessageQueue::Message(ipSender)
    {}
    
    ~OneToOneMessage() = default;
    
    std::string mText;
};

// this processing function is on the main thread side and receives
// notification of work done from the working thread.
//
void processDoneQueue(MessageQueue::Message *ipMessage)
{
    ostringstream oss;
    oss << "message" << gIndexOfProcessedMessage << " processed";

    OneToOneMessage *m = dynamic_cast<OneToOneMessage*>(ipMessage);
    
    EXPECT_STREQ(oss.str().c_str(), m->mText.c_str());
    gIndexOfProcessedMessage++;
    
    cout << m->mText << endl;
}

// this processing function is on the work thread and
// simulates an heavy computation.
//
void processOneToOne(MessageQueue::Message *ipMessage)
{
    // simulate heavy work load
    this_thread::sleep_for(chrono::seconds(1));
    
    OneToOneMessage *m = dynamic_cast<OneToOneMessage*>(ipMessage);
    printf("%s\n", m->mText.c_str());
    
    //reply in the done queue
    OneToOneMessage *reply = new OneToOneMessage();
    reply->mText = m->mText + " processed";
    gDoneQueue.post(reply);
}

// This test will post 4 messages with 1 sec processing time
// and will call stopThread before any message could be processed
// this validates we can terminate a thread even it still has
// messages in the queue. It checks that the queue is empty at the
// end, because stopThread is suppose to delete all messages.
//
TEST(MessageQueue, oneToOneFast_stopThread)
{
    MessageQueue qThreaded;
    
    using placeholders::_1;
    gDoneQueue.setProcessingFunction(bind(processDoneQueue, _1));
    
    qThreaded.setProcessingFunction(std::bind(processOneToOne, _1));
    qThreaded.startInThread();
    
    OneToOneMessage *m0 = new OneToOneMessage; m0->mText = "message0";
    OneToOneMessage *m1 = new OneToOneMessage; m1->mText = "message1";
    OneToOneMessage *m2 = new OneToOneMessage; m2->mText = "message2";
    OneToOneMessage *m3 = new OneToOneMessage; m3->mText = "message3";
    
    qThreaded.post(m0);
    qThreaded.post(m1);
    qThreaded.post(m2);
    qThreaded.post(m3);
    
    qThreaded.stopThread();
    gDoneQueue.processMessages();
    
    EXPECT_EQ(qThreaded.getNumberOfMessages(), 0);
}

// This test will post 4 messages with 1 sec processing time
// and will call waitForThreadToFinish. Since process done
// queue makes sure all message were treated in order, we
// validate that the thread did process all messages before
// terminating.
//
TEST(MessageQueue, oneToOneFast_waitForThreadToFinish)
{
    MessageQueue qThreaded;
    
    using placeholders::_1;
    gDoneQueue.setProcessingFunction(bind(processDoneQueue, _1));
    
    qThreaded.setProcessingFunction(std::bind(processOneToOne, _1));
    qThreaded.startInThread();
    
    OneToOneMessage *m0 = new OneToOneMessage; m0->mText = "message0";
    OneToOneMessage *m1 = new OneToOneMessage; m1->mText = "message1";
    OneToOneMessage *m2 = new OneToOneMessage; m2->mText = "message2";
    OneToOneMessage *m3 = new OneToOneMessage; m3->mText = "message3";
    
    qThreaded.post(m0);
    qThreaded.post(m1);
    qThreaded.post(m2);
    qThreaded.post(m3);
    
    qThreaded.waitForThreadToFinish();
    gDoneQueue.processMessages();
    
    EXPECT_EQ(qThreaded.getNumberOfMessages(), 0);
}