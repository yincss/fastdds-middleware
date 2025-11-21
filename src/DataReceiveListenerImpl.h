#ifndef DATARECEIVELISTENERIMPL
#define DATARECEIVELISTENERIMPL

#include <fastdds/dds/subscriber/DataReaderListener.hpp>

#include "DataDistributionAPI.h"
#include "DataStandard.h"


namespace Sample {

    //使用该类是标准的修改dds回调函数的方法，即重写DDS::DataReaderListener
    //目前仅修改了on_data_available函数，表示收到数据之后的处理，其他函数均为空实现
    //该类用于封装，用户修改的是DataReceiveListenerImpl类
    //fastdds对应的是 eprosima::fastdds::dds::DataReaderListener

        class DataReceiveListenerBase;

        template <class T> class DataReceiveListenerImpl : public eprosima::fastdds::dds::DataReaderListener
        {
        public:
            //Constructor
            //为了配合订阅者对该类进行封装，使用人员只需要构建DataReceiveListener再把这个类对象传进来就可以激活自己写的回调函数
            DataReceiveListenerImpl();
            void SetDataReceiveListener(Sample::DataReceiveListenerBase* dataReceiveListener);
            int RemoveDataReceiveListener(Sample::DataReceiveListenerBase* dataReceiveListener);

            //Destructor
            virtual ~DataReceiveListenerImpl(void);

        public:
            virtual void on_requested_deadline_missed(
                eprosima::fastdds::dds::DataReader* reader,
                const eprosima::fastdds::dds::RequestedDeadlineMissedStatus& status);

            virtual void on_requested_incompatible_qos(
                eprosima::fastdds::dds::DataReader* reader,
                const eprosima::fastdds::dds::RequestedIncompatibleQosStatus& status);

            virtual void on_sample_rejected(
                eprosima::fastdds::dds::DataReader* reader,
                const eprosima::fastdds::dds::SampleRejectedStatus& status);

            virtual void on_data_available(
                eprosima::fastdds::dds::DataReader* reader) = 0; //接收数据回调函数

            virtual void on_subscription_matched(
                eprosima::fastdds::dds::DataReader* reader,
                const eprosima::fastdds::dds::SubscriptionMatchedStatus& info);

            virtual void on_sample_lost(
                eprosima::fastdds::dds::DataReader* reader,
                const eprosima::fastdds::dds::SampleLostStatus& status);

        protected:
            //存放用户自己实现的回调函数
            std::set<Sample::DataReceiveListenerBase*> m_dataReceiveListeners;
        };


        // Implementation skeleton constructor
        template <class T>
        DataReceiveListenerImpl<T>::DataReceiveListenerImpl()
        {
        }

        template <class T>
        void DataReceiveListenerImpl<T>::SetDataReceiveListener(Sample::DataReceiveListenerBase* dataReceiveListener)
        {
            m_dataReceiveListeners.insert(dataReceiveListener);
        }

        template <class T>
        int DataReceiveListenerImpl<T>::RemoveDataReceiveListener(Sample::DataReceiveListenerBase* dataReceiveListener)
        {
            m_dataReceiveListeners.erase(dataReceiveListener);
            return m_dataReceiveListeners.size();
        }

        template <class T>
        // Implementation skeleton destructor
        DataReceiveListenerImpl<T>::~DataReceiveListenerImpl()
        {
            m_dataReceiveListeners.clear();
        }

        template <class T>
        void DataReceiveListenerImpl<T>::on_requested_deadline_missed(
            eprosima::fastdds::dds::DataReader* /*reader*/,
            const eprosima::fastdds::dds::RequestedDeadlineMissedStatus& /*status*/)
        {
        }

        template<class T>
        inline void DataReceiveListenerImpl<T>::on_requested_incompatible_qos(eprosima::fastdds::dds::DataReader* reader,
            const eprosima::fastdds::dds::RequestedIncompatibleQosStatus& status)
            {
            }

            template<class T>
        inline void DataReceiveListenerImpl<T>::on_sample_rejected(
            eprosima::fastdds::dds::DataReader* reader,
            const eprosima::fastdds::dds::SampleRejectedStatus& status)
        {
        }
       
        template<class T>
        inline void DataReceiveListenerImpl<T>::on_subscription_matched(
            eprosima::fastdds::dds::DataReader* reader,
            const eprosima::fastdds::dds::SubscriptionMatchedStatus& info)
            {
            }

            template<class T>
            inline void DataReceiveListenerImpl<T>::on_sample_lost(
                eprosima::fastdds::dds::DataReader* reader,
                const eprosima::fastdds::dds::SampleLostStatus& status)
            {
            }

}


#endif // !DATARECEIVELISTENERIMPL
