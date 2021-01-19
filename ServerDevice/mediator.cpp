#include "mediator.h"

Mediator::Mediator()
{
    logger = ServerDeviceLogger::getInstance();
    multimeter = MultimeterEmulator::getInstance();
}

Mediator::~Mediator()
{

}

void Mediator::start()
{
    if ( !server.start() )
        logger->logInfo(LOG_TYPE::LOG_ERROR,"Server device not started." );
}
