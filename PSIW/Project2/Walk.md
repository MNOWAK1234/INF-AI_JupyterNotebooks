To run the program, use the `make.sh` script and provide 3 parameters:

1.  **n**: The number of people walking
2.  **d**: The length of the walk
3.  **k**: The number of steps required from parent to child

First, `n` threads are created, and their roles are randomly assigned.
Then, in a mutual exclusion mode, each thread assigns itself a number, which also indicates the order of starting.
We place people along the route; otherwise, there's a high chance that all children will walk together first, then all parents, simplifying the problem.

We then allow people to move.
Before a person moves, they enter a critical section and check if moving to the next field is compliant with the rules.
If not, they release the critical section and wait for a signal.
A signal can be received when a thread from the opposite group makes a move; it will then send a signal to all threads waiting on the released field, indicating that they are now allowed to enter.
