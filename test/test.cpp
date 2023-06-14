//
// Created by cnh on 6/6/23.
//

#include "test.h"
#include "backend.h"
#include <algorithm>
#include <future>
#include <gtest/gtest.h>
#include <thread>
#include <zmq.hpp>
#include <zmq_addon.hpp>

void PublisherThread(zmq::context_t &ctx, bool &keep_running) {
    //  Prepare publisher
    zmq::socket_t publisher(*ctx, zmq::socket_type::pub);
    publisher.bind("inproc://#1");

    // Give the subscribers a chance to connect, so they don't lose any messages
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    while (keep_running) {
        //  Write three messages, each with an envelope and content
        publisher.send(zmq::str_buffer("A"), zmq::send_flags::sndmore);
        publisher.send(zmq::str_buffer("Message in A envelope"));
        publisher.send(zmq::str_buffer("B"), zmq::send_flags::sndmore);
        publisher.send(zmq::str_buffer("Message in B envelope"));
        publisher.send(zmq::str_buffer("C"), zmq::send_flags::sndmore);
        publisher.send(zmq::str_buffer("Message in C envelope"));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

TEST(backendTests, createInstance) {
    backend b;
    ASSERT_DOUBLE_EQ(0.0, b.value);
}

TEST(backendTests, async) {
    std::vector<std::future<void>> tasks;
    for (int i = 0; i < 100; i++) {
        tasks.push_back(std::async([] {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            std::cout << ".";
        }));
    }
    for (auto &task: tasks) {
        auto result = task.wait_for(std::chrono::seconds(1));
        ASSERT_EQ(result, std::future_status::ready);
    }
    std::cout << std::endl;
}

TEST(backendTests, simplePubsub) {
    zmq::context_t ctx;
    zmq::socket_t sock(ctx, zmq::socket_type::pub);
    sock.bind("inproc://test");
    sock.send(zmq::str_buffer("Hello, world"), zmq::send_flags::dontwait);
    auto pubTask = std::async([&ctx](){
        zmq::socket_t subSocket(ctx, zmq::socket_type::sub);
        zmq::message_t msg;
        subSocket.set(zmq::sockopt::subscribe, "topic1");
        auto result = subSocket.recv(msg,zmq::recv_flags::none);
        assert(result.has_value());
        ASSERT_EQ(result.value(),"");
        return std::string{};
    });
}

TEST(backendTests, actualPubSub){
    zmq::context_t ctx;
    bool keep_running = true;
    auto task = std::async(std::launch::async, [&](){
        return PublisherThread(ctx, keep_running);
    });
    //  Prepare subscriber
    zmq::socket_t subscriber(*ctx, zmq::socket_type::sub);
    subscriber.connect("inproc://#1");

    //  Thread2 opens "A" and "B" envelopes
    subscriber.set(zmq::sockopt::subscribe, "A");
    subscriber.set(zmq::sockopt::subscribe, "B");

    while (1) {
        // Receive all parts of the message
        std::vector<zmq::message_t> recv_msgs;
        zmq::recv_result_t result =
                zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
        assert(result && "recv failed");
        assert(*result == 2);

        std::cout << "Thread2: [" << recv_msgs[0].to_string() << "] "
                  << recv_msgs[1].to_string() << std::endl;
    }
    keep_running = false;
}