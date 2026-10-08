#include "execcore/order_book.hpp"
#include <numeric>
#include <new>

ArenaOrderBook::ArenaOrderBook(std::size_t levels): arena_(levels) {
    for(std::size_t i=0;i<levels;i++) arena_[i] = {static_cast<std::int64_t>(i),0, static_cast<std::uint32_t>(i+1<levels?i+1:0)};
}
void ArenaOrderBook::update(std::size_t i,std::int64_t qty){arena_[i%arena_.size()].quantity=qty;}
std::int64_t ArenaOrderBook::total() const { std::int64_t s=0; for(auto& x:arena_) s+=x.quantity; return s; }

PointerOrderBook::PointerOrderBook(std::size_t levels){ nodes_.reserve(levels); for(std::size_t i=0;i<levels;i++) nodes_.push_back(new Node{static_cast<std::int64_t>(i),0,nullptr}); for(std::size_t i=0;i+1<levels;i++) nodes_[i]->next=nodes_[i+1]; }
PointerOrderBook::~PointerOrderBook(){for(auto p:nodes_) delete p;}
void PointerOrderBook::update(std::size_t i,std::int64_t qty){nodes_[i%nodes_.size()]->quantity=qty;}
std::int64_t PointerOrderBook::total() const {std::int64_t s=0; for(auto p:nodes_) s+=p->quantity; return s;}
