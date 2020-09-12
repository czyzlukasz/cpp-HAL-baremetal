#include <LedDebug.hpp>

LedDebug::LedDebug() : Task({}, 500, 0), i(){

}

void LedDebug::initialize() {
    i = 1234567;
}

void LedDebug::run() {
    ++i;
}
