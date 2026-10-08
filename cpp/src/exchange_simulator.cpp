#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

void run_exchange_simulator(std::uint16_t port) {
    int server = ::socket(AF_INET, SOCK_STREAM, 0);
    int one=1; setsockopt(server,SOL_SOCKET,SO_REUSEADDR,&one,sizeof(one));
    sockaddr_in addr{}; addr.sin_family=AF_INET; addr.sin_addr.s_addr=htonl(INADDR_LOOPBACK); addr.sin_port=htons(port);
    bind(server,reinterpret_cast<sockaddr*>(&addr),sizeof(addr)); listen(server,1);
    int client=accept(server,nullptr,nullptr); if(client<0){close(server);return;}
    char buf[1024]; ssize_t n;
    while((n=recv(client,buf,sizeof(buf)-1,0))>0){ buf[n]=0; std::cout<<"EXCHANGE: "<<buf; }
    close(client); close(server);
}
