#include "NetworkConnector.h"
#include <iostream>
#include <thread>
#include <winsock2.h>
#include "ModelLoader.h"

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

	PORT = 19413;
	Server_Address = "127.0.0.1";

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

	for (int i=0;i<1;i++) // First: initial, Second: actual data transfer [...]
	{
		//const char* message = getPlaneInfoAsChar();
		const char* message = "-1.0,4,-2.0,1.0,4.0,5.0,1,";
	
		int slen = sizeof(srv_addr);
		if (sendto(sock, message, strlen(message), 0, (struct sockaddr*)&srv_addr, slen) == SOCKET_ERROR)
		{
			std::cout << "ERROR" << std::endl;
		}
	}
}




void NetworkConnector::ReadAndSetData(char* buf , EnemyPlane* enemy)
{
	/*
	float x = 1, y = 1, z = 1;
	float rx = 1, ry = 1, rz = 1;
	float rx2 = 1, ry2 = 1, rz2 = 1;
	float rx3 = 1, ry3 = 1, rz3 = 1;
	*/
	float m_00 = 1, m_01 = 1, m_02 = 1, m_03 = 1;
	float m_10 = 1, m_11 = 1, m_12 = 1, m_13 = 1;
	float m_20 = 1, m_21 = 1, m_22 = 1, m_23 = 1;
	float m_30 = 1, m_31 = 1, m_32 = 1, m_33 = 1;

	float speed=0;
	float thisPlayerHp=100.0;

	bool shooting = false;

	std::string DataAsString = (std::string)buf;

	std::string delimiter = ",";

	int pos = 0;
	int item = 0;
	std::string token;
	while ((pos = DataAsString.find(delimiter)) != std::string::npos) {
		token = DataAsString.substr(0, pos);

		//std::cout << "TOKEN: " << token << " ITEM: " << item << std::endl;

		switch (item)
		{
		case 0:
			m_00 = std::stof(token);
			break;
		case 1:
			m_01 = std::stof(token);
			break;
		case 2:
			m_02 = std::stof(token);
			break;
		case 3:
			m_03 = std::stof(token);
			break;
		case 4:
			m_10 = std::stof(token);
			break;
		case 5:
			m_11 = std::stof(token);
			break;
		case 6:
			m_12 = std::stof(token);
			break;
		case 7:
			m_13 = std::stof(token);
			break;
		case 8:
			m_20 = std::stof(token);
			break;
		case 9:
			m_21 = std::stof(token);
			break;
		case 10:
			m_22 = std::stof(token);
			break;
		case 11:
			m_23 = std::stof(token);
			break;
		case 12:
			m_30 = std::stof(token);
			break;
		case 13:
			m_31 = std::stof(token);
			break;
		case 14:
			m_32 = std::stof(token);
			break;
		case 15:
			m_33 = std::stof(token);
			break;
		case 16:
			if (token == "1")shooting = true;
			else shooting = false;
			break;
		case 17:
			speed = std::stof(token);
			break;
		case 18:
			thisPlayerHp = std::stof(token);
			break;
		default:
			break;
		}

		DataAsString.erase(0, pos + delimiter.length());
		item++;
	}

	if (m_00 == -99999999 && m_01 == -99999999 && m_02 == -99999999 && m_03 == -99999999 && m_10 == -99999999 && m_11 == -99999999 && m_12 == -99999999 && m_13 == -99999999) {
		if (this->hasRecievedDummyData == false) {
			std::cout << "[Input-Thread] Connected to Server! Waiting for Player 2" << std::endl;
			this->hasRecievedDummyData = true;
		}
	}
	else 
	{
		if (hasRecievedActualData == false) { 
			hasRecievedActualData = true; 
			std::cout << "[Input-Thread] Player 2 Connected!" << std::endl;
		}
	}
	//std::cout <<"[Input - Thread] "<< "x=" << x << " y=" << y << " z=" << z << " rx=" << rx << " ry=" << ry << " rz=" << rz << " shooting=" << std::boolalpha << shooting << std::endl;

	bool isAnUpdate = true;
	if (enemy->Enemy_Tranformation.m00 == m_00 && enemy->Enemy_Tranformation.m01 == m_01 && enemy->Enemy_Tranformation.m02 == m_02 && enemy->Enemy_Tranformation.m03 == m_03 && 
		enemy->Enemy_Tranformation.m10 == m_10 && enemy->Enemy_Tranformation.m11 == m_11 && enemy->Enemy_Tranformation.m12 == m_12 && enemy->Enemy_Tranformation.m13 == m_13 &&
		enemy->Enemy_Tranformation.m20 == m_20 && enemy->Enemy_Tranformation.m21 == m_21 && enemy->Enemy_Tranformation.m22 == m_22 && enemy->Enemy_Tranformation.m23 == m_23 &&
		enemy->Enemy_Tranformation.m30 == m_30 && enemy->Enemy_Tranformation.m31 == m_31 && enemy->Enemy_Tranformation.m32 == m_32 && enemy->Enemy_Tranformation.m33 == m_33 &&
		speed != 0)
		isAnUpdate = false;

	enemy->Enemy_Tranformation.m00 = m_00;
	enemy->Enemy_Tranformation.m01 = m_01;
	enemy->Enemy_Tranformation.m02 = m_02;
	enemy->Enemy_Tranformation.m03 = m_03;

	enemy->Enemy_Tranformation.m10 = m_10;
	enemy->Enemy_Tranformation.m11 = m_11;
	enemy->Enemy_Tranformation.m12 = m_12;
	enemy->Enemy_Tranformation.m13 = m_13;

	enemy->Enemy_Tranformation.m20 = m_20;
	enemy->Enemy_Tranformation.m21 = m_21;
	enemy->Enemy_Tranformation.m22 = m_22;
	enemy->Enemy_Tranformation.m23 = m_23;

	enemy->Enemy_Tranformation.m30 = m_30;
	enemy->Enemy_Tranformation.m31 = m_31;
	enemy->Enemy_Tranformation.m32 = m_32;
	enemy->Enemy_Tranformation.m33 = m_33;
	enemy->Enemy_Speed = speed;

	//std::cout << "[Input-Thread] Set..." << std::endl;

	if(isAnUpdate)
	enemy->Enemy_Tranformation_Validation = true;

	enemy->isShooting = shooting;

	if(ModelLoader::instance().pPlayerPlane != nullptr)
	ModelLoader::instance().pPlayerPlane->hp = thisPlayerHp;
}

