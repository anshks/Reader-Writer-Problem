# Multiuser-Chat
The reader-writer problem is implemented using operations on a queue. Multiple readers can read at the same time. However they can not read the element being written to by the writer. Also during dequeue by the reader no reader can access that element. This is implemented using mutex locks.

Compiling: Just run the Makefile.

Testing: Just run the ./a.out.

Input: Enter the number of writers you want.

Output: The whole execution sequence will be displayed.

Errors Handled: 
1) The user needs to enter integer for the number of writers.
2) The number should be positive.

References: https://www.softprayog.in/programming/queue-implementation-in-c-using-linked-list
https://stackoverflow.com/questions/23094813/reader-writer-program-in-c-using-mutexes-and-pthreads
