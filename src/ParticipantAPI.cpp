#include "ParticipantAPI.h"
#include "ParticipantAPIImpl.h"
#include <mutex>

namespace Sample {
    std::mutex mtx;
    bool  DomainInfo::SetDomainId(uint32_t _doMainId)
    {
        doMainId = _doMainId;
        return true;
    }

    uint32_t  DomainInfo::GetDomainId()
    {
        return doMainId;
    }

    bool  DomainInfo::SetIP(std::string ipStr)
    {
        ipServer = ipStr;
        return true;
    }

    std::string  DomainInfo::GetIP()
    {
        return ipServer;
    }

    bool  DomainInfo::SetPortID(long portStr)
    {
        portServer = portStr;
        return true;
    }

    long  DomainInfo::GetPort()
    {
        return portServer;
    }

    bool  DomainInfo::SetParticipantName(std::string participantName)
    {
        nameParticipant = participantName;
        return true;
    }

    std::string DomainInfo::GetParticipantName() {
        return nameParticipant;
    }

    ParticipantAPI* ParticipantAPI::CreateParticipant(DomainInfo domainInfo, std::string configFilePath) {
        return new ParticipantAPIImpl(domainInfo, configFilePath);
    }
    bool ParticipantAPI::DeleteParticipant(ParticipantAPI* participant) {
        if (!participant) return false;
        std::lock_guard<std::mutex> lock(mtx);
        delete participant;
        return true;
    }
}