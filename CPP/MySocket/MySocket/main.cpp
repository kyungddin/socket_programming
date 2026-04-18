#include "MySocket.h"

using namespace std;

int main()
{
	string ip = "127.0.0.1";
	int port = 50000;
	TCPClient testClient(ip, port);



	return 0;
}