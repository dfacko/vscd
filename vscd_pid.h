#ifndef VSC_PID_H
#define VSC_PID_H

int write_pid();
void cleanup();
void handle_signal(int signal);
void register_terminate_signals();

#endif /* VSC_PID_H */
