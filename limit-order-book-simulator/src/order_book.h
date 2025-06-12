#pragma once
#include <chrono>
#include <queue>
#include <map>
#include <unordered_map>
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
public:
	void AddOrder(const Order& val);
	std::optional<Order*> GetBestMatch(OrderSide side, double price);
	bool ExecuteOrder();
	void ModifyOrder(int id, double val);
	void RemoveOrder(Order order);
	void RemoveOrder(uint64_t orderId);
	std::string GetInstrumentName() { return _instrument; }

private:
	std::map<double, std::queue<Order>, std::greater<double>> _buyOrders;
	std::map<double, std::queue<Order>> _sellOrders;
	std::unordered_map<uint64_t, std::pair<OrderSide, double>> _orderIndex;
	std::string _instrument = "";
};