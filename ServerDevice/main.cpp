#include <iostream>
#include "serverdevice.h"

#include "cmdparser.h"
#include "mediator.h"

int main()
{
    Mediator mediator;
    mediator.start();
    return 0;
}
