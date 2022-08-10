#include "NetworkConnector.h"
#include <iostream>
#include <thread>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

void NetworkConnector::WinSockSettings() {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
}

NetworkConnector::NetworkConnector(EnemyPlane& enemy, const char* srv_Adr, int port)
{
	std::cout << "[Network] Enemy Network Connector started..." << std::endl;
	PORT = port;
	Server_Address = srv_Adr;
	this->WinSockSettings();

	std::thread t1(&NetworkConnector::Connection,this, std::ref(enemy));
	t1.detach();
}

void test() // used to simulate initial sending of info from client 1
{
	std::cout << "[Input - Thread] TEST" << std::endl;

	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in srv_addr;
	memset((void*)&srv_addr, '\0', sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(19411);   //Player 1 send-port
	inet_pton(AF_INET, "127.0.0.1", &srv_addr.sin_addr.s_addr);
	int result = bind(sock, (sockaddr*)&srv_addr, sizeof(srv_addr));
	if (result < 0)std::cout << "ERROR" << std::endl;

	std::cout << "[Output-Thread] SENDING..." << std::endl;

	for (int i=0;i<3;i++) // First: initial, Second: actual data transfer [...]
	{
		//const char* message = getPlaneInfoAsChar();
		const char* message = "1.0,2.0,3.0,1.0,4.0,5.0,1,";

		int slen = sizeof(srv_addr);
		if (sendto(sock, message, strlen(message), 0, (struct sockaddr*)&srv_addr, slen) == SOCKET_ERROR)
		{
			std::cout << "ERROR" << std::endl;
		}
		//else
		//{
		//		std::cout << "SUCCESS" << std::endl;
		//}
	}
}

void NetworkConnector::Connection(EnemyPlane& enemy) 
{
	std::cout << "[Network-Thread] Reading Network for Updates..." << std::endl;

	test(); 
	// Server now knows client, sends to 19413

	//------------------------------------------------------------------------------------------------------------------------------------------
	
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in srv_addr;
	memset((void*)&srv_addr, '\0', sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(19413); //Player 1 recv-port
	inet_pton(AF_INET, "127.0.0.1", &srv_addr.sin_addr.s_addr);
	int slen = sizeof(srv_addr);
	int result = bind(sock, (sockaddr*)&srv_addr, sizeof(srv_addr));
	if (result < 0)std::cout << "[Input - Thread] ERROR" << std::endl;

	//------------------------------------------------------------------------------------------------------------------------------------------

	//GET COORDS´n´Stuff
	for(;;)
	{
		char buf[200];
		recvfrom(sock, buf, 200, 0, (SOCKADDR*)&srv_addr, &slen);
		std::cout << "[Input - Thread] " << buf << std::endl;
		//Not blocking...? Doesnt get shit

		//enemy.Enemy_Position = Vector(rand(), rand(), rand()); //Platzhalter
	}	
}
