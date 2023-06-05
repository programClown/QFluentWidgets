#include "SignalBus.h"

SignalBus::SignalBus(QObject *parent) : QObject(parent) { }

SignalBus *SignalBus::signalBus()
{
    static SignalBus *bus = nullptr;
    if (bus == nullptr) {
        bus = new SignalBus();
    }
    return bus;
}
