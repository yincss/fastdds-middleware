#include "ParticipantDev.h"
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>

namespace Sample
{
    eprosima::fastdds::dds::DomainParticipant* ParticipantDev::GetDomainParticipant( ParticipantAPI* participant)
    {
        return dynamic_cast< ParticipantAPIImpl*>(participant)->GetDomainParticipant();
    }

    eprosima::fastdds::dds::Publisher* ParticipantDev::GetPublisher( ParticipantAPI* participant)
    {
        return dynamic_cast< ParticipantAPIImpl*>(participant)->GetPublisher();
    }

    eprosima::fastdds::dds::Subscriber* ParticipantDev::GetSubscriber( ParticipantAPI* participant)
    {
        return dynamic_cast< ParticipantAPIImpl*>(participant)->GetSubscriber();
    }

     ParticipantAPIImpl::publisherListener* ParticipantDev::GetPubListener( ParticipantAPI* participant)
    {
        return dynamic_cast< ParticipantAPIImpl*>(participant)->GetPublisherListener();
    }
};