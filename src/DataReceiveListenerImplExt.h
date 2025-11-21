#ifndef DATARECEIVELISTENERIMPLEXT_H
#define DATARECEIVELISTENERIMPLEXT_H

#include "DataReceiveListenerImpl.h"
#include "DataDistributionConfig.h"
#include "sampleData.h"

namespace exampleDataInterchange {

	class DATADISTRIBUTIONCONFIG_API EntityDataReceiveListenerImpl : public Sample::DataReceiveListenerImpl<EntityData> {
	public:
		virtual void on_data_available(eprosima::fastdds::dds::DataReader* reader) override;
	};


}

#endif // !DATARECEIVELISTENERIMPLEXT_H
