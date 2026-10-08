#include "execcore/order.hpp"

bool Order::transition(OrderState next) {
    bool ok = false;
    switch (state) {
        case OrderState::NEW: ok = (next == OrderState::SENT || next == OrderState::CANCELLED); break;
        case OrderState::SENT: ok = (next == OrderState::PARTIAL || next == OrderState::FILLED || next == OrderState::CANCELLED); break;
        case OrderState::PARTIAL: ok = (next == OrderState::PARTIAL || next == OrderState::FILLED || next == OrderState::CANCELLED); break;
        default: ok = false;
    }
    if (ok) state = next;
    return ok;
}
