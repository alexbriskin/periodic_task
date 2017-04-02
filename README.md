# periodic_task
This repository implements periodic tasks API using POSIX RT signals.
The signals that I've used are SIGRTMIN (33) to SIGRTMAX (64), so you cannot have more than 32 timers +per process+.

For some altervatives as well please see:
http://www.2net.co.uk/tutorial/periodic_threads
