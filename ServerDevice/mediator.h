#ifndef MEDIATOR_H
#define MEDIATOR_H

#include "serverdevice.h"
#include "serverdevicelogger.h"
#include "multimeteremulator.h"

class Mediator
{
public:
    Mediator();
    ~Mediator();

    void start();

private:
    ServerDeviceLogger *logger{nullptr};
    ServerDevice server;
    MultimeterEmulator *multimeter{nullptr};
};

#endif // MEDIATOR_H
