#ifndef PARTICIPANTAPI
#define PARTICIPANTAPI

#include "DataDistributionConfig.h"
#include <set>
#include <string>

namespace Sample {
	//交互域信息
	struct DATADISTRIBUTIONCONFIG_API DomainInfo {
    public:
        // Domain ID
        bool SetDomainId(uint32_t doMainId);
        uint32_t GetDomainId();

        // IP Address
        bool SetIP(std::string ipStr);
        std::string GetIP();

        // Port Number
        bool SetPortID(long portStr);
        long GetPort();

        // Participant Name
        bool SetParticipantName(std::string participantName);
        std::string GetParticipantName();

    private:
        uint32_t doMainId;        // Domain ID
        std::string ipServer;     // IP Address
        long portServer;          // Server Port
        std::string nameParticipant; // Participant Identifier
    };
  
	
    class ParticipantAPI
    {
    public:
        // 创建Participant域参与者,开放config Qos
        static DATADISTRIBUTIONCONFIG_API ParticipantAPI* CreateParticipant(DomainInfo domainInfo, std::string configFilePath = "");
        // 删除域参与者
        static DATADISTRIBUTIONCONFIG_API bool DeleteParticipant(ParticipantAPI* participant);
        // 注册参与者监听者
       // virtual bool ListenParticipant(RCComp_ParticipantListener* listenParticipant) = 0;
        // 取消注册参与者监听者
       // virtual bool UnlistenParticipant(RCComp_ParticipantListener* unlistenParticipant) = 0;
        // 查询交互域中参与者
        //virtual std::set<ParticipantInfo> GetParticipants() = 0;
        virtual ~ParticipantAPI() = default;
    protected:
        ParticipantAPI() = default;
        ParticipantAPI(const ParticipantAPI&) = delete;

    };
}

#endif // !PARTICIPANTAPI

