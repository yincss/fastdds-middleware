
#ifndef DATASTANDARDIMPL_H
#define DATASTANDARDIMPL_H

#include "DataDistributionAPI.h"

#include <fastdds/dds/topic/Topic.hpp>
#include <iostream>
#include "ParticipantDev.h"
#include <mutex>
// #include "StructureFac.h"
// #include "Structure.h"
// #include "ReaderWriterStructureFac.h"
// #include "DataReceiveListenerImpl.h"

#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>

namespace Sample
{
	// 基类派生模板类
	template <class T, class D, class U, class L>
	class DATADISTRIBUTIONCONFIG_API DataDistribution : public DataDistributionAPI
	{
	public:
		DataDistribution(Sample::ParticipantAPI *participant);
		~DataDistribution();

		virtual bool PublishTopic(const char *Topic_name, QoS *qos) override;	// 发布主题
		virtual bool UnPublishTopic(const char *Topic_name) override;			// TODO取消发布主题
		virtual bool SendData(const char *Topic_name, Data *data) override;		// 发送数据
		virtual bool SubscribeTopic(const char *Topic_name, QoS *qos) override; // 订阅主题

		virtual bool UnSubscribeTopic(const char *Topic_name) override; // TODO取消订阅主题
		virtual bool ListenDataReceive(const char *Topic_name, DataReceiveListenerBase *listener) override;//监听者增加新的回调
		virtual bool UnlistenDataReceive(const char *Topic_name, DataReceiveListenerBase *listener) override;//监听者移除回调
		virtual bool SendDatas(const char *Topic_name, const std::vector<Data *> &data) override; // TODO 发送多条数据

		// virtual QoS* MakePublisherQoS(FitCreateQoS_Publisher) override;
		// virtual QoS* MakeSubscriberQoS(FitCreateQoS_Subscriber) override;
		// virtual bool SetPublisherQoS(const char* Topic_name, FitRunQoS_Publisher publisherQos)override;
		// virtual bool SetSubscriberQoS(const char* Topic_name, FitRunQoS_Subscriber subscriberQos) override;

	protected:
		Sample::ParticipantAPI *m_participant;
		std::map<std::string, eprosima::fastdds::dds::Topic *> m_Topics; // 存放当前数据类型的所有已经注册的topic，key为主题名称
		U *m_data;
		std::map<std::string, L *> m_dataReaderListener;
	};

	template <class T, class D, class U, class L>
	DataDistribution<T, D, U, L>::DataDistribution(Sample::ParticipantAPI *participant)
	{
		m_data = new U();
		// 创建参与者
		m_participant = participant;
	}

	template <class T, class D, class U, class L>
	DataDistribution<T, D, U, L>::~DataDistribution() //
	{
		// 删除接收监听者
		m_dataReaderListener.clear();
		// 删除已注册主题
		m_Topics.clear();
	}

