#include "DataDistributionAPI.h"
#include "DataStandard.h"

namespace Sample {

	Controller::~Controller(){}

	bool DataDistributionFactory::DeleteDataDistribution(DataDistributionAPI* dataDistribution)
	{
		if (dataDistribution) {
			delete dataDistribution;
			return true;
		}
		else
		{
			return false;
		}
		
	}
}