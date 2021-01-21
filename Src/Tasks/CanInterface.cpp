#include <CanInterface.hpp>
#include <Hardware.hpp>

CanInterface::CanInterface() : Task("", 500, 5){

}

void CanInterface::initialize() {
    Hardware::initializeCan();
}

void CanInterface::run() {
    CAN::TxMessage msg(0x1234);
    msg.payload = {1, 2, 3, 4, 5, 6, 7, 8};
    Hardware::sendCanMessage(msg);
    const auto m = Hardware::receiveCanMessage();
    if(m){
        const auto x = m.value();
        printf("%lu", x.getId());
    }
}