	// 为主题对应的数据写者的创建
	// 创建该数据结构某个主题的数据写者，不是所有主题都创建一个数据写者，那样消耗太大
	template <class T, class D, class U, class L>
	bool DataDistribution<T, D, U, L>::PublishTopic(const char *Topic_name, QoS *qos)
	{

		if ((!Topic_name || strlen(Topic_name) == 0)) // 空字符串直接返回
		{
			std::cerr << "ERROR: Wrong Topic Name" << std::endl;
			return false;
		}
		if ((m_data->Topic_names.find(Topic_name) == m_data->Topic_names.end()) && (!m_data->Topic_names.empty())) // 不符合规定的主题直接返回
		{
			std::cerr << "ERROR: not create Topic Name." << std::endl;
			return false;
		}

		if (m_Topics.find(Topic_name) == m_Topics.end()) // 这个主题未创建
		{
			if (eprosima::fastdds::dds::RETCODE_OK != m_data->dataType_DDS.register_type(Sample::ParticipantDev::GetDomainParticipant(m_participant)))
			{
				std::cerr << m_data->dataType_DDS.get_type_name() << " register_type failed!" << std::endl;
				return false;
			}
			else
			{
				std::cout << m_data->dataType_DDS.get_type_name() << " register_type successed!" << std::endl;
			}

			eprosima::fastdds::dds::Topic *topic = Sample::ParticipantDev::GetDomainParticipant(m_participant)->create_topic(Topic_name, m_data->dataType_DDS.get_type_name(), eprosima::fastdds::dds::TOPIC_QOS_DEFAULT);
			if (topic == nullptr)
			{
				std::cerr << Topic_name << " create_topic failed!" << std::endl;
				return false;
			}
			else
			{
				std::cout << Topic_name << " create_topic successed!" << std::endl;
			}

			m_Topics[Topic_name] = topic; // 用数据类型名 主题名来存放topic
		}

		// 获取主题
		eprosima::fastdds::dds::Topic *topic = m_Topics.find(Topic_name)->second;
		// 创建数据写者
		eprosima::fastdds::dds::DataWriterQos dw_default_qos;
		Sample::ParticipantDev::GetPublisher(m_participant)->get_default_datawriter_qos(dw_default_qos);

		if (qos != nullptr)
		{
			// Sample::DataWriterQos* dwqos = dynamic_cast<Sample::DataWriterQos*>(qos);
			// dw_default_qos = dwqos->GetDataWriterQos();
		}
		else
		{
			dw_default_qos.durability().kind = eprosima::fastdds::dds::DurabilityQosPolicyKind::TRANSIENT_LOCAL_DURABILITY_QOS;
			dw_default_qos.reliability().kind = eprosima::fastdds::dds::ReliabilityQosPolicyKind::RELIABLE_RELIABILITY_QOS;
			dw_default_qos.history().kind = eprosima::fastdds::dds::HistoryQosPolicyKind::KEEP_ALL_HISTORY_QOS;
		}

		eprosima::fastdds::dds::DataWriter *base_dw = Sample::ParticipantDev::GetPublisher(m_participant)->create_datawriter(topic, dw_default_qos, Sample::ParticipantDev::GetPubListener(m_participant));
		if (!base_dw)
		{
			std::cerr << "create_datawriter failed!" << std::endl;
			return false;
		}

		return true;
	}
	// TODO
	template <class T, class D, class U, class L>
	bool DataDistribution<T, D, U, L>::UnPublishTopic(const char *Topic_name)
	{

		if (!Topic_name || strlen(Topic_name) == 0) //  空字符串直接返回
		{
			return false;
		}

		if (m_Topics.find(Topic_name) != m_Topics.end()) // 这个主题已发布
		{
			
			eprosima::fastdds::dds::Topic *topic = m_Topics[Topic_name];
			eprosima::fastdds::dds::DataWriter *datawriter_ = Sample::ParticipantDev::GetPublisher(m_participant)->lookup_datawriter(Topic_name);
			if (datawriter_ != nullptr) // 先判断这个数据写者是否存在
			{
				datawriter_->set_listener(nullptr);
				//数据写者存在就删除
				if (eprosima::fastdds::dds::RETCODE_OK != Sample::ParticipantDev::GetPublisher(m_participant)->delete_datawriter(datawriter_))
				{
					std::cerr << "delete_datawriter failed!" << std::endl;
					return false;
				}
			}
			else
			{
				std::cerr << "ERROR: no datawriter" << std::endl;
				return false;
			}
		}
		return true;
	}

	// 订阅主题的实质就是创建给主题的空数据读者
	template <class T, class D, class U, class L>
	bool DataDistribution<T, D, U, L>::SubscribeTopic(const char *Topic_name, QoS *qos)
	{

		if (!Topic_name || strlen(Topic_name) == 0) //  空字符串直接返回
		{
			std::cout << "ERROR: Empty Topic Name" << std::endl;
			return false;
		}

		if ((m_data->Topic_names.find(Topic_name) == m_data->Topic_names.end()) && !m_data->Topic_names.empty()) // 不符合规定的直接返回
		{
			return false;
		}

		if (m_Topics.find(Topic_name) == m_Topics.end()) // 这个主题未被创建
		{
			if (eprosima::fastdds::dds::RETCODE_OK != m_data->dataType_DDS.register_type(Sample::ParticipantDev::GetDomainParticipant(m_participant)))
			{
				std::cerr << m_data->dataType_DDS.get_type_name() << " register_type failed!" << std::endl;
				return false;
			}
			else
			{
				std::cout << m_data->dataType_DDS.get_type_name() << " register_type successed!" << std::endl;
			}

			eprosima::fastdds::dds::Topic *topic = Sample::ParticipantDev::GetDomainParticipant(m_participant)->create_topic(Topic_name, m_data->dataType_DDS.get_type_name(), eprosima::fastdds::dds::TOPIC_QOS_DEFAULT);
			if (topic == nullptr)
			{
				std::cerr << Topic_name << " create_topic failed!" << std::endl;
				return false;
			}
			else
			{
				std::cout << Topic_name << " create_topic successed!" << std::endl;
			}
			m_Topics[Topic_name] = topic; // 用数据类型名 主题名来存放topic
		}

		// 获取主题
		eprosima::fastdds::dds::Topic *topic = m_Topics.find(Topic_name)->second;

		// 创建空监听者
		L *_dataReaderListener = new L();
		m_dataReaderListener[Topic_name] = _dataReaderListener; // 储存该空监听者

		// 创建数据读者
		eprosima::fastdds::dds::DataReader *base_dr;
		eprosima::fastdds::dds::DataReaderQos reader_qos;
		Sample::ParticipantDev::GetSubscriber(m_participant)->get_default_datareader_qos(reader_qos);
		if (qos != nullptr)
		{
			// Sample::DataReaderQos* drQos = dynamic_cast<Sample::DataReaderQos*>(qos);
			// reader_qos = drQos->GetDataReaderQos();
		}
		else
		{
			reader_qos.reliability().kind = eprosima::fastdds::dds::ReliabilityQosPolicyKind::RELIABLE_RELIABILITY_QOS; 
			reader_qos.durability().kind = eprosima::fastdds::dds::DurabilityQosPolicyKind::TRANSIENT_LOCAL_DURABILITY_QOS;
			reader_qos.history().kind = eprosima::fastdds::dds::HistoryQosPolicyKind::KEEP_ALL_HISTORY_QOS;
			// reader_qos.history().depth = 1;
		}
		base_dr = Sample::ParticipantDev::GetSubscriber(m_participant)->create_datareader(topic, reader_qos, _dataReaderListener);
		// DefaultQoS::GetInstance()->SetRunDataReaderQos(Topic_name, reader_qos);

		if (base_dr == nullptr)
		{
			std::cerr << Topic_name << " create_datareader failed!" << std::endl;
			return false;
		}
		else
		{
			std::cout << Topic_name << " create_datareader successed! " << std::endl;
		}
		return true;
	}

