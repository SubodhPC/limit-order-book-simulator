#include "order_book.h"

void OrderBook::AddOrder(const Order& order)
{
	if (order.quantity < 0) return;

	order.side == OrderSide::BUY ? _buyOrders[order.price].push(order) : _sellOrders[order.price].push(order);
    _orderIndex.insert({ order.id, std::make_pair(order.side, order.price) });
}

std::optional<Order*> OrderBook::GetBestMatch(OrderSide side, double price)
{
	switch (side)
	{
	case OrderSide::BUY:
	{
		if (_sellOrders.empty())
			return std::nullopt;

		auto it = _sellOrders.begin();
		if (it->first <= price)
			return &(it->second.front());
		break;
	}
	case OrderSide::SELL:
	{
		if (_buyOrders.empty())
			return std::nullopt;

		auto it = _buyOrders.begin();
		if (it->first >= price)
			return &(it->second.front());
		break;
	}
	}
	return std::nullopt;
}

void OrderBook::RemoveOrder(Order& orderToRemove)  
{  
    RemoveOrder(orderToRemove.id);
}

void OrderBook::RemoveOrder(uint64_t& orderId)
{
    auto it = _orderIndex.find(orderId);
    if (it == _orderIndex.end()) {
        return;
    }

    OrderSide side = it->second.first;
    double price = it->second.second;

    side == OrderSide::BUY ? RemoveBuyOrder(orderId, price) : RemoveSellOrder(orderId, price);
}

void OrderBook::RemoveBuyOrder(uint64_t& orderId, double price)
{
    auto& orders = _buyOrders;
    auto mapIt = orders.find(price);
    if (mapIt == orders.end()) {
        // Order in orderindex but not in order book
        _orderIndex.erase(orderId);
        return;
    }

    std::queue<Order>& q = mapIt->second;
    std::queue<Order> newQueue;

    // Rebuild queue without the target order
    while (!q.empty()) {
        Order o = q.front(); q.pop();
        if (o.id != orderId) {
            newQueue.push(o);
        }
    }

    if (newQueue.empty()) {
        orders.erase(mapIt); // remove price level entirely
    }
    else {
        mapIt->second = std::move(newQueue);
    }

    _orderIndex.erase(orderId);
}

void OrderBook::RemoveSellOrder(uint64_t& orderId, double price)
{
    auto& orders = _sellOrders;
    auto mapIt = orders.find(price);
    if (mapIt == orders.end()) {
        // Order in orderindex but not in order book
        _orderIndex.erase(orderId);
        return;
    }

    std::queue<Order>& q = mapIt->second;
    std::queue<Order> newQueue;

    // Rebuild queue without the target order
    while (!q.empty()) {
        Order o = q.front(); q.pop();
        if (o.id != orderId) {
            newQueue.push(o);
        }
    }

    if (newQueue.empty()) {
        orders.erase(mapIt); // remove price level entirely
    }
    else {
        mapIt->second = std::move(newQueue);
    }

    _orderIndex.erase(orderId);
}
