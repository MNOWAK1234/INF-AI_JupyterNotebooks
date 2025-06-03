To run the program, use the `make.sh` script and provide 2 parameters:

1.  **n**: The number of people (processes)
2.  **k**: The maximum number of messages in the queue

First, `n` processes are created, and each is assigned a role.
Then, writers attempt to write, and readers try to read.
Mutual exclusion is implemented using 4 semaphores:

- **0**: Is there space to write?
- **1**: Is there space to read?
- **2**: Exclusive write access
- **3**: Exclusive read access

Additionally, we have 2 extra semaphores:

- **5**: For checking the current status of all people (a writer needs to know who to send messages to).
- **4**: For checking how many people are deadlocked.

The fourth semaphore is a crucial element of the program's logic. This is because, at the very beginning, everyone will try to read, but no one has written anything yet. If we were to allow all people to deadlock while waiting to read, we'd allow someone to give up on trying to use the "reading room." This way, they can change their role and become a writer. A writer also needs to read something first, so deadlock would occur again. Therefore, we anticipate that a writer will eventually acquire the raising semaphore, "bypass" the reading room, and start writing.

An analogous reasoning is applied when too many people want to write and there's no space. In such a case, we discourage a writer from writing and allow them to become a reader, which might unblock the system.
