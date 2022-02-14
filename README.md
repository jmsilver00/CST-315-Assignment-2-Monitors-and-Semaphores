To compile and run code:

For the semaphore program readwriteSem.c
Compile: gcc readwriteSem.c -pthread
Run: ./a.out

For the monitor program readwriteMon.cpp
Compile: g++ -o mon -pthread readwriteMon.cpp
Run: ./mon

In this assignment we addressed the reader-writer problem. The reader writer problem can be summarized as multiple processes attempting to share an object, in this case the processes are the readers and writers. The problem deals with synchronization, the program shares a set of data among the process where only one writer can write at a time and if a single reader is reading than no writing can occur. 

Readers Writers Problem Using Semaphore and Mutex:
In this program the writer is restricted to writing only when the read count in the program is set to 0 or there are no readers actively waiting, the writer will get locked out if the reader performs a wait operation through the semaphores before the writer. The signal operation is used to let the writer know when the final reader has exited. For the reader when a writer begins its operation to set the read count to 0 then the readers will all get blocked and the current one will be set to wait. Both programs will have 10 writers and 10 readers, in this program the writer will multiply the existing value by 2 to show the reader and writer switching. 

Readers-Writers Problem using monitor structure:
To address the issue of a shared resource a monitor structure for mutual exclusion and pthreads were used. Condition variables were also used to halt the execution of a process until a specific condition is met, while the pthread mutual exclsioin lock was also used to control access to the shared resources. 

