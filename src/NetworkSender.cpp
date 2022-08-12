#include "NetworkSender.h"
#include <iostream>
#include <thread>
#include "Matrix.h"
#include <string>


NetworkSender::~NetworkSender() 
{
	closesocket(sock);
}

void NetworkSender::WinSockSettings() {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
}

NetworkSender::NetworkSender(const char* srv_Adr, int port)
{
	std::cout << "[Network-Sender] Network Sender started..." << std::endl;
	this->PORT = port;
	this->Server_Address = srv_Adr;
	this->WinSockSettings();




	sock = socket(AF_INET, SOCK_DGRAM, 0);
	memset((void*)&srv_addr, '\0', sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(this->PORT);   //Player 1 send-port
	inet_pton(AF_INET, this->Server_Address, &srv_addr.sin_addr.s_addr);
	int result = bind(sock, (sockaddr*)&srv_addr, sizeof(srv_addr));
	if (result < 0)std::cout << "ERROR not send (bind)" << std::endl;
}

void NetworkSender::SendData(Camera* cam)
{
	//std::cout << "SENDING..." << std::endl;
	/*
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in srv_addr;
	memset((void*)&srv_addr, '\0', sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(this->PORT);   //Player 1 send-port
	inet_pton(AF_INET, this->Server_Address, &srv_addr.sin_addr.s_addr);
	int result = bind(sock, (sockaddr*)&srv_addr, sizeof(srv_addr));
	if (result < 0)std::cout << "ERROR not send (bind)" << std::endl;
	*/
	//std::cout << "[Network-Sender] SENDING..." << std::endl;

	Matrix invView = Matrix(cam->getViewMatrix());
	invView.invert();
	float x = invView.m03;
	float y = invView.m13;
	float z = invView.m23;

	std::string incompleteMessage="";
	incompleteMessage += std::to_string(x);
	incompleteMessage += ",";
	incompleteMessage += std::to_string(y);
	incompleteMessage += ",";
	incompleteMessage += std::to_string(z);
	incompleteMessage += ",";
	incompleteMessage += "1.0,4.0,5.0,1,";

	//std::cout << "[Network-Sender] <inc_mess> " << incompleteMessage << std::endl;

	//const char* message = getPlaneInfoAsChar();
	//const char* message = "-1.0,10.0,-2.0,1.0,4.0,5.0,1,";
	const char* message = incompleteMessage.c_str();

	int slen = sizeof(srv_addr);
	if (sendto(sock, message, strlen(message), 0, (struct sockaddr*)&srv_addr, slen) == SOCKET_ERROR)
	{
		std::cout << "ERROR not send" << std::endl;
	}
	

	//closesocket(sock);
}

