#pragma once

#include <msgpack.hpp>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include "msgs.hpp"
#include "zmq.hpp"
#include <zlib.h>


template<class msg>
class Transport {
public:
    Transport():compression_level(Z_BEST_SPEED) {}
    int compression_level;

    zmq::message_t pack(msg& m);
    std::string pack_str(msg& m);

    // unpack an array of messages
    void unpack(const zmq::message_t& zm, std::vector<msg>& recv);
    void unpack(const std::stringstream& ss, std::vector<msg>& recv);

    // unpack a single message, assumption is there is only 1 valid message
    // to recover in the buffer
    msg unpack(const zmq::message_t& zm);
    msg unpack(const std::stringstream& ss);

    /*******************
    Compress a STL string using zlib with given compression level and return
    the binary data.

    https://panthema.net/2007/0328-ZLibString.html

    https://zlib.net/manual.html
    #define Z_NO_COMPRESSION         0
    #define Z_BEST_SPEED             1
    #define Z_BEST_COMPRESSION       9
    #define Z_DEFAULT_COMPRESSION  (-1)
    ****/
    std::string compress_string(const std::string& str);

    // Decompress an STL string using zlib and return the original data.
    std::string decompress_string(const std::string& str);
};


template<class msg>
zmq::message_t Transport<msg>::pack(msg& m){
    std::stringstream ss;
    msgpack::pack(ss, m);

    // std::cout << "uncompressed string size: " << ss.str().size() << std::endl;

    std::string cs = compress_string(ss.str());
    // std::cout << "compressed string size: " << cs.size() << std::endl;

    // zmq::message_t zm(static_cast<void*>(ss.str().data()), ss.str().size());
    zmq::message_t zm(reinterpret_cast<void*>(cs.data()), cs.size());
    // std::cout << "compressed message_t size: " << zm.size() << std::endl;
    // std::cout << "pack addr: " << &zm << std::endl;
    return zm;
}

template<class msg>
std::string Transport<msg>::pack_str(msg& m){
    std::stringstream ss;
    msgpack::pack(ss, m);

    // std::cout << "uncompressed string size: " << ss.str().size() << std::endl;

    std::string cs = compress_string(ss.str());

    return cs;
}

template<class msg>
void Transport<msg>::unpack(const zmq::message_t& zm, std::vector<msg>& recv){
    std::string cs(reinterpret_cast<const char*>(zm.data()), zm.size());
    std::string ucs = decompress_string(cs);
    std::stringstream ss(ucs);
    unpack(ss, recv);
}

template<class msg>
void Transport<msg>::unpack(const std::stringstream& ss, std::vector<msg>& recv){
    size_t offset = 0;
    try {
        while((ss.str().size() - offset) > 0){
            // std::cout << "buffer: " << ss.str().size() << "  offset: " << offset << std::endl;
            msgpack::object_handle oh = msgpack::unpack(ss.str().data(), ss.str().size(), offset);
            msgpack::object obj = oh.get();

            msg e;
            obj.convert(e);
            recv.push_back(e);
        }
    }
    // catch (msgpack::v3::insufficient_bytes& e)
    catch (const std::exception &e){
        std::cout << "" << e.what() << std::endl;
    }
}

template<class msg>
msg Transport<msg>::unpack(const zmq::message_t& zm){
    std::string cs(reinterpret_cast<const char*>(zm.data()), zm.size());
    std::string ucs = decompress_string(cs);
    std::stringstream ss(ucs);
    return unpack(ss);
}

template<class msg>
msg Transport<msg>::unpack(const std::stringstream& ss){
    size_t offset = 0;
    msg e;
    try {
        // std::cout << "buffer: " << ss.str().size() << "  offset: " << offset << std::endl;
        msgpack::object_handle oh = msgpack::unpack(ss.str().data(), ss.str().size(), offset);
        msgpack::object obj = oh.get();
        obj.convert(e);
    }
    // catch (msgpack::v3::insufficient_bytes& e)
    catch (const std::exception &e){
        std::cout << "" << e.what() << std::endl;
    }
    return e;
}

/** Compress a STL string using zlib with given compression level and return
the binary data.

https://panthema.net/2007/0328-ZLibString.html

https://zlib.net/manual.html
#define Z_NO_COMPRESSION         0
#define Z_BEST_SPEED             1
#define Z_BEST_COMPRESSION       9
#define Z_DEFAULT_COMPRESSION  (-1)
*/
template<class msg>
std::string Transport<msg>::compress_string(const std::string& str)
{
    z_stream zs;  // z_stream is zlib's control structure
    memset(&zs, 0, sizeof(zs));

    if (deflateInit(&zs, compression_level) != Z_OK)
        throw(std::runtime_error("deflateInit failed while compressing."));

    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size();  // set the z_stream's input

    int ret;
    char outbuffer[32768];
    std::string outstring;

    // retrieve the compressed bytes blockwise
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = deflate(&zs, Z_FINISH);

        if (outstring.size() < zs.total_out) {
            // append the block to the output string
            outstring.append(outbuffer, zs.total_out - outstring.size());
        }
    } while (ret == Z_OK);

    deflateEnd(&zs);

    if (ret != Z_STREAM_END) {  // an error occurred that was not EOF
        std::ostringstream oss;
        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    }

    return outstring;
}

// Decompress an STL string using zlib and return the original data.
template<class msg>
std::string Transport<msg>::decompress_string(const std::string& str)
{
    z_stream zs;  // z_stream is zlib's control structure
    memset(&zs, 0, sizeof(zs));

    if (inflateInit(&zs) != Z_OK)
        throw(std::runtime_error("inflateInit failed while decompressing."));

    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size();

    int ret;
    static char outbuffer[32768];  // FIXME: need better solution
    std::string outstring;

    // get the decompressed bytes blockwise using repeated calls to inflate
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = inflate(&zs, 0);

        if (outstring.size() < zs.total_out) {
            outstring.append(outbuffer, zs.total_out - outstring.size());
        }

    } while (ret == Z_OK);

    inflateEnd(&zs);

    if (ret != Z_STREAM_END) {  // an error occurred that was not EOF
        std::ostringstream oss;
        oss << "Exception during zlib decompression: (" << ret << ") "
            << zs.msg;
        throw(std::runtime_error(oss.str()));
    }

    return outstring;
}

// does this do anything of value???
// template class Transport<imu_t>;
// template class Transport<Pose>;
// template class Transport<PoseStamped>;


// #include "transport.cpp"
