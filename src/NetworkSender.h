#ifndef NetworkSender_hpp
#define NetworkSender_hpp

#include <stdio.h>
#include "Camera.h"
#include "Globals.h"
#pragma comment(lib,"ws2_32.lib") //Winsock Library

class NetworkSender
{
public:
    NetworkSender(const char* srv_Adr, int port);
    ~NetworkSender();
    void SendData(Camera* cam);
private:
    void WinSockSettings();
    int PORT;
    const char* Server_Address;

    sockaddr_in srv_addr;
    SOCKET sock;
    //void ReadAndSetData(char* buf, EnemyPlane* enemy);
};



#endif /* NetworkSender_hpp */
