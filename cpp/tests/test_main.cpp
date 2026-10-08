#include "execcore/order.hpp"
#include "execcore/execution_engine.hpp"
#include "execcore/spsc_queue.hpp"
#include <cassert>
#include <iostream>

static void test_fsm(){Order o{1,"X",Side::BUY,10};assert(o.transition(OrderState::SENT));assert(o.transition(OrderState::PARTIAL));assert(!o.transition(OrderState::NEW));assert(o.transition(OrderState::FILLED));assert(!o.transition(OrderState::CANCELLED));}
static void test_queue(){SPSCQueue<int,8> q;for(int i=0;i<8;i++)assert(q.try_push(i));assert(!q.try_push(9));for(int i=0;i<8;i++){int x=-1;assert(q.try_pop(x));assert(x==i);}int x;assert(!q.try_pop(x));}
static void test_duplicate_fill(){ExecutionEngine e;Order o{7,"AAPL",Side::BUY,100};o.transition(OrderState::SENT);assert(e.apply_fill(99,40,o));assert(o.filled==40);assert(!e.apply_fill(99,40,o));assert(o.filled==40);assert(e.apply_fill(100,60,o));assert(o.state==OrderState::FILLED);}
int main(){test_fsm();test_queue();test_duplicate_fill();std::cout<<"all tests passed\n";}
