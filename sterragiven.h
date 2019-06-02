#pragma once

#include <string>

class Request
{
    size_t        _id;
    std::string   _data;

    size_t getCounter();

public:

    Request();
    Request(const std::string &data);

    std::string data() const;
    size_t id() const;
};

class Stopper
{
    bool _endSign;
    bool _notified;
public:
    Stopper();

    bool isEnd() const;
    void stop();

    bool isNotified() const;
    void setNotified(bool value);
};

// возвращает NULL, если объект stopSignal указывает на необходимость остановки,
// либо указатель на память, которую в дальнейшем требуется удалить
Request* GetRequest(Stopper stopSignal) noexcept;

// обрабатывает запрос, но память не удаляет, завершает обработку досрочно, если
// объект stopSignal указывает на необходимость остановки
void ProcessRequest(Request* request, Stopper stopSignal) noexcept;
