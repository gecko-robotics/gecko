// // #pragma once
// // #include "transport.hpp"
//
// using namespace std;
//
//
// template<class msg>
// zmq::message_t Transport<msg>::pack(msg& m){
//     stringstream ss;
//     msgpack::pack(ss, m);
//
//     cout << "uncompressed string size: " << ss.str().size() << endl;
//
//     string cs = compress_string(ss.str(), compression_level);
//     cout << "compressed string size: " << cs.size() << endl;
//
//     // zmq::message_t zm(static_cast<void*>(ss.str().data()), ss.str().size());
//     zmq::message_t zm(reinterpret_cast<void*>(cs.data()), cs.size());
//     cout << "compressed message_t size: " << zm.size() << endl;
//     cout << "pack addr: " << &zm << endl;
//     return zm;
// }
//
// template<class msg>
// void Transport<msg>::unpack(const zmq::message_t& zm, std::vector<msg>& recv){
//     string cs(reinterpret_cast<const char*>(zm.data()), zm.size());
//     string ucs = decompress_string(cs);
//     stringstream ss(ucs);
//     unpack(ss, recv);
// }
//
// template<class msg>
// void Transport<msg>::unpack(const std::stringstream& ss, std::vector<msg>& recv){
//     size_t offset = 0;
//     try {
//         while((ss.str().size() - offset) > 0){
//             cout << "buffer: " << ss.str().size() << "  offset: " << offset << endl;
//             msgpack::object_handle oh = msgpack::unpack(ss.str().data(), ss.str().size(), offset);
//             msgpack::object obj = oh.get();
//
//             msg e;
//             obj.convert(e);
//             recv.push_back(e);
//         }
//     }
//     // catch (msgpack::v3::insufficient_bytes& e)
//     catch (const std::exception &e){
//         cout << "" << e.what() << endl;
//     }
// }
//
// template<class msg>
// msg Transport<msg>::unpack(const zmq::message_t& zm){
//     string cs(reinterpret_cast<const char*>(zm.data()), zm.size());
//     string ucs = decompress_string(cs);
//     stringstream ss(ucs);
//     return unpack(ss);
// }
//
// template<class msg>
// msg Transport<msg>::unpack(const std::stringstream& ss){
//     size_t offset = 0;
//     msg e;
//     try {
//         cout << "buffer: " << ss.str().size() << "  offset: " << offset << endl;
//         msgpack::object_handle oh = msgpack::unpack(ss.str().data(), ss.str().size(), offset);
//         msgpack::object obj = oh.get();
//         obj.convert(e);
//     }
//     // catch (msgpack::v3::insufficient_bytes& e)
//     catch (const std::exception &e){
//         cout << "" << e.what() << endl;
//     }
//     return e;
// }
//
// /** Compress a STL string using zlib with given compression level and return
// the binary data.
//
// https://panthema.net/2007/0328-ZLibString.html
//
// https://zlib.net/manual.html
// #define Z_NO_COMPRESSION         0
// #define Z_BEST_SPEED             1
// #define Z_BEST_COMPRESSION       9
// #define Z_DEFAULT_COMPRESSION  (-1)
// */
// template<class msg>
// std::string Transport<msg>::compress_string(const std::string& str, const int level)
// {
//     z_stream zs;  // z_stream is zlib's control structure
//     memset(&zs, 0, sizeof(zs));
//
//     if (deflateInit(&zs, level) != Z_OK)
//         throw(std::runtime_error("deflateInit failed while compressing."));
//
//     zs.next_in = (Bytef*)str.data();
//     zs.avail_in = str.size();  // set the z_stream's input
//
//     int ret;
//     char outbuffer[32768];
//     std::string outstring;
//
//     // retrieve the compressed bytes blockwise
//     do {
//         zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
//         zs.avail_out = sizeof(outbuffer);
//
//         ret = deflate(&zs, Z_FINISH);
//
//         if (outstring.size() < zs.total_out) {
//             // append the block to the output string
//             outstring.append(outbuffer, zs.total_out - outstring.size());
//         }
//     } while (ret == Z_OK);
//
//     deflateEnd(&zs);
//
//     if (ret != Z_STREAM_END) {  // an error occurred that was not EOF
//         std::ostringstream oss;
//         oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
//         throw(std::runtime_error(oss.str()));
//     }
//
//     return outstring;
// }
//
// /** Decompress an STL string using zlib and return the original data. */
// template<class msg>
// std::string Transport<msg>::decompress_string(const std::string& str)
// {
//     z_stream zs;  // z_stream is zlib's control structure
//     memset(&zs, 0, sizeof(zs));
//
//     if (inflateInit(&zs) != Z_OK)
//         throw(std::runtime_error("inflateInit failed while decompressing."));
//
//     zs.next_in = (Bytef*)str.data();
//     zs.avail_in = str.size();
//
//     int ret;
//     static char outbuffer[32768];  // FIXME: need better solution
//     std::string outstring;
//
//     // get the decompressed bytes blockwise using repeated calls to inflate
//     do {
//         zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
//         zs.avail_out = sizeof(outbuffer);
//
//         ret = inflate(&zs, 0);
//
//         if (outstring.size() < zs.total_out) {
//             outstring.append(outbuffer, zs.total_out - outstring.size());
//         }
//
//     } while (ret == Z_OK);
//
//     inflateEnd(&zs);
//
//     if (ret != Z_STREAM_END) {  // an error occurred that was not EOF
//         std::ostringstream oss;
//         oss << "Exception during zlib decompression: (" << ret << ") "
//             << zs.msg;
//         throw(std::runtime_error(oss.str()));
//     }
//
//     return outstring;
// }
