#ifndef DATADISTRIBUTIONIMPLFACTORYEXT_H
#define DATADISTRIBUTIONIMPLFACTORYEXT_H

#include "DataDistributionAPI.h"
#include "DataDistributionConfig.h"
#include "ParticipantAPI.h"
#include "sampleData.h"

#include <map>

namespace exampleDataInterchange {

	class DATADISTRIBUTIONCONFIG_API EntityDataDataDistributionImplFactory : public Sample::DataDistributionFactory {
	public:
		virtual Sample::DataDistributionAPI* CreateDataDistribution(Sample::ParticipantAPI* participantAPI) override;
	private:
		static std::map<Sample::ParticipantAPI*, Sample::DataDistributionAPI*> ms_dataDistribution;
	};

	DATADISTRIBUTIONCONFIG_API Sample::DataDistributionAPI* GetEntityDataDataDistribution(Sample::ParticipantAPI* participantAPI);
}


#endif // !DATADISTRIBUTIONIMPLFACTORYEXT_H
