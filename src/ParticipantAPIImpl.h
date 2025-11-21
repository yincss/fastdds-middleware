#ifndef PARTICIPANTAPIIMPL
#define PARTICIPANTAPIIMPL

#include "ParticipantAPI.h"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/qos/DomainParticipantQos.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/log/Log.hpp>
#include <fastdds/dds/log/FileConsumer.hpp>
#include <fastdds/rtps/transport/shared_mem/SharedMemTransportDescriptor.hpp>
#include <fastdds/rtps/transport/UDPv4TransportDescriptor.hpp>

namespace Sample {

    //RCCoComp_ParticipantAPI的实现类
    class  ParticipantAPIImpl : public  ParticipantAPI {
    public:
         ParticipantAPIImpl( DomainInfo domainInfo, std::string configFilePath);
         ~ParticipantAPIImpl() override;

        // 注册参与者监听者
        // bool ListenParticipant( ParticipantListener* listenParticipant);
        // 取消注册参与者监听者
        // bool UnlistenParticipant( ParticipantListener* unlistenParticipant);
        // 查询交互域中参与者
        // std::set< ParticipantInfo> GetParticipants();

        eprosima::fastdds::dds::DomainParticipant* GetDomainParticipant() { return participant_; }
        eprosima::fastdds::dds::Subscriber* GetSubscriber() { return subscriber_; }
        eprosima::fastdds::dds::Publisher* GetPublisher() { return publisher_; }

        // datareader和datawriter 成功匹配，触发on_publication_matched()
        class publisherListener : public eprosima::fastdds::dds::DataWriterListener {
        public:
            publisherListener() : matched_(0) {}
            ~publisherListener() override {}

            void on_publication_matched(eprosima::fastdds::dds::DataWriter*, const eprosima::fastdds::dds::PublicationMatchedStatus& info) override {
                if (info.current_count_change == 1) {
                    matched_ = info.total_count;
                    std::cout << "publisher matched. " << std::endl;
                }
                else if (info.current_count_change == -1) {
                    matched_ = info.total_count;
                    std::cout << "publisher unmatched. " << std::endl;
                }
                else {
                    std::cout << info.current_count_change << " is not a valid value for publicationmatchedStatus current count change. " << std::endl;
                }
            }
            std::atomic_int matched_;
        }listener_;

        publisherListener* GetPublisherListener() { return &listener_; }

    private:
        eprosima::fastdds::dds::DomainParticipant* participant_{ nullptr };
        eprosima::fastdds::dds::Subscriber* subscriber_{ nullptr };
        eprosima::fastdds::dds::Publisher* publisher_{ nullptr };
        DomainInfo domainInfo_;
    };
   

}

#endif