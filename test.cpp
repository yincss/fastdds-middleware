#include <string>
#include <iostream>
#include <cstring>

#include "ParticipantAPI.h"
#include "DataDistributionAPI.h"
#include "DataDistributionImplFactoryExt.h"
#include <thread>

int g_entityDataTime = 0;
class EntityDataDataReceiveListenerImp : public Sample::DataReceiveListenerBase {
public:
	virtual void   OnDataReceive(const char* Topic_name, const Sample::Data* data) override {
		if (strcmp(Topic_name, "EntityData")  && dynamic_cast<const exampleDataInterchange::EntityData*>(data)) {
			auto datas = dynamic_cast<const exampleDataInterchange::EntityData*>(data);
			std::cout << "--DataRceiveListener: ---\n";
			std::cout << "datas entityid is " << datas->entityId << ",datas.time is "<< datas->time << std::endl;
			g_entityDataTime++;
		}
	}

	virtual ~EntityDataDataReceiveListenerImp(){}
};

int main() {
	std::cout << "EXE Start." << std::endl;
	Sample::DomainInfo domainInfo;
	domainInfo.SetDomainId(1);
	domainInfo.SetParticipantName("ssssssss");
	domainInfo.SetPortID(2);
	domainInfo.SetIP("192.168.1.10");
	Sample::ParticipantAPI* participant1 = Sample::ParticipantAPI::CreateParticipant(domainInfo);

	if (participant1) {
		std::cout << "Create participant Success! domainid : " << domainInfo.GetDomainId() << " id : " << domainInfo.GetPort();
	}
	else
	{
		std::cout << "Create participant Fail!" << std::endl;
		return 0;
	}



	{
		std::cout << " [ RUN ] " << "   EntityData " << std::endl;

		Sample::DataDistributionAPI* dataDistribution1 = exampleDataInterchange::GetEntityDataDataDistribution(participant1);
		EntityDataDataReceiveListenerImp* dataReceiveListenerImp1 = new EntityDataDataReceiveListenerImp;

		Sample::QoS* tempQos = nullptr;
		if (!dataDistribution1->SubscribeTopic("EntityData", tempQos)) {
			std::cout << "SubscribeTopic Fail!" << std::endl;

		}
		else
		{
			std::cout << "SubscribeTopic Success!" << std::endl;

			if (!dataDistribution1->ListenDataReceive("EntityData", dataReceiveListenerImp1)) {
				std::cout << "ListenDataReceive Fail!" << std::endl;

			}
			else
			{
				std::cout << "ListenDataReceive Success!" << std::endl;

				if (!dataDistribution1->PublishTopic("EntityData", tempQos)) {
					std::cout << "PublishTopic Fail!" << std::endl;

				}
				else
				{
					std::cout << "PublishTopic Success!" << std::endl;

					int i = 0;
					while (i++ < 10) {
						exampleDataInterchange::EntityData data;
						if (!dataDistribution1->SendData("EntityData", &data)) {
							std::cout << "SendData Fail!" << std::endl;

						}
						else
						{
							std::cout << "SendData Success!" << std::endl;

						}
					}

				}
			}
		}

		std::cout << " [ RUN ] " << "   EntityData end." << std::endl;

	}




	while (true)
	{

	}




}