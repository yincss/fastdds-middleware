#ifndef SAMPLEDATA_H
#define SAMPLEDATA_H

namespace Sample {
#ifndef SampleData
#define SampleData
	class Data {
	public:
		virtual ~Data() {}
	};
#endif
};

#include <string>

namespace exampleDataInterchange{

	struct Vec3Df {
		double x;
		double y;
		double z;
	};

	enum level {
		LDefault = -1,
	    LLow = 0,
		LMidumn = 1,
		LHigh = 2
	};
	/*实体数据 */
	struct EntityData : public Sample::Data{
		std::string time;
		std::string entityId;
		Vec3Df pos;
		level entityLevel;
	};


	
}

#endif // !SAMPLEDATA_H
