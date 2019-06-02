#include "sterragiven.h"
#include "helpers.h"

Request *GetRequest(Stopper stopSignal) noexcept
{
    if (stopSignal.isEnd())
    {
        return nullptr;
    }

    return RequestsGenerator();
}

bool Stopper::isNotified() const
{
    return _notified;
}

void Stopper::setNotified(bool value)
{
    _notified = value;
}

Stopper::Stopper() : _endSign(false), _notified(false)
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
        return;
    }

    ProcessEmulator(request);
}

size_t Request::id() const
{
    return _id;
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
