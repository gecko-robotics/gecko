// https://msgpack.org/

#include <msgpack.h>
#include <stdio.h>
#include <assert.h>
#include <ctime>
#include <sys/time.h>
#include <vector>

using namespace std;

enum GeckoMsgs {
    GVECTOR,
    GQUATERNION,
    GIMU,
    GWRENCH,
    GTWIST,
    GLIDAR,
    GIMAGE,
    GPOSE
};

/**
 * Base class for keeping track of message type
 */
class base_t {
public:
    base_t(uint8_t t): type(t) {}
    const uint8_t type;
};

class vec_t: public base_t {
public:
    vec_t(): base_t(GVECTOR) {}
    vec_t(vec_t& a): base_t(GVECTOR) {
        x = a.x;
        y = a.y;
        z = a.z;
    }
    vec_t(double a, double b, double c): base_t(GVECTOR){
        x = a;
        y = b;
        z = c;
    }
    // double operator[](uint8_t i){
    //     if(i == 0) return x;
    //     else if (i == 1) return y;
    //     else if (i == 2) return z;
    //     else printf("invalide: %d", i);
    //     return 0;
    // }
    // double& operator[](int i){
    //     if(i == 0) return x;
    //     else if (i == 1) return y;
    //     else if (i == 2) return z;
    //     else printf("invalide: %d", i);
    //     return x;
    // }

    double x,y,z;

    void pack(msgpack_packer* pk){
        msgpack_pack_array(pk, 4);
        msgpack_pack_uint8(pk, type);
        msgpack_pack_double(pk, x);
        msgpack_pack_double(pk, y);
        msgpack_pack_double(pk, z);
    }

    void unpack(const msgpack_object& obj){
        msgpack_object_array a = static_cast<msgpack_object_array>(obj.via.array);
        unpack(a);
    }

    void unpack(const msgpack_object_array& a){
        x = (double)a.ptr[1].via.f64;
        y = (double)a.ptr[2].via.f64;
        z = (double)a.ptr[3].via.f64;
    }
    // vec_t& operator=(vec_t&& a) noexcept {
    //     x=a.x;
    //     y=a.y;
    //     z=a.z;
    //     return *this;
    // }
    // const uint8_t type;
    void print(char end='\n'){
        printf(" [%f %f %f]%c", x, y, z,end);
    }
};

class quaternion_t: public base_t {
public:
    quaternion_t(): base_t(GQUATERNION) {}
    double w,x,y,z;
};

class msg_t {
public:
    msg_t(): timestamp(time_now()) {}
    double time_now(){
        struct timeval ts;
        gettimeofday(&ts, NULL);
        return ((double)(ts.tv_sec) + 1e-6*(ts.tv_usec));
    }
    double timestamp;
};

class imu_t: public base_t, public msg_t  {
public:
    imu_t(): base_t(GIMU) {}
    imu_t(vec_t a, vec_t g, vec_t m): base_t(GIMU), accel(a), gyro(g), mag(m) {}
    vec_t accel, gyro, mag;

    void print(){
        printf("IMU [%f]\n", timestamp);
        accel.print();
        gyro.print();
        mag.print();
    }

    void pack(msgpack_packer* pk){
        msgpack_pack_array(pk, 5);
        msgpack_pack_uint8(pk, type);
        accel.pack(pk);
        gyro.pack(pk);
        mag.pack(pk);
        msgpack_pack_double(pk, timestamp);
    }

    // shouldn't msgs ALWAYS be an object?
    void unpack(const msgpack_object& obj){
        msgpack_object_array a = static_cast<msgpack_object_array>(obj.via.array);
        unpack(a);
    }

    void unpack(const msgpack_object_array& a){
        accel.unpack(a.ptr[1].via.array);
        gyro.unpack(a.ptr[2].via.array);
        mag.unpack(a.ptr[3].via.array);
        timestamp = (double)a.ptr[4].via.f64;
    }
};

class Twist: public base_t, public msg_t {
public:
    Twist(): base_t(GTWIST) {}
    vec_t linear, angular;
};

class Pose: public base_t, public msg_t {
public:
    Pose(): base_t(GPOSE) {}
    vec_t position;
    quaternion_t orientation;
};

class Lidar: public base_t, public msg_t {
public:
    Lidar(): base_t(GLIDAR) {}
    std::vector<double> scan;
};

///////////////////////////////////////////////////////////

// #define UNPACKED_BUFFER_SIZE 2048

// class Packer {
// public:
//     Packer(){
//         msgpack_sbuffer_init(&sbuf);
//         msgpack_packer_init(&pk, sbuf, msgpack_sbuffer_write);
//     }
//     msgpack_sbuffer sbuf;
//     msgpack_packer pk;
// }
template<typename message>
class sbuffer {
public:
    sbuffer(){
        msgpack_sbuffer_init(&buf);
        msgpack_packer_init(&pk, &buf, msgpack_sbuffer_write);
    }

