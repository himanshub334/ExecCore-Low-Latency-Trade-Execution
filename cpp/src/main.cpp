#include "execcore/execution_engine.hpp"
#include "execcore/order.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <thread>

static void exchange(std::uint16_t port){
 int s=socket(AF_INET,SOCK_STREAM,0),one=1; setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&one,sizeof(one));
 sockaddr_in a{};a.sin_family=AF_INET;a.sin_addr.s_addr=htonl(INADDR_LOOPBACK);a.sin_port=htons(port);bind(s,(sockaddr*)&a,sizeof(a));listen(s,1);int c=accept(s,nullptr,nullptr);char b[2048];while(recv(c,b,sizeof(b),0)>0){}close(c);close(s);
}
int main(){const uint16_t port=39091; std::thread t(exchange,port); std::this_thread::sleep_for(std::chrono::milliseconds(20)); ExecutionEngine e; if(!e.connect("127.0.0.1",port)){std::cerr<<"connect failed\n";t.join();return 1;} Order o{1,"AAPL",Side::BUY,100,0,180.25,OrderState::NEW}; o.transition(OrderState::SENT); e.submit(o); e.process_one(); std::cout<<"order submitted, retries="<<e.retries()<<"\n"; e.close(); t.join(); }
