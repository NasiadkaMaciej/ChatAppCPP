#pragma once

#include <functional>
#include <string>
#include <unordered_map>

class CommandProcessor {
  public:
	using CommandHandler = std::function<void(const std::string&)>;

	CommandProcessor();

	// Register a command handler
	void registerCommand(const std::string& command, CommandHandler handler);

	// Process a command
	bool processCommand(const std::string& command, const std::string& args);

	// Check if input is a command
	bool isCommand(const std::string& input) const;

  private:
	std::unordered_map<std::string, CommandHandler> commandHandlers;
};