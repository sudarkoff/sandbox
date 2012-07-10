#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

int MAX_SIZE = 5;
std::queue<int> Q;
std::condition_variable E, F;
std::mutex M;

int item = 0;

bool exit() { return (item > 81920); }

void producer()
{
    while (!exit()) {
        std::unique_lock<std::mutex> lock(M);
        E.wait(lock, []() { return Q.size() <= MAX_SIZE; });

        std::cout << "PUSH item: " << ++item << std::endl;
        Q.push(item);

        F.notify_one();
    }
}

void consumer()
{
    while (!exit()) {
        std::unique_lock<std::mutex> lock(M);
        F.wait(lock, []() { return Q.size() > 0; });

        int item = Q.front(); Q.pop();
        std::cout << "\t\tPOP item: " << item << ", Q.size: " << Q.size() << std::endl;

        E.notify_one();
    }
}

int main(int argc, char* argv[])
{
    std::thread p1(producer);
    std::thread p2(producer);
    std::thread p3(producer);

    std::thread c1(consumer);
    std::thread c2(consumer);
    std::thread c3(consumer);

    p1.join();
    p2.join();
    p3.join();

    c1.join();
    c2.join();
    c3.join();

    return 0;
}
