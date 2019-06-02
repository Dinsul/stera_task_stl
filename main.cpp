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

    int processesCount = 2;
    int duration       = 30;
    int created        = 0;
    int executed       = 0;

#ifdef __unix__
    std::cout << "\033[0;0H\033[J" << std::endl;
#endif

    ParseArgs(argc, argv, processesCount, duration);

    std::cout << "Running for " << duration << " seconds with "
              << processesCount << " processes threads" << std::endl;

    std::queue<Request *> requests;
    Stopper stoper;


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

                if (requests.empty())
                {
                    break;
                }

                auto reqest = requests.front();
                requests.pop();

                lock.unlock();

                ProcessRequest(reqest, stoper);

                // Инкрементируем счётчик, только если
                // обработчик не прервал свою работу.
                // TODO
                // Опасное место. Тут stoper.stop() может сработать
                // после обработчика и до проверки. Надо обезопасить.
                if (!stoper.isEnd())
                {
                    ++executed;
                }

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

    // Освобождаем память от невыполненных процессов.
    while (!requests.empty())
    {
        auto reqest = requests.front();

        requests.pop();

        delete reqest;
    }

#ifdef __unix__
    std::cout << "\033[" << executed << ";" << 1 << "H" << std::endl;
#endif

    std::cout << created  << " processes was creted.\n"
              << executed << " processes was executed."
              << std::endl;

    return 0;
}
