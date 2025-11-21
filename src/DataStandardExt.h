#ifndef DATASTANDARDEXT_H
#define DATASTANDARDEXT_H

#include "sampleData.h"
#include "sampleData.hpp"
#include "DataStandard.h"
#include <fastdds/dds/topic/TypeSupport.hpp>
#include "sampleDataPubSubTypes.hpp"

namespace exampleDataInterchange {

	class EntityContorller : public Sample::Controller {
	public:
		
		static sampleData_idl::EntityData Data_to_DDS(exampleDataInterchange::EntityData data) {
			sampleData_idl::EntityData _ddsData;
			_ddsData.entityId(data.entityId);
			_ddsData.entityLevel(data.entityLevel);
			_ddsData.time(data.time);

			sampleData_idl::Vec3Df data1;
			data1.x(data.pos.x);
			data1.y(data.pos.y);
			data1.z(data.pos.z);

			_ddsData.pos(data1);

			return _ddsData;
		}


		static exampleDataInterchange::EntityData  Data_to_C(sampleData_idl::EntityData _ddsData) {
			exampleDataInterchange::EntityData data;
			data.entityId = _ddsData.entityId();
			data.entityLevel = (level)_ddsData.entityLevel();
			data.time = _ddsData.time();

			sampleData_idl::Vec3Df data1 = _ddsData.pos();
			data.pos.x = data1.x();
			data.pos.y = data1.y();
			data.pos.z = data1.z();
			return data;

		}


		eprosima::fastdds::dds::TypeSupport dataType_DDS;
		std::set<std::string> Topic_names = { "EntityData"};
		
		
		EntityContorller() : dataType_DDS(new sampleData_idl::EntityDataPubSubType()) {

		}
		~EntityContorller() {

		}

	};

}
#endif // !DATASTANDARD
