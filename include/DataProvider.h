#pragma once

//
// DataProvider.h
// 
// A calling function will instantiate a DataProvider if it wants to retrieve data
// from a remote service.
// The BeginListening() call tells the DataProvider to attempt to connect to a ZMQ
// socket specified in its constructor.
//
// The constructor also takes a queue<GameMessage*> and a mutex:
// The DataProvider stores messages received from the network in the queue,
// where they are then processed by the calling function.


#include "GameMessage.h"
#include "zmq.hpp"
#include <thread>
#include <mutex>
#include <queue>

#define MSEC_QUEUE_ADD_DELAY 25 // Wait 25 ms before attemping queue access

// Provides data by subscribing to server data publisher and asynchronously inserting
// messages it receives into the queue
class DataProvider
{
public:
	DataProvider(std::mutex& mutex, std::queue<GameMessage*>& q, const char *address);
	void BeginListening();
	~DataProvider();
private:
	std::mutex& mutex_; // Reference to mutex protecting MessageQueue
	std::queue<GameMessage*>& msgQueue_; // Reference to MessageQueue
	std::thread *listenerThread_;
	zmq::context_t ctx_;
	zmq::socket_t sock_;
	const char *address_;

private:
	void Listen();
	zmq::message_t RetrieveMessage();
	void AddMessageToQueue(const zmq::message_t&);
};