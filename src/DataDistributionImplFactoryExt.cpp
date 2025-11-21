
#include "DataDistributionImplFactoryExt.h"
#include "DataDistributionImpl.h"
#include "DataReceiveListenerImplExt.h"
#include "DataStandardExt.h"
#include "sampleData.h"
#include <mutex>


namespace exampleDataInterchange {

	std::map<Sample::ParticipantAPI*, Sample::DataDistributionAPI*> EntityDataDataDistributionImplFactory::ms_dataDistribution;
	
	Sample::DataDistributionAPI* EntityDataDataDistributionImplFactory::CreateDataDistribution(Sample::ParticipantAPI* participantAPI)
	{
			auto it = ms_dataDistribution.find(participantAPI);
			if (it == ms_dataDistribution.end()) {
				Sample::DataDistributionAPI* dataDistribution = new Sample::DataDistribution<EntityData, sampleData_idl::EntityData, EntityContorller, EntityDataReceiveListenerImpl>(participantAPI);
				 ms_dataDistribution[participantAPI] = dataDistribution;
			}
			return ms_dataDistribution[participantAPI];
		}
		
	Sample::DataDistributionAPI* GetEntityDataDataDistribution(Sample::ParticipantAPI* participantAPI)
	{
		EntityDataDataDistributionImplFactory factory;
		return factory.CreateDataDistribution(participantAPI);
	}
}
