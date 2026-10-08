#include "execcore/execution_engine.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <sstream>

ExecutionEngine::~ExecutionEngine(){ close(); }

bool ExecutionEngine::connect_socket(const std::string& host, std::uint16_t port) {
    fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ < 0) return false;
    int one = 1;
    ::setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
#ifdef SO_BUSY_POLL
    int busy = 50;
    ::setsockopt(fd_, SOL_SOCKET, SO_BUSY_POLL, &busy, sizeof(busy));
#endif
    sockaddr_in addr{}; addr.sin_family = AF_INET; addr.sin_port = htons(port);
    if (::inet_pton(AF_INET, host.c_str(), &addr.sin_addr) != 1) { close(); return false; }
    if (::connect(fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) { close(); return false; }
    return true;
}

bool ExecutionEngine::connect(const std::string& host, std::uint16_t port) { return connect_socket(host, port); }

bool ExecutionEngine::send_order(const Order& o) {
    std::ostringstream out;
    out << o.sequence << ' ' << (o.side == Side::BUY ? 'B' : 'S') << ' ' << o.symbol << ' ' << o.quantity << ' ' << o.price << '\n';
    const std::string payload = out.str();
    iovec iov{const_cast<char*>(payload.data()), payload.size()};
    msghdr msg{}; msg.msg_iov=&iov; msg.msg_iovlen=1;
    const auto n = ::sendmsg(fd_, &msg, MSG_NOSIGNAL);
    return n == static_cast<ssize_t>(payload.size());
}

bool ExecutionEngine::process_one() {
    Order o;
    if (!intake_.try_pop(o)) return false;
    if (fd_ < 0) { ++retries_; intake_.try_push(o); return false; }
    if (!send_order(o)) { ++retries_; intake_.try_push(o); return false; }
    return true;
}

bool ExecutionEngine::apply_fill(std::uint64_t fill_sequence, std::int64_t qty, Order& order) {
    if (fills_.possibly_contains(fill_sequence)) return false;
    fills_.add(fill_sequence);
    if (qty <= 0 || order.terminal()) return false;
    order.filled += qty;
    if (order.filled >= order.quantity) { order.filled = order.quantity; order.transition(OrderState::FILLED); }
    else { order.transition(OrderState::PARTIAL); }
    return true;
}

void ExecutionEngine::close() { if (fd_ >= 0) { ::shutdown(fd_, SHUT_RDWR); ::close(fd_); fd_=-1; } }
