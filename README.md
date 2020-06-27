# sleeping-TA

This program does some things with pthreads. You enter the number of students and the length of time the program will run at command line. The program creates a thread for each student and a thread for the TA. At random times students show up and try to see the TA. If they see the TA the TA thread becomes locked and subsequents student must sit ouside in a queue. If a student arrives and the queue is full the thread leaves and comes back later. The TA comes out and accepts a studnet at random times. If the TA emerges and there is no student then the TA goes to sleep. 
