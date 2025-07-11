#include "client.h"

int main() {
	std::setlocale(LC_ALL, "");

	Client client("wss://chat.nasiadka.pl/ws");
	client.run();
	return 0;
}