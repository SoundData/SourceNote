#include "DataProvider.h"

#include <iostream>

DataProvider::DataProvider(std::mutex& mutex, std::queue<GameMessage*>& q, const char* address)
	:mutex_(mutex),
	msgQueue_(q),
	ctx_(1),
	sock_(ctx_, ZMQ_SUB),
	address_(address)
{
	// Do nothing
}

DataProvider::~DataProvider()
{
	// Shut down network connections and delete the listener thread
	this->sock_.close();
	this->ctx_.close();
	delete listenerThread_;
}

void DataProvider::BeginListening()
{
	// Since we are a subscription socket, we must specify at least one filter rule
	// which will match messages which are of interest to us
	// All messages are of interest, so make a blank filter
	sock_.setsockopt(ZMQ_SUBSCRIBE, "", 0);
	sock_.connect(address_);
	// Spawn a thread which will asynchronously write to the queue
	listenerThread_ = new std::thread(&DataProvider::Listen, this);
}

void DataProvider::Listen()
{
	std::cout << "Listening for events from " << address_ << std::endl;
	while (true)
	{
		// Block until message received
		zmq::message_t msg = RetrieveMessage();

		// Attempt to access queue (wait a bit it's already accessed by consumer thread)
		bool access = false;
		while (!access)
		{
			access = mutex_.try_lock();
			if (!access)
			{
				// sleep for a while, after 25ms or so the main thread should be done processing
				std::this_thread::sleep_for(std::chrono::milliseconds(MSEC_QUEUE_ADD_DELAY));
			}
		}

		// Add received message to the message queue
		AddMessageToQueue(msg);

		// Release mutex lock
		mutex_.unlock();
	}
}

zmq::message_t DataProvider::RetrieveMessage()
{
	// retrieve an event from the ZMQ socket (blocking)
	zmq::message_t msg(MAX_MESSAGE_SIZE);

	sock_.recv(&msg);
	return msg;
}

void DataProvider::AddMessageToQueue(const zmq::message_t& msg)
{
	// Store the message into the synchronized message queue
	char buf[MAX_MESSAGE_SIZE];

	size_t msgLen = strnlen(reinterpret_cast<const char*>(msg.data()), MAX_MESSAGE_SIZE);
	assert(msgLen < MAX_MESSAGE_SIZE);
	
	strncpy(buf, reinterpret_cast<const char*>(msg.data()), msgLen);
	buf[msgLen] = '\0';

	// Form a new GameMessage
	// Its destructor is called from main() only after it has been processed
	GameMessage *gameMessage = new GameMessage(buf, msgLen);
	msgQueue_.push(gameMessage);
}