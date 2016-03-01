

#include "Server.h"
#include <iostream>
int main()
{

    unsigned short port = 0;
    std::cin >> port;
    // 9600

    g = new Server;
    g->initialization();
    g->run();
    return 0;
}