    // template<typename message>
    void pack(message& msg){
        msg.pack(&pk);
    }

    void zmq(){
        // pack this
    }

    msgpack_sbuffer buf;  // buf.data, buf.size -> zmq
    msgpack_packer pk;
};

template<typename buffer,typename message>
void pack_msg(buffer* sbuf, message& msg){
    msgpack_packer pk;
    msgpack_sbuffer_init(sbuf);
    msgpack_packer_init(&pk, sbuf, msgpack_sbuffer_write);
    msg.pack(&pk);
}

template<typename buffer,typename message>
void unpack_msg(buffer* buf, size_t len, message& msg) {
    // buf is allocated by client.
    msgpack_unpacked result;
    msgpack_unpacked_init(&result);

    size_t off = 0;
    msgpack_unpack_return ret;
    int i = 0;

    ret = msgpack_unpack_next(&result, buf->data, len, &off);

    while (ret == MSGPACK_UNPACK_SUCCESS) {
        msgpack_object obj = result.data;
        msg.unpack(obj);
        ret = msgpack_unpack_next(&result, buf->data, len, &off);
    }
    // // msgpack_object obj = result.data;
    // msg.unpack(obj);
    msgpack_unpacked_destroy(&result);

    if (ret == MSGPACK_UNPACK_CONTINUE) {
        printf("All msgpack_object in the buffer is consumed.\n");
    }
    else if (ret == MSGPACK_UNPACK_PARSE_ERROR) {
        printf("The data in the buf is invalid format.\n");
    }
}

int main(void) {
    msgpack_sbuffer sbuf;
    // msgpack_sbuffer_init(&sbuf);

    // prepare(&sbuf);
    vec_t a(1.2,3.4,-5.6789);
    vec_t b(-1.2,-3.4,5.6789);
    vec_t c(1e-6,2e3,3e9);

    imu_t d(a,b,c);
    pack_msg<msgpack_sbuffer,imu_t>(&sbuf, d);

    imu_t e;
    unpack_msg<msgpack_sbuffer,imu_t>(&sbuf, sbuf.size, e);
    printf("end\n");
    e.print();


    sbuffer<imu_t> buff;
    buff.pack(e);
    e.print();

    // printf(">> buffer is %zu\n", sbuf.size);

    // unpack(sbuf.data, sbuf.size);

    // printf(">> buffer is %zu\n", sbuf.size);

    msgpack_sbuffer_destroy(&sbuf);
    return 0;
}

// void prepare(msgpack_sbuffer* sbuf) {
//     vec_t a(1.2,3.4,-5.6789);
//     vec_t b(-1.2,-3.4,5.6789);
//     vec_t c(1e-6,2e3,3e9);
//
//     imu_t d(a,b,c);
//
//     msgpack_packer pk;
//     msgpack_packer_init(&pk, sbuf, msgpack_sbuffer_write);
//
//     a.pack(&pk);
//     d.pack(&pk);
// }


// void unpack(char const* buf, size_t len) {
//     // buf is allocated by client.
//     msgpack_unpacked result;
//     size_t off = 0;
//     msgpack_unpack_return ret;
//     int i = 0;
//     char unpacked_buffer[UNPACKED_BUFFER_SIZE];
//     msgpack_unpacked_init(&result);
//     ret = msgpack_unpack_next(&result, buf, len, &off);
//     while (ret == MSGPACK_UNPACK_SUCCESS) {
//         msgpack_object obj = result.data;
//
//         // Use obj.
//         printf("Object no %d:\n", ++i);
//         msgpack_object_print_buffer(unpacked_buffer, UNPACKED_BUFFER_SIZE, obj);
//         printf("%s\n", unpacked_buffer);
//
//         if(obj.type == MSGPACK_OBJECT_ARRAY){
//             msgpack_object_array a = obj.via.array;
//             uint8_t type = (uint8_t)a.ptr[0].via.u64;
//             if (type == GVECTOR) {
//                 vec_t r;
//                 r.unpack(obj);
//                 r.print();
//             }
//             else if (type == GIMU) {
//                 imu_t r;
//                 r.unpack(obj);
//                 r.print();
//             }
//         }
//
//         ret = msgpack_unpack_next(&result, buf, len, &off);
//     }
//     msgpack_unpacked_destroy(&result);
//
//     if (ret == MSGPACK_UNPACK_CONTINUE) {
//         printf("All msgpack_object in the buffer is consumed.\n");
//     }
//     else if (ret == MSGPACK_UNPACK_PARSE_ERROR) {
//         printf("The data in the buf is invalid format.\n");
//     }
// }
