#ifndef ROBOT-CONTROLLER_H_INCLUDED
#define ROBOT-CONTROLLER_H_INCLUDED

void signal_forward();
void signal_back();
void signal_left();
void signal_right();
int has_enough_time(void);
int connect_to_robot(void);
void disconnect(void);

#endif // ROBOT-CONTROLLER_H_INCLUDED
