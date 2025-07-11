#include "eventBus.h"

void EventBus::subscribe(const std::string& eventType, EventHandler handler) {
	handlers[eventType].push_back(handler);
}

void EventBus::publish(const std::string& eventType, const std::string& data) {
	auto it = handlers.find(eventType);
	if (it != handlers.end())
		for (const auto& handler : it->second)
			handler(data);
}