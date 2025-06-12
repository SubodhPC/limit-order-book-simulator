#include "matching_engine.h"

void MatchingEngine::SubmitOrder(Order& order)
{
	std::optional<Order*> match = _orderBook.GetBestMatch(order.side, order.price);

	while (match.has_value() && order.quantity > 0)
	{
		ExecuteOrder(order, match.value());
		match = _orderBook.GetBestMatch(order.side, order.price);
	}
	if (order.quantity > 0)
	{
		_orderBook.AddOrder(order);
	}
}

void MatchingEngine::CancelOrder(uint64_t orderId)
{
	_orderBook.RemoveOrder(orderId);
}

void MatchingEngine::ExecuteOrder(Order& incoming, Order* resting)
{
	while (incoming.quantity > 0)
	{
		uint64_t remainingQuantities = incoming.quantity - resting->quantity;
		if (remainingQuantities < 0)
		{
			resting->quantity -= incoming.quantity;
			incoming.quantity = 0;
		}
		else 
		{
			incoming.quantity -= resting->quantity;
			_orderBook.RemoveOrder(*resting);
		}
	}
}
