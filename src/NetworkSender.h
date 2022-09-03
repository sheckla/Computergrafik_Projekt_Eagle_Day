#ifndef NetworkSender_hpp
#define NetworkSender_hpp

#include <stdio.h>
//#include <winsock2.h>
//#include <Ws2tcpip.h>
#include "Model.h"
//#include "Plane.h"

class Plane;

class NetworkSender
{
public:
    NetworkSender(const char* srv_Adr, int port);
    ~NetworkSender();
    void SendData(Plane* plane);
private:
    void WinSockSettings();
    int PORT;
    const char* Server_Address;

    sockaddr_in srv_addr;
    SOCKET sock;
};



#endif /* NetworkSender_hpp */
