#include "execcore/order_book.hpp"
#include <chrono>
#include <iostream>
#include <random>
#include <string>

template<class B> long long run(B& b,std::size_t n){auto st=std::chrono::steady_clock::now(); long long sink=0;for(std::size_t i=0;i<n;i++){b.update((i*2654435761ULL)%1000000,i%1000);sink+=b.total();}auto us=std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now()-st).count();std::cout<<us<<" us sink="<<sink<<"\n";return us;}
int main(){ArenaOrderBook a;PointerOrderBook p;std::cout<<"arena ";run(a,10000);std::cout<<"pointer ";run(p,10000);}
