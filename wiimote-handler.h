#ifndef EVENT-HANDLER_H_INCLUDED
#define EVENT-HANDLER_H_INCLUDED

#include "wiiuse.h"
#include "robot-controller.h"

void handle_disconnection(wiimote* wm);
void handle_event(wiimote* wm);
void handle_status(wiimote* wm);
void setup_wiimote(wiimote** wm);
short has_wiimote_connections(wiimote** wm, int wiimotes_number);

#endif // EVENT-HANDLER_H_INCLUDED
