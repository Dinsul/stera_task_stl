#include "sterragiven.h"
#include "helpers.h"

#include <random>
#include <thread>
#include <chrono>
#include <iostream>

Request *GetRequest(Stopper stopSignal) noexcept
{
    if (stopSignal.isEnd())
    {
        return nullptr;
    }

    return RequestsGenerator();
}

Stopper::Stopper(bool &endFlag)
    : _endSign(endFlag)
{
}


bool Stopper::isEnd() const
{
    return  _endSign;
}

void Stopper::stop()
{
    _endSign = true;
}

void ProcessRequest(Request *request, Stopper stopSignal) noexcept
{
    if (stopSignal.isEnd())
    {
        ++(Request::getLogger().dryRuns);

        return;
    }

    ++(Request::getLogger().started);

    std::random_device rand;

    // Имитируем выполнение разных действий.
    // Так как в задании сказано, что время работы
    // функци ProcessEmulator() может быть разным, будем
    // спать случайное время, но не больше 3 с.
    auto cyclesCount = rand() % 30;

    for (unsigned int i = 0; i < cyclesCount; ++i)
    {
        if (stopSignal.isEnd())
        {
            std::cout << "Request id #" << request->id()
                      << " was breaked." << std::endl;
            return;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << request->data() << std::endl;

    ++(Request::getLogger().executed);
}

size_t Request::id() const
{
    return _id;
}

Request::Logger &Request::getLogger()
{
    static Logger logger;

    return logger;
}

size_t Request::getCounter()
{
    static size_t _counter;

    return ++_counter;
}

Request::Request()
    : _id(getCounter()),
      _data("Hello, world! id:")
{
    _data.append(std::to_string(_id));
}

Request::Request(const std::string &data)
    : _id(getCounter()),
      _data(data)
{
    _data.append(" id:");
    _data.append(std::to_string(_id));
}

std::string Request::data() const
{
    return _data;
}