	// 取消订阅TODO
	template <class T, class D, class U, class L>
	bool DataDistribution<T, D, U, L>::UnSubscribeTopic(const char *Topic_name)
	{
		if (!Topic_name || strlen(Topic_name) == 0) // 主题名称不为空
		{
			return false;
		}
		eprosima::fastdds::dds::DataReader *datareader_ = Sample::ParticipantDev::GetSubscriber(m_participant)->lookup_datareader(Topic_name);
		if (datareader_ != nullptr) // 数据读者存在
		{
			datareader_->set_listener(nullptr);
			auto ret = Sample::ParticipantDev::GetSubscriber(m_participant)->delete_datareader(datareader_);
			if (eprosima::fastdds::dds::RETCODE_OK != ret)
			{
				std::cerr << Topic_name << " with Subscriber delete_datareader failed!" << std::endl;
				return false;
			}
		}
		else
		{
			std::cerr << "ERROR: no datareader" << std::endl;
			return false;
		}
		std::cout << Topic_name << " UnSubscribeTopic." << std::endl;
		return true;
	}

	template <class T, class D, class U, class L>
	bool DataDistribution<T, D, U, L>::ListenDataReceive(const char *Topic_name, Sample::DataReceiveListenerBase *listener)
	{
		if (!listener) // 空监听者直接返回，此处这个监听者是自己实现的
		{
			std::cout << "False RCComp::DataReceiveListener" << std::endl;
			return false;
		}

		if (Sample::ParticipantDev::GetSubscriber(m_participant)->lookup_datareader(Topic_name) == nullptr)
		{
			std::cerr << "listenDataReceive : " << Topic_name << " datareader not created." << std::endl;
			return false;
		}

		m_dataReaderListener[Topic_name]->SetDataReceiveListener(listener); // 监听者增加新的回调
		std::cout << Topic_name << " ListenDataReceive." << std::endl;
		return true;
	}

	// TODO
	template <class T, class D, class U, class L>
	bool DataDistribution<T, D, U, L>::UnlistenDataReceive(const char *Topic_name, Sample::DataReceiveListenerBase *listener)
	{
		if (!listener) // 空监听者直接返回，此处这个监听者是自己实现的
		{
			std::cout << "False RCComp::DataReceiveListener" << std::endl;
			return false;
		}

		if (Sample::ParticipantDev::GetSubscriber(m_participant)->lookup_datareader(Topic_name) == nullptr)
		{
			std::cerr << "UnListenDataReceive : " << Topic_name << " ERROR: datareader not created." << std::endl;
			return false;
		}

		m_dataReaderListener[Topic_name]->RemoveDataReceiveListener(listener);

		std::cout << Topic_name << " UnlistenDataReceive." << std::endl;

		return true;
	}

	// TODO
	template <class T, class D, class U, class L>
	bool DataDistribution<T, D, U, L>::SendDatas(const char *Topic_name, const std::vector<Data *> &data) { return false; }

	// 调用某个主题的数据写者的write方法
	template <class T, class D, class U, class L>
	bool DataDistribution<T, D, U, L>::SendData(const char *Topic_name, Data *data)
	{
		// 首先进行赋值
		T *_temp = dynamic_cast<T *>(data);
		//转化为fastdds的topic类型格式
		D _target = U::Data_to_DDS(*_temp);

		if (!Topic_name || strlen(Topic_name) == 0 || Topic_name == "") // 空字符串直接返回
		{
			std::cerr << "ERROR: Empty Topic Name" << std::endl;
			return false;
		}
		if (data == nullptr) // 空数据直接返回
		{
			std::cerr << "ERROR: Empty data" << std::endl;
			return false;
		}

		eprosima::fastdds::dds::DataWriter *base_dr = Sample::ParticipantDev::GetPublisher(m_participant)->lookup_datawriter(Topic_name);
		if (base_dr == nullptr)
		{
			std::cerr << "ERROR: Empty DataWriter " << std::endl;
			return false;
		}
		bool ret = false;
		ret = (eprosima::fastdds::dds::RETCODE_OK == base_dr->write(&_target));

		if (!ret)
		{
			std::cerr << "ERROR: DataWriter returned error." << std::endl;
		}
		else
		{
			std::cout << "DataWriter success send " << Topic_name << " data." << std::endl;
			return true;
		}

		return false;
	}

}
#endif // DATASTANDARDIMPL_H