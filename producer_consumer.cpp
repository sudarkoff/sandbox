/**
 * C++11 implementation of the producer-consumer design pattern.
 *
 * To build and execute:
 *   $ clang++ -std=c++11 -stdlib=libc++ producer_consumer.cpp -lc++ -lpthread && ./a.out
 */

#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

const int ITEMS_TO_PRODUCE = 100000;
const int QUEUE_CAPACITY = 10;

struct Queue {
  public:
    Queue() : done(false) {}

  public:
    std::queue<int> q;
    std::condition_variable not_full, not_empty;
    std::mutex m;
    bool done;
};

void producer(Queue& Q)
{
  static int item = 0;

  while (!Q.done) {
    std::unique_lock<std::mutex> lock(Q.m);
    Q.not_full.wait(lock, [&Q]() { return Q.q.size() <= QUEUE_CAPACITY; });

    if (item < ITEMS_TO_PRODUCE) {
      std::cout << "<<< PUSH item: " << ++item << std::endl;
      Q.q.push(item);

      Q.not_empty.notify_one();
    }
    else {
      Q.done = true;
    }
  }
}

void consumer(Queue& Q)
{
  int item = 0;

  while (1) {
    std::unique_lock<std::mutex> lock(Q.m);

    Q.not_empty.wait(lock, [&Q]() { return (!Q.q.empty() || Q.done); });
    if (Q.done && Q.q.empty()) { Q.not_empty.notify_all(); break; }

    item = Q.q.front(); Q.q.pop();
    std::cout << "\t\t>>> POP item: " << item << ", queue size: " << Q.q.size() << std::endl;

    Q.not_full.notify_one();
  }
}

int main(int argc, char* argv[])
{
  Queue Q;

  std::vector<std::thread> P(8);
  std::vector<std::thread> C(4);

  for (auto &p : P) { p = std::thread(producer, std::ref(Q)); }
  for (auto &c : C) { c = std::thread(consumer, std::ref(Q)); }

  for (auto &c : C) { c.join(); }
  for (auto &p : P) { p.join(); }

  return 0;
}

