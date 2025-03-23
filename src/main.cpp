#include "client.h"

int main() {
    ChatClient client("wss://chat.nasiadka.pl/ws");
    client.run();
    return 0;
}