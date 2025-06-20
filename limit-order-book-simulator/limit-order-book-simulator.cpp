// limit-order-book-simulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "src/matching_engine.h"
#include <thread>
#include <chrono>

void simulateClient(MatchingEngine& engine, int clientId, int numOrders) {
    for (int i = 0; i < numOrders; ++i) {
        Order order;
        order.id = clientId * 1000 + i;
        order.side = (i % 2 == 0) ? OrderSide::BUY : OrderSide::SELL;
        order.price = 95 + rand() % 11; // $95 to $105
        order.quantity = 10 + rand() % 91;
        order.timestamp = std::chrono::steady_clock::now();

        engine.SubmitOrder(order);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main()
{
    OrderBook book;
    MatchingEngine engine(book);

    //engine.start();

    std::vector<std::thread> clients;
    for (int i = 0; i < 4; ++i) {
        clients.emplace_back(simulateClient, std::ref(engine), i, 10);
    }

    for (auto& t : clients) t.join();

    //engine.stopEngine();
    return 0;
}