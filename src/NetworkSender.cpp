#include "NetworkSender.h"
#include <iostream>
#include <thread>
#include "Matrix.h"
#include <string>

#include "ApplicationSettings.h"
#include "Plane.h"
#include "ModelLoader.h"


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
	std::cout << "[Network-Sender] WSAStartup complete" << std::endl;
}

NetworkSender::NetworkSender(const char* srv_Adr, int port)
{
	std::cout << "[Network-Sender] Network Sender started..." << std::endl;
	std::cout << "[Network-Sender] Sending to " << srv_Adr << " : " << port << std::endl;
	this->PORT = port;
	this->Server_Address = srv_Adr;

	//Unknown translation error -> Not enought time so hardcoded...
	this->PORT = std::atoi(ApplicationSettings::LOCAL_PORT.c_str());
	this->Server_Address = ApplicationSettings::LOCAL_IP.c_str();

	this->WinSockSettings();

	//Default UDP-Socket Code (From Distributed Systems Course)
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	memset((void*)&srv_addr, '\0', sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(this->PORT);   //Player 1 send-port
	inet_pton(AF_INET, this->Server_Address, &srv_addr.sin_addr.s_addr);
	int result = bind(sock, (sockaddr*)&srv_addr, sizeof(srv_addr));
	if(result == SOCKET_ERROR)std::cout << "ERROR (bind)" << std::endl;
	if (result < 0)std::cout << "ERROR not send (bind)" << std::endl;
}

void NetworkSender::SendData(Plane* plane)
{
	Model* model = plane->getParts()[0];

	float enemyHP = 100.0f;
	if(ModelLoader::instance().pEnemyPlane != nullptr)
	enemyHP = ModelLoader::instance().pEnemyPlane->hp;

		Matrix PlaneTranform;

		PlaneTranform = model->transform();

		//Translate Matrix into udp-sendable data

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

		if (plane->isShooting == true)incompleteMessage += "1";
		else incompleteMessage += "0";
		incompleteMessage += ",";

		incompleteMessage += std::to_string(plane->getSpeed());// SPEED
		incompleteMessage += ",";

		incompleteMessage += std::to_string(enemyHP);
		incompleteMessage += ",";

		std::cout << "[Network-Sender] "<<enemyHP  << std::endl;

		const char* message = incompleteMessage.c_str();

		//Send udp-package
		int slen = sizeof(srv_addr);
		if (sendto(sock, message, strlen(message), 0, (struct sockaddr*)&srv_addr, slen) == SOCKET_ERROR)
		{
			std::cout << "ERROR not send" << std::endl;
		}//else std::cout << "send successfuly" << std::endl;

}

