#include "helpers.h"
#include "sterragiven.h"

#include <random>
#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>

Request *RequestsGenerator()
{
    std::random_device rand;

    // Имитируем выполнение разных действий.
    // Так как в задании сказано, что время работы
    // функци GetRequest() может быть разным, будем
    // спать случайное время, но не больше 1 с.
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1000));

    auto request = new Request();

    return request;
}

void ParseArgs(int argc, char **argv, int &threads, int &duration)
{
    bool runHelpPage = false;

    for (int i = 1; i < argc - 1; ++i)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
            case 't':
            case 'T':
                try
                {
                    threads = std::stoi(argv[i + 1]);
                }
                catch (...)
                {
                    runHelpPage = true;
                }
                break;

            case 'd':
            case 'D':
                try
                {
                    duration = std::stoi(argv[i + 1]);
                }
                catch (...)
                {
                    runHelpPage = true;;
                }
                break;

            default:
                runHelpPage = true;;
                break;
            }
        }
    }

    if (threads < 2 || threads > 10)
    {
        threads = 2;
        runHelpPage = true;;
    }

    if (duration < 10 || duration > 120)
    {
        duration = 30;
    }

    if (runHelpPage)
    {
        PrintHelp(argv[0]);
    }
}

void PrintHelp(const char * pr_name)
{
    std::cout << "Usage: " << pr_name << " [-t THREADS] [-d DURATION]\n"
                 "  THREADS\tCount of processing threads (2-10). Default 4.\n"
                 "  DURATION\tDuration of executing in seconds(10-120). Default 30."
              << std::endl;
}
