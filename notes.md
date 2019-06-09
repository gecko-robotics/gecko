using send_result_t = std::optional<size_t>;
using recv_result_t = std::optional<size_t>;
using recv_buffer_result_t = std::optional<recv_buffer_size>;


```
watch -n 1 'ps -o pid,%mem,%cpu,comm -p 36879'
```

https://www.boost.org/doc/libs/1_64_0/doc/html/boost_process/tutorial.html


```cpp
string hn = boost::asio::ip::host_name();
```

```cpp
#include <boost/serialization/string.hpp>

#include <b0/b0.h>
#include <b0/message/message.h>

namespace b0
{

namespace message
{

namespace resolv
{

/*!
 * \brief Sent by a ServiceClient to resolve a service name to a ZeroMQ address
 *
 * \mscfile service-resolve.msc
 *
 * \sa ResolveServiceResponse, \ref protocol
 */
class ResolveServiceRequest : public Message
{
public:
    //! The name of the service to be resolved
    std::string service_name;

public:
    std::string type() const override {return "b0.message.resolv.ResolveServiceRequest";}
};

} // namespace resolv

} // namespace message

} // namespace b0

//! \cond HIDDEN_SYMBOLS

namespace spotify
{

namespace json
{

using b0::message::resolv::ResolveServiceRequest;

template <>
struct default_codec_t<ResolveServiceRequest>
{
    static codec::object_t<ResolveServiceRequest> codec()
    {
        auto codec = codec::object<ResolveServiceRequest>();
        codec.required("service_name", &ResolveServiceRequest::service_name);
        return codec;
    }
};

} // namespace json

} // namespace spotify
```

```cpp
//! \brief Alias for function
template<typename T> using function = boost::function<T>;

//! \brief Alias for callback raw without type
using CallbackRaw = function<void(const std::string&, std::string&)>;

//! \brief Alias for callback raw with type
using CallbackRawType = function<void(const std::string&, const std::string&, std::string&, std::string&)>;

//! \brief Alias for callback raw message parts
using CallbackParts = function<void(const std::vector<b0::message::MessagePart>&, std::vector<b0::message::MessagePart>&)>;

//! \brief Alias for callback message class
template<class TReq, class TRep> using CallbackMsg = function<void(const TReq&, TRep&)>;

//! \brief Alias for callback message class + raw extra parts
template<class TReq, class TRep> using CallbackMsgParts = function<void(const TReq&, const std::vector<b0::message::MessagePart>&, TRep&, std::vector<b0::message::MessagePart>&)>;

```

When geckocpp publishes topics, it tracks them. When it exits, it broadcasts a shutdown command

OR maybe better, when a geckocore notices the pid is no longer valid, it pulls that topic from
its DB of published topics

```cpp
// grab from env, use def (default) if not found
std::string get(const std::string &var, const std::string &def)
{
    std::string ret = def;
    const char *v = std::getenv(var.c_str());
    if(v) ret = v;
    return ret;
}

bool getBool(const std::string &var, bool def)
{
    bool ret = def;
    std::string val = get(var), vlc = boost::algorithm::to_lower_copy(val);
    if(vlc == "");
    else if(vlc == "true"  || vlc == "yes" || vlc == "on"  || vlc == "1") ret = true;
    else if(vlc == "false" || vlc == "no"  || vlc == "off" || vlc == "0") ret = false;
    else throw exception::ArgumentError(val, var);
    return ret;
}

int getInt(const std::string &var, int def)
{
    int ret = def;
    std::string val = get(var);
    if(val != "") ret = std::atoi(val.c_str());
    return ret;
}

double getDouble(const std::string &var, double def)
{
    double ret = def;
    std::string val = get(var);
    if(val != "") ret = std::atof(val.c_str());
    return ret;
}
```

```cpp
void ServiceServer::spinOnce()
{
    if(!callback_ && !callback_with_type_ && !callback_multipart_) return;

    while(poll())
    {
        if(callback_)
        {
            std::string req, rep;
            readRaw(req);
            callback_(req, rep);
            writeRaw(rep);
        }
        if(callback_with_type_)
        {
            std::string req, reqtype, rep, reptype;
            readRaw(req, reqtype);
            callback_with_type_(req, reqtype, rep, reptype);
            writeRaw(rep, reptype);
        }
        if(callback_multipart_)
        {
            std::vector<b0::message::MessagePart> reqparts, repparts;
            readRaw(reqparts);
            callback_multipart_(reqparts, repparts);
            writeRaw(repparts);
        }
    }
}
```

```cpp
int Node::freeTCPPort()
{
    // by binding the OS socket to port 0, an available port number will be used
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), 0);
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket(io_service, ep);
    return socket.local_endpoint().port();
}
```

```
========================================
 Geckocore [38588]
-------------
 Key: local
 Host IP: 10.0.1.70
 Listening on: 224.3.29.110:11311
-------------
Known Services [4]
 * ryan:......................... tcp://10.0.1.70:63033
 * mike:......................... tcp://10.0.1.70:63034
 * sammie:....................... tcp://10.0.1.70:63035
 * scott:........................ tcp://10.0.1.70:63036

Binders [4]
 [38604] ryan.................. cpu:  0.1%  mem:  0.0%
 [38606] mike.................. cpu:  0.1%  mem:  0.0%
 [38608] sammie................ cpu:  0.1%  mem:  0.0%
 [38610] scott................. cpu:  0.1%  mem:  0.0%

Connections [4]
 [38607] mike.................. cpu: 19.6%  mem:  0.0%
 [38611] scott................. cpu: 19.6%  mem:  0.0%
 [38605] ryan.................. cpu: 19.7%  mem:  0.0%
 [38609] sammie................ cpu: 19.8%  mem:  0.0%
 ```
