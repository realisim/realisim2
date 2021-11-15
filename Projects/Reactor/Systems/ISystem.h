
#pragma once

namespace Realisim {
namespace Reactor
{

    class Broker;
    class Hub;

    class ISystem {
    public:
        ISystem() = delete;
        explicit ISystem(Broker* ipBroker, Hub* ipHub) : mBroker(*ipBroker), mHub(*ipHub) { ; }
        ISystem(const ISystem&) = delete;
        ISystem& operator=(const ISystem&) = delete;
        virtual ~ISystem() = 0 {}

        const Broker& getBroker() const { return mBroker; }
        Broker& getBrokerRef() { return mBroker; }

        const Hub& getHub() const { return mHub; }
        Hub& getHubRef() { return mHub; }

        virtual bool preInit() { return true; }
        virtual bool init() { return true; }
        virtual bool postInit() { return true; }

        virtual void preUpdate() { ; }
        virtual void update() { ; }
        virtual void postUpdate() { ; }

        virtual void preTerminate() { ; }
        virtual void terminate() { ; }
        virtual void postTerminate() { ; }

    protected:
        Broker& mBroker;
        Hub& mHub;
    };

}
}

