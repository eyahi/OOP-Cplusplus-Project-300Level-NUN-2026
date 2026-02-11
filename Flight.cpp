#include "Flight.h"

Flight::Flight(int i, QString d, int s) : id(i), destination(d), seats(s) {}

bool Flight::bookSeat() {
    if (seats > 0) {
        seats--;
        return true;
    }
    return false;
}
