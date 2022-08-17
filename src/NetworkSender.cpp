#include "NetworkSender.h"
#include <iostream>
#include <thread>
#include <winsock2.h>
#include "Matrix.h"
#include <string>
#include "Plane.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library


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
	std::cout << "[Network-Sender] Sending to " << srv_Adr << " : " << port << std::endl;
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

void NetworkSender::SendData(Plane* plane)
{
	Model* model = plane->getParts()[0];
	//std::cout << "[Network-Sender] SENDING..." << std::endl;

		Matrix PlaneTranform;

		PlaneTranform = model->transform();

		std::string incompleteMessage = "";
		incompleteMessage += std::to_string(PlaneTranform.m00);
		incompleteMessage += ",";
		incompleteMessage += std::to_string(PlaneTranform.m01);
		incompleteMessage += ",";
		incompleteMessage += std::to_string(PlaneTranform.m02);
		incompleteMessage += ",";
		incompleteMessage += std::to_string(PlaneTranform.m03);
		incompleteMessage += ",";

		incompleteMessage += std::to_string(PlaneTranform.m10);
		incompleteMessage += ",";
		incompleteMessage += std::to_string(PlaneTranform.m11);
		incompleteMessage += ",";
		incompleteMessage += std::to_string(PlaneTranform.m12);
		incompleteMessage += ",";
		incompleteMessage += std::to_string(PlaneTranform.m13);
		incompleteMessage += ",";

		incompleteMessage += std::to_string(PlaneTranform.m20);
		incompleteMessage += ",";
		incompleteMessage += std::to_string(PlaneTranform.m21);
		incompleteMessage += ",";
		incompleteMessage += std::to_string(PlaneTranform.m22);
		incompleteMessage += ",";
		incompleteMessage += std::to_string(PlaneTranform.m23);
		incompleteMessage += ",";

		incompleteMessage += std::to_string(PlaneTranform.m30);
		incompleteMessage += ",";
		incompleteMessage += std::to_string(PlaneTranform.m31);
		incompleteMessage += ",";
		incompleteMessage += std::to_string(PlaneTranform.m32);
		incompleteMessage += ",";
		incompleteMessage += std::to_string(PlaneTranform.m33);
		incompleteMessage += ",";

		incompleteMessage += "1,";

		incompleteMessage += std::to_string(plane->getSpeed());// SPEED
		incompleteMessage += ",";

		//std::cout << "[Network-Sender] <inc_mess> " << incompleteMessage << std::endl;

		const char* message = incompleteMessage.c_str();

		int slen = sizeof(srv_addr);
		if (sendto(sock, message, strlen(message), 0, (struct sockaddr*)&srv_addr, slen) == SOCKET_ERROR)
		{
			std::cout << "ERROR not send" << std::endl;
		}

}

