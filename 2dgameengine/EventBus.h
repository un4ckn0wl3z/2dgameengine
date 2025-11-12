#pragma once
#include "Logger.h"
#include <map>
#include <typeindex>
#include <list>

class Event {
public:
	Event() = default;
};

class IEventCallback {
private:
	virtual void Call(Event& e) = 0;
public:
	virtual ~IEventCallback() = default;
	void Execute(Event& e) {
		Call(e);
	}
};

template <typename TOWner, typename TEvent>
class EventCallback: public IEventCallback {
private:
	typedef void (TOWner::*CallbackFunction)(TEvent&);

	TOWner* ownerInstance;
	CallbackFunction callbackFunction;

	virtual void Call(Event& e) override {
		std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(e));
	}
public:
	EventCallback(TOWner* ownerInstance, CallbackFunction callbackFunction) {
		this->ownerInstance = ownerInstance;
		this->callbackFunction = callbackFunction;
	}

	virtual ~EventCallback() override = default;

};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus {
private:
	std::map<std::type_index, std::unique_ptr<HandlerList>> m_subscribers;

public:
	EventBus() {
		Logger::Log("EventBus constructor called!");
	}

	~EventBus() {
		Logger::Log("EventBus deconstructor called!");
	}


	template <typename TEvent>
	void EmitEvent() {
		auto handlers = m_subscribers[typeid(TEvent)].get();
		if (handlers) {
			for (auto it = handlers->begin(), it != handlers->end(); it++) {
				auto handler = it->get();
				TEvent event();
				handler->Execute(event);
			}
		}
	}

	template <typename TEvent, typename TOWner>
	void SubscribeToEvent(
		TOWner* ownerInstance, 
		void (TOWner::* callbackFunction)(TEvent&)){

		if (!m_subscribers[typeid(TEvent)].get()) {
			m_subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
		}
	
		auto subscriber = std::make_unique<EventCallback<TOWner, TEvent>>(ownerInstance, callbackFunction);
		m_subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
	
	}

};