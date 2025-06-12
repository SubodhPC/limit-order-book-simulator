#include "order_book.h"

void OrderBook::AddOrder(const Order& order)
{
	if (order.quantity < 0) return;

	order.side == OrderSide::BUY ? _buyOrders[order.price].push(order) : _sellOrders[order.price].push(order);
}

std::optional<Order> OrderBook::getBestMatch(OrderSide side, double price)
{
	switch (side)
	{
	case OrderSide::BUY:
	{
		if (_sellOrders.empty())
			return std::nullopt;

		auto it = _sellOrders.begin();
		if (it->first <= price)
			return it->second.front();

	}
	case OrderSide::SELL:
	{
		if (_buyOrders.empty())
			return std::nullopt;

		auto it = _buyOrders.begin();
		if (it->first >= price)
			return it->second.front();
	}
	}
	return std::nullopt;
}