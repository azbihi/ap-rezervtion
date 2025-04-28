#include "Reservation.h"
#include <sstream>

Reservation::Reservation() : reservation_id(0), passenger_id(0), flight_id(0), amount_paid(0.0), is_canceled(false) {}

Reservation::Reservation(int id, int passengerId, int flightId, double amount)
        : reservation_id(id), passenger_id(passengerId), flight_id(flightId), amount_paid(amount), is_canceled(false) {}

int Reservation::getReservationId() const { return reservation_id; }
int Reservation::getPassengerId() const { return passenger_id; }
int Reservation::getFlightId() const { return flight_id; }
double Reservation::getAmountPaid() const { return amount_paid; }
bool Reservation::isCanceled() const { return is_canceled; }

void Reservation::cancel() { is_canceled = true; }