void NetworkConnector::Connection(EnemyPlane& enemy) 
{
	std::cout << "[Input-Thread] Reading Network for Updates..." << std::endl;
	std::cout << "[Input-Thread] Recieving from " << Server_Address << " : " << PORT << std::endl;

	//test(); 
	// Server now knows client, sends to 19413

	//-NETWORK-----------------------------------------------------------------------------------------------------------------------------------------
	
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in srv_addr;
	memset((void*)&srv_addr, '\0', sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(PORT); //Player 1 recv-port
	inet_pton(AF_INET, Server_Address, &srv_addr.sin_addr.s_addr);
	int slen = sizeof(srv_addr);
	int result = bind(sock, (sockaddr*)&srv_addr, sizeof(srv_addr));
	if (result < 0)std::cout << "[Input-Thread] ERROR, cannot bind" << std::endl;
	//else std::cout << "[Input-Thread] SUCCESS" << std::endl;

	//-NETWORK-----------------------------------------------------------------------------------------------------------------------------------------

	std::cout << "[Input-Thread] Connecting to Server..." << std::endl;
	//GET COORDS´n´Stuff
	for(;;)
	{
		char buf[200];
		recvfrom(sock, buf, 200, 0, (SOCKADDR*)&srv_addr, &slen);
		//std::cout << "[Input - Thread] " << buf << std::endl;
		this->ReadAndSetData(buf,&enemy);
		//std::cout << "readDone" << std::endl;
		//delete[] buf;
		//enemyPlane.Enemy_Position = Vector(rand(), rand(), rand()); //Platzhalter
	}	
}
