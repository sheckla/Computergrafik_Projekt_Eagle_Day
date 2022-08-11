#ifndef NetworkSender_hpp
#define NetworkSender_hpp

#include <stdio.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include "Camera.h"

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
