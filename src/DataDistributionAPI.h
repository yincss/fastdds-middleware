#ifndef DATADISTRIBUTIONAPI
#define DATADISTRIBUTIONAPI
#include "DataDistributionConfig.h"
#include "ParticipantAPI.h"
#include "QosExt.h"
#include <vector>

namespace Sample
{
#ifndef SampleData
#define SampleData
    class Data
    {
    public:
        virtual ~Data() {}
    };
#endif

    // 接收数据回调函数类
    // TODO应该让开发人员可以使用该类简单的编写各主体的回调函数
    // 计划采用模板类的形式
    class DataReceiveListenerBase
    {
    public:
        virtual void OnDataReceive(const char *Topic_name, const Data *data) = 0;
        virtual ~DataReceiveListenerBase() {}

    private:
        DataReceiveListenerBase &operator=(DataReceiveListenerBase &) {}
    };

    // 数据交互类，负责主题的发布与订阅，数据的发送
    // 实现为模板类，大大减轻代码生成的负担
    // I/ImpI使用工厂模式对用户进行封装
    class DATADISTRIBUTIONCONFIG_API DataDistributionAPI
    {
    public:
        virtual bool PublishTopic(const char *Topic_name, QoS *qos = nullptr) = 0; // 发布主题
        virtual bool UnPublishTopic(const char *Topic_name) = 0;                   // TODO取消发布主题
        virtual bool SendData(const char *Topic_name, Data *data) = 0;             // 发送数据
        virtual bool SubscribeTopic(const char *Topic_name, QoS *qos = 0) = 0;     // 订阅主题
        virtual bool UnSubscribeTopic(const char *Topic_name) = 0;                 // TODO取消订阅主题
        virtual bool ListenDataReceive(const char *Topic_name, DataReceiveListenerBase *listener) = 0;
        virtual bool UnlistenDataReceive(const char *Topic_name, DataReceiveListenerBase *listener) = 0;
        virtual bool SendDatas(const char *Topic_name, const std::vector<Data *> &datas) = 0; // 发送多条数据

        // virtual Qos* MakePublisherQos(FitCreateQos_Publisher preCreationQos) = 0;
        // virtual bool SetPublisherQos(const char* Topic_name, FitRunQos_Publisher publisherQos) = 0;
        // virtual bool SetSubscriberQos(const char* Topic_name, FitRunQos_Subscriber subscriberQos) = 0;

        virtual ~DataDistributionAPI() {}

    private:
        DataDistributionAPI &operator=(DataDistributionAPI &_temp) { return _temp; }
    };

    // 工厂类，生成dataDistributionImpl
    // 缺点在于派生类无法对用户封装，因为每个数据类型对应一个数据交互类而用户需要自己知道自己要使用哪个数据交互类
    class DATADISTRIBUTIONCONFIG_API DataDistributionFactory
    {
    public:
        virtual DataDistributionAPI *CreateDataDistribution(ParticipantAPI *participantAPI) = 0; // 创建DataDistrib
        virtual ~DataDistributionFactory() {}
        static bool DeleteDataDistribution(DataDistributionAPI *dataDistribution);
    };
}
#endif // !DATADISTRIBUTIONAPI
