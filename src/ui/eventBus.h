#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

class EventBus {
  public:
	using EventHandler = std::function<void(const std::string&)>;

	// Register an event handler
	void subscribe(const std::string& eventType, EventHandler handler);

	// Publish an event
	void publish(const std::string& eventType, const std::string& data);

  private:
	std::unordered_map<std::string, std::vector<EventHandler>> handlers;
};