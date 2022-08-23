#ifndef NetworkConnector_hpp
#define NetworkConnector_hpp

#include <stdio.h>
#include "Application.h"
#include "EnemyPlane.h"
#include <winsock2.h>
#include <Ws2tcpip.h>


//class enemyPlane;

class NetworkConnector
{
public:
    NetworkConnector(EnemyPlane &Enemy, const char* srv_Adr, int port);
    void Connection(EnemyPlane &enemy);
private:
    void WinSockSettings();
    int PORT;
    const char* Server_Address;
    void ReadAndSetData(char* buf, EnemyPlane* enemy);
    bool hasRecievedDummyData = false;
    bool hasRecievedActualData = false;
};



#endif /* NetworkConnector_hpp */
