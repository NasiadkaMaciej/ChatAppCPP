#include "client.h"

int main() {
	std::setlocale(LC_ALL, "");

	// ToDo: Get server from command line argument
	Client client("wss://chat.nasiadka.pl/ws");
	client.run();
	return 0;
}