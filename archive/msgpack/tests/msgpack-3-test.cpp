

/*
https://github.com/jordansissel/experiments/blob/master/c/zeromq/msgpack-zeromq.c
msgpack_sbuffer* buffer = msgpack_sbuffer_new();
zmq_msg_t message;
//printf("%zd: %.*s\n", buffer->size, buffer->size, buffer->data);
zmq_msg_init_data(&message, buffer->data, buffer->size, NULL, NULL);
rc = zmq_send(socket, &message, 0);
*/

// https://gist.github.com/mashiro/5150508
#include <thread>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <zmq.hpp>
#include <msgpack.hpp>

struct message
{
  std::string tag;
  std::time_t time;
  std::string text;
  MSGPACK_DEFINE(tag, time, text);
};

template <typename T>
void subscribe(zmq::socket_t& socket, T& data)
{
  zmq::message_t tag_msg, body_msg;
  socket.recv(&tag_msg);
  socket.recv(&body_msg);

  const std::string tag(static_cast<const char*>(tag_msg.data()), tag_msg.size());

  msgpack::unpacked unpacked_body;
  msgpack::unpack(&unpacked_body, static_cast<const char*>(body_msg.data()), body_msg.size());
  unpacked_body.get().convert(&data);
}

void sub()
{
  zmq::context_t context(1);
  zmq::socket_t socket(context, ZMQ_SUB);
  socket.connect("tcp://127.0.0.1:12900");

  const std::string tag = "msg";
  socket.setsockopt(ZMQ_SUBSCRIBE, tag.c_str(), tag.size());

  while (true)
  {
    message msg;
    subscribe(socket, msg);
    std::cout << msg.time << " [" << msg.tag << "] " << msg.text << std::endl;
  }
}

template <typename T>
void publish(zmq::socket_t& socket, const std::string& tag, const T& data)
{
  msgpack::sbuffer packed;
  msgpack::pack(&packed, data);

  zmq::message_t tag_msg(tag.size());
  std::memcpy(tag_msg.data(), tag.data(), tag.size());

  zmq::message_t body_msg(packed.size());
  std::memcpy(body_msg.data(), packed.data(), packed.size());

  socket.send(tag_msg, ZMQ_SNDMORE);
  socket.send(body_msg);
}

void pub()
{
  zmq::context_t context(1);
  zmq::socket_t socket(context, ZMQ_PUB);
  socket.bind("tcp://127.0.0.1:12900");

  const std::string tag = "msg";
  while (true)
  {
    const std::time_t now = std::time(0);

    message msg;
    msg.tag = tag;
    msg.time = now;
    msg.text = "hello";

    publish(socket, tag, msg);

    std::cout << now << std::endl;
    sleep(1);
  }
}

int main(){

    std::cout << "start" << std::endl;
    std::thread t1(sub);
    std::thread t2(pub);
    t1.join();
    t2.join();

    return 0;
}
