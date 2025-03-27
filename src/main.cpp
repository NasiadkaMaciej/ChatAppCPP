#include "client.h"

int main() {
	// ToDo: Get server from command line argument
	Client client("wss://chat.nasiadka.pl/ws");
	client.run();
	return 0;
}