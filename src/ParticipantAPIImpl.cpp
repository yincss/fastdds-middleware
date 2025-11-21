#include "ParticipantAPIImpl.h"
#include <random>

namespace Sample {

    eprosima::fastdds::rtps::GuidPrefix_t GenerateUniqueGuidPrefix(uint32_t domain_id, uint32_t participant_id)
    {
        eprosima::fastdds::rtps::GuidPrefix_t prefix;
        uint8_t* p = prefix.value;

        // ����ʱ�� + ����� + domain_id + participant_id ����
        uint64_t timestamp = static_cast<uint64_t>(
            std::chrono::steady_clock::now().time_since_epoch().count());
        uint32_t random_part = static_cast<uint32_t>(std::random_device{}());
        uint32_t pid = static_cast<uint32_t>(::getpid());

        memcpy(p, &timestamp, 6);
        memcpy(p + 6, &pid, 3);
        memcpy(p + 9, &participant_id, 1);
        memcpy(p + 10, &domain_id, 2);

        // ��ӡ
        std::cout << "GUID Prefix = ";
        for (int i = 0; i < 12; ++i)
            printf("%02X", prefix.value[i]);
        std::cout << std::endl;

        return prefix;
    }

    
    ParticipantAPIImpl::ParticipantAPIImpl(DomainInfo domainInfo, std::string configFilePath)
    {
        std::cout << " ParticipantAPI initialization. " << std::endl;
        domainInfo_ = domainInfo;
        eprosima::fastdds::dds::Log::SetVerbosity(eprosima::fastdds::dds::Log::Kind::Info);
        eprosima::fastdds::dds::Log::SetCategoryFilter(std::regex(".*"));
        std::unique_ptr<eprosima::fastdds::dds::FileConsumer> file_consumer(new eprosima::fastdds::dds::FileConsumer("fastdds.log"));
        eprosima::fastdds::dds::Log::RegisterConsumer(std::move(file_consumer));
        std::cout << "fastdds.log initialization. " << std::endl;

        try {
            eprosima::fastdds::dds::DomainParticipantQos pqos;
            pqos = eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->get_default_participant_qos();
            pqos.name(domainInfo.GetParticipantName());
            std::cout << "getDomainParticipantName " << std::endl;

            pqos.wire_protocol().participant_id = domainInfo.GetPort();
            pqos.wire_protocol().prefix = GenerateUniqueGuidPrefix(domainInfo.GetDomainId(),domainInfo.GetPort());
            // ......
            pqos.transport().use_builtin_transports = false;
            auto udp_transport = std::make_shared<eprosima::fastdds::rtps::UDPv4TransportDescriptor>();
            udp_transport->sendBufferSize = 8 * 1024 * 1024;
            udp_transport->receiveBufferSize = 8 * 1024 * 1024;
            udp_transport->interfaceWhiteList.push_back(domainInfo.GetIP());
            pqos.transport().user_transports.push_back(udp_transport);
            std::cout << "Ip is " << domainInfo.GetIP() << std::endl;

            participant_ = eprosima::fastdds::dds::DomainParticipantFactory::get_shared_instance()->create_participant(domainInfo.GetDomainId(), pqos);
            if (participant_ == nullptr)
            {
                std::cout << " ParticipantAPI initialization failed. " << std::endl;
                throw std::runtime_error(" ParticipantAPI initialization failed");
            }
            else {
                std::cout << " ParticipantAPI create participant_. domainid is " << domainInfo.GetDomainId() << " participant_id is " << pqos.wire_protocol().participant_id << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "participant create fail. exception : " << e.what() << std::endl;
        }
        eprosima::fastdds::dds::Log::Flush();

        if (participant_ != nullptr) {
            eprosima::fastdds::dds::SubscriberQos sub_qos;
            participant_->get_default_subscriber_qos(sub_qos);
            subscriber_ = participant_->create_subscriber(sub_qos, nullptr);
            if (subscriber_ == nullptr)
            {
                std::cout << " ParticipantAPI Subscriber initialization failed." << std::endl;
                throw std::runtime_error(" ParticipantAPI Subscriber initialization failed");
            }
            else {
                std::cout << " ParticipantAPI create subscriber_. " << std::endl;
            }

            eprosima::fastdds::dds::PublisherQos pub_qos = eprosima::fastdds::dds::PUBLISHER_QOS_DEFAULT;
            participant_->get_default_publisher_qos(pub_qos);
            publisher_ = participant_->create_publisher(pub_qos, nullptr);
            if (publisher_ == nullptr)
            {
                std::cout << " ParticipantAPI Publisher initialization failed. " << std::endl;
                throw std::runtime_error(" ParticipantAPI Publisher initialization failed");
            }
            else {
                std::cout << " ParticipantAPI create publisher_. " << std::endl;
            }
        }
    }

    ParticipantAPIImpl::~ParticipantAPIImpl()
    {
        if (participant_) {
            std::cout << " ParticipantAPI delete. domainid is " << domainInfo_.GetDomainId() << " participant_id is " << participant_->get_qos().wire_protocol().participant_id << std::endl;
            participant_->delete_contained_entities();
            std::cout << " ParticipantAPI delete contained_entities. " << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            eprosima::fastdds::dds::DomainParticipantFactory::get_shared_instance()->delete_participant(participant_);
            std::cout << " ParticipantAPI delete_participant. " << std::endl;
            participant_ = nullptr;
        }
    }
    /*
    bool  ParticipantAPIImpl::ListenParticipant(ParticipantListener* listenParticipant) { ... }
    bool  ParticipantAPIImpl::UnlistenParticipant(ParticipantListener* unlistenParticipant) { ... }
    std::set< ParticipantInfo>  ParticipantAPIImpl::GetParticipants() { ... }
}*/
}


