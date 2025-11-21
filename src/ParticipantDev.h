#ifndef PARTICIPANTDEV
#define PARTICIPANTDEV

#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>

#include "ParticipantAPI.h"
#include "ParticipantAPIImpl.h"

namespace eprosima {
    namespace fastdds {
        namespace dds {
            class DomainParticipant;
            class Publisher;
            class Subscriber;
        }
    }
}

namespace Sample
{
    class ParticipantDev
    {
    public:
        static eprosima::fastdds::dds::DomainParticipant* GetDomainParticipant(
            ParticipantAPI* participant);
        static eprosima::fastdds::dds::Publisher* GetPublisher(
            ParticipantAPI* participant);
        static eprosima::fastdds::dds::Subscriber* GetSubscriber(
            ParticipantAPI* participant);
        static  ParticipantAPIImpl::publisherListener* GetPubListener(
                ParticipantAPI* participant);
    };
};

#endif


