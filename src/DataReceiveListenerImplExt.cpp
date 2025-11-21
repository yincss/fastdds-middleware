#include "DataReceiveListenerImplExt.h"

#include <fastdds/dds/topic/TopicDescription.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include "sampleDataPubSubTypes.hpp"
#include "DataStandardExt.h"
#include "sampleData.hpp"

namespace exampleDataInterchange {
	
	void EntityDataReceiveListenerImpl::on_data_available(eprosima::fastdds::dds::DataReader* reader)
	
    {
        eprosima::fastdds::dds::SampleInfoSeq infoSeq;
        eprosima::fastdds::dds::LoanableSequence<sampleData_idl::EntityData> data;

        eprosima::fastdds::dds::ReturnCode_t retcode = reader ->take( data, infoSeq, eprosima::fastdds::dds::LENGTH_UNLIMITED, eprosima::fastdds::dds::ANY_SAMPLE_STATE, eprosima::fastdds::dds::ANY_VIEW_STATE, eprosima::fastdds::dds::ALIVE_INSTANCE_STATE);
        if (retcode == eprosima::fastdds::dds::RETCODE_OK)
        {
            for (eprosima::fastdds::dds::LoanableCollection::size_type i = 0; i < infoSeq.length(); ++i)
            {
                if ((infoSeq[i].instance_state == eprosima::fastdds::dds::ALIVE_INSTANCE_STATE) && (infoSeq[i].valid_data))
                {
                    // È¡Êý¾Ý
                    exampleDataInterchange::EntityData dataContent = exampleDataInterchange::EntityContorller::Data_to_C(data[i]);

                    for (std::set<Sample::DataReceiveListenerBase*>::iterator iter = m_dataReceiveListeners.begin(); iter != m_dataReceiveListeners.end(); iter++)
                    {
                        Sample::DataReceiveListenerBase* dataReceiveListener = *iter;
                        dataReceiveListener->OnDataReceive(reader->get_topicdescription()->get_name().c_str(), &dataContent); //callback
                    }
                }
            }
        }
        else
        {
            std::cout << reader->get_topicdescription()->get_name().c_str() << " on_data_available failed." << std::endl;
        }
        reader->return_loan(data, infoSeq);
    }

  
}