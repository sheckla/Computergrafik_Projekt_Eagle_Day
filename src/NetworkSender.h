/*
 * NetworkSender
 * - Network Component that is only instantiated in online mode, as it is part of the enemy plane
 * - Opens a socket connection, that translates and sends data through the internet
 * - Sends a udp-package every frame
 */

#ifndef NetworkSender_hpp
#define NetworkSender_hpp

#include <stdio.h>
#include "Model.h"

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
