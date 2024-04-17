#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Source : public cSimpleModule
{
    private:
      simtime_t timeout;
      cMessage *timeoutEvent;
      cPacket *packet;
      cPacket *datastore;
      int transmissions = 0;
      int successful = 0;
    public:
      Source();
      virtual ~Source();
    protected:
       virtual void initialize () override;
       virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Source);

Source::Source()
{
    timeoutEvent = nullptr;
    datastore = nullptr;
    packet = nullptr;
}

Source::~Source()
{
    cancelAndDelete(timeoutEvent);
    delete packet;
}
void Source :: initialize()
{
    timeout = 1.0;
    timeoutEvent = new cMessage("timeoutEvent");
    EV <<"Sending initial message \n";
    packet = new cPacket("datapacket");
    send(packet->dup(),"out");
    scheduleAt(simTime()+timeout,timeoutEvent);
    transmissions++;

}

void Source::handleMessage(cMessage *msg)
{
    int numberoftrans = par ("limit");
    if (msg == timeoutEvent) {
        EV <<"Timeout expired, resending message and restarting timer \n";
        //cPacket *copy = datastore->dup();
        send(packet->dup(),"out");
        if(transmissions<numberoftrans)
        {
        transmissions++;
        EV<< "Number of current: " << transmissions;
        cancelEvent(timeoutEvent);
        scheduleAt(simTime()+timeout,timeoutEvent);
        }

        /*store a duplicate --> use the duplicate */
    }
    else {
        EV << "Timeout cancelled.\n";
        cancelEvent(timeoutEvent);
        delete msg;
/*way to story it is use the parameters like last class and assign it*/
        datastore = new cPacket("packet");
        // find a way to store this new message and use the same parameter method as classwork 9
        datastore-> setBitLength(8);
        send(datastore,"out");
        if(transmissions<numberoftrans)
        {
        transmissions++;
        successful++;
        EV<< "Number of current transmission: " << transmissions;
        EV<< "Number of successful transmission: " << successful;
        scheduleAt(simTime()+timeout,timeoutEvent);
        }

    }
}

class Destination : public cSimpleModule
{
private:
    double probability;


  protected:
    virtual void handleMessage (cMessage *msg) override;
};

Define_Module(Destination);

void Destination::handleMessage(cMessage *msg)
{

    cPacket *packet = check_and_cast<cPacket *>(msg);

   if (packet->hasBitError()) {
        EV << "\"Message\"lost. \n";
        bubble("message lost");
        delete packet;
    }
    else {
        EV << "Sending back same message as Ack. \n";
        send (msg, "out");
    }
}
