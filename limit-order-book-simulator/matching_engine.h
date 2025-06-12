#pragma once
#include "src/order_book.h"

class MatchingEngine
{
public:
	MatchingEngine(OrderBook& book) : _orderBook(book){}
	void SubmitOrder(Order& order);
	void CancelOrder(uint64_t order);

private:
	OrderBook& _orderBook;
	void ExecuteOrder(Order& order, Order& price);
};

