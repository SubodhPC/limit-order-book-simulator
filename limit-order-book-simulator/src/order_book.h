#pragma once
#include <chrono>
#include <queue>
#include <map>
#include <string>
#include <optional>

enum class OrderSide { BUY, SELL };

struct Order
{
	uint64_t id = 0;
	OrderSide side;
	double price = 0.0;
	uint64_t  quantity = 0;
	std::chrono::high_resolution_clock::time_point timestamp;
};

class OrderBook
{
	std::map<double, std::queue<Order>, std::greater<double>> _buyOrders;
	std::map<double, std::queue<Order>> _sellOrders;
	std::string instrument = "";
public:
	void AddOrder(const Order& val);
	std::optional<Order> getBestMatch(OrderSide side, double price);
	bool ExecuteOrder();
	void ModifyOrder(int id, double val);
	void RemoveOrder(int id);
	std::string GetInstrumentName() { return instrument; }
};