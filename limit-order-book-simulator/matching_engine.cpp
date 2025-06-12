#include "matching_engine.h"

void MatchingEngine::SubmitOrder(Order& order)
{
	std::optional<Order> match = _orderBook.getBestMatch(order.side, order.price);

	while (match.has_value() && order.quantity > 0)
	{
		ExecuteOrder(order, match.value());
		match = _orderBook.getBestMatch(order.side, order.price);
	}
	if (order.quantity > 0)
	{
		_orderBook.AddOrder(order);
	}
}

void MatchingEngine::CancelOrder(uint64_t order)
{
}

void MatchingEngine::ExecuteOrder(Order& order, Order& price)
{
}
