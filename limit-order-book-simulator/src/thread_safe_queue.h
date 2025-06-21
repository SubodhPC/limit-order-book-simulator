#include <queue>
#include <shared_mutex>
#include <condition_variable>
#include <optional>

template<class T>
class ThreadSafeQueue {
private:
    std::queue<T> queue;
    mutable std::shared_mutex mtx; // Ensure mtx is mutable for const methods
    std::condition_variable cv;

public:
    void push(const T& value) {
        std::unique_lock<std::shared_mutex> lock(mtx);
        queue.push(value);
        cv.notify_one();
    }

    /*T waitAndPop() {
        std::unique_lock<std::shared_mutex> lock(mtx);
        cv.wait(lock, [&] { return !queue.empty(); });
        T val = queue.front(); queue.pop();
        return val;
    }*/

    const T& front() const {
        std::shared_lock<std::shared_mutex> lock(mtx);
        return queue.front();
    }

    bool empty() const {
        std::shared_lock<std::shared_mutex> lock(mtx);
        return queue.empty();
    }

    void pop() {
        std::unique_lock<std::shared_mutex> lock(mtx);
        if (queue.empty()) return;
        queue.pop();
    }

    std::optional<T> tryPop() {
        std::unique_lock<std::shared_mutex> lock(mtx);
        if (queue.empty()) return std::nullopt;
        T val = queue.front(); queue.pop();
        return val;
    }

    void move(std::queue<T>& newQueue) {
        std::unique_lock<std::shared_mutex> lock(mtx);
        queue = std::move(newQueue);
    }

    void move(std::queue<T>&& newQueue) {
        std::unique_lock<std::shared_mutex> lock(mtx);
        queue = std::move(newQueue);
    }
};