#include <iostream>

#include <chrono>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <random>
#include <ctime>

#include "sterragiven.h"
#include "helpers.h"

int main(int argc, char **argv)
{
    std::mutex mx;
    std::condition_variable cv;
    std::vector<std::thread> processes;
    std::random_device rand;

    int     processesCount  = 4;
    int     duration        = 30;
    size_t  created         = 0;

    ParseArgs(argc, argv, processesCount, duration);

    std::cout << "Running for " << duration << " seconds with "
              << processesCount << " processes threads" << std::endl;

    std::queue<Request *> requests;

    bool endFlag;
    Stopper stoper{endFlag};


    std::thread creator([&]()
    {
        auto runTime = time(nullptr);

        // Получаем запросы в течении времени duration
        while (runTime + duration > time(nullptr))
        {
            std::unique_lock<std::mutex> lock(mx);

            requests.push(GetRequest(stoper));

            ++created;

            // Дадим шанс другим потокам завладеть мютексом
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::microseconds(1));

            cv.notify_one();
        }

        std::this_thread::sleep_for(std::chrono::microseconds(1));

        stoper.stop();
    });


    for (int i = 0; i < processesCount; ++i)
    {
        processes.emplace_back([&]() {

            while (!stoper.isEnd())
            {
                std::unique_lock<std::mutex> lock(mx);

                // Ждём разрешения на выполнение
                cv.wait(lock, [&](){
                    // Если очередь пуста, а конец ещё не наступил
                    // подождать ещё.
                    return !requests.empty() || stoper.isEnd();
                });

                if (requests.empty() || stoper.isEnd())
                {
                    break;
                }

                auto reqest = requests.front();
                requests.pop();

                lock.unlock();

                ProcessRequest(reqest, stoper);

                delete reqest;
            }
        });
    }

    creator.join();

    // Когда поток заполнения отработал,
    // выводим все потоки из ожидания.
    cv.notify_all();

    for (auto & prc : processes)
    {
        prc.join();
    }

    std::cout     << "\nRequests created: " << created
                  << "\nRequests started: " << Request::getLogger().started
                  << "\nRequests executed: " << Request::getLogger().executed
                  << "\nRequests run after stopping: " << Request::getLogger().dryRuns
                  << "\nRequests unprocessed:" << created - Request::getLogger().executed
                  << "\nRequests unprocessed in queue: " << requests.size()
                  << std::endl;

    // Освобождаем память от невыполненных процессов.
    while (!requests.empty())
    {
        auto reqest = requests.front();

        requests.pop();

        delete reqest;
    }

    return 0;
}
