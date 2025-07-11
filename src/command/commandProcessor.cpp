#include "commandProcessor.h"

CommandProcessor::CommandProcessor() {}

void CommandProcessor::registerCommand(const std::string& command, CommandHandler handler) {
	commandHandlers[command] = handler;
}

bool CommandProcessor::processCommand(const std::string& command, const std::string& args) {
	auto it = commandHandlers.find(command);
	if (it == commandHandlers.end()) return false;

	it->second(args);
	return true;
}

bool CommandProcessor::isCommand(const std::string& input) const {
	return !input.empty() && input[0] == '/';
}