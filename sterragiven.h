#pragma once

#include <string>

class Request
{
    size_t        _id;
    std::string   _data;

    size_t getCounter();

    struct Logger
    {
        size_t started;
        size_t executed;
        size_t dryRuns;
    };


public:

    Request();
    Request(const std::string &data);

    std::string data() const;
    size_t id() const;

    static Logger &getLogger();
};

class Stopper
{
    // Так как в ProcessRequest() Stopper передаётся по значению,
    // а в процессе работы хочется знать актуальное состояние,
    // флаг конца будем хранить по ссылке.
    bool &_endSign;
public:
    Stopper(bool &endFlag);

    bool isEnd() const;
    void stop();
};

// возвращает NULL, если объект stopSignal указывает на необходимость остановки,
// либо указатель на память, которую в дальнейшем требуется удалить
Request* GetRequest(Stopper stopSignal) noexcept;

// обрабатывает запрос, но память не удаляет, завершает обработку досрочно, если
// объект stopSignal указывает на необходимость остановки
void ProcessRequest(Request* request, Stopper stopSignal) noexcept;
