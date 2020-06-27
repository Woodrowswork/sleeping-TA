#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>

#define TRUE 1
#define SEATS 3

pthread_mutex_t mutex;

sem_t full, empty;

int line[SEATS];
int counter;

pthread_t tid;
pthread_attr_t attr;

void *ta(void *param);
void *student(void *param);
int getinline(int studentid);
int getinoffice();

void *student(void *param){
	int studentid;

    while(TRUE){
        //studentid = pthread_self();//how do we ask for the thread id?
	
        studentid = (int*)param;
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        if(getinline(studentid) == -1) {
            fprintf(stderr, "This student could not get a seat and left to program.%d\n",studentid);
            sleep(.001);
        }
            if(getinline(studentid) == -2){
                fprintf(stderr, "Student had to wake up the TA.\n");
            }
        else
            printf("This student took a seat %d\n", studentid);

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }

}

void *ta(void *param){
    int studentid;

    while(TRUE){
	int studentid;
        sem_wait(&full);

        pthread_mutex_lock(&mutex);
	studentid = getinoffice();
        if(studentid == -1){
            fprintf(stderr, "No students found in line TA has gone to sleep.\n");
        }
        else {
            printf("This student saw the TA. %d\n", studentid);
        }
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int getinline(int studentid){
    if (counter > 0 && counter < SEATS){
        line[counter] = studentid;
        counter++;
        return -2;
    }
        if(counter == 0){
            line[counter] = studentid;
            counter++;
            return 0;
        }
    else
        return -1;
}

int getinoffice(){
	int studentid;
    if (counter > 0){
        studentid = line[(counter-1)];
        counter--;
        return studentid;
    }
    else
        return -1;
}
int main(int argc, char *argv[]) {
    
    int runtime = atoi(argv[1]);
    int numStudents = atoi(argv[2]);
    //printf ("Student number is %d\n", numStudents);
    pthread_mutex_init(&mutex, NULL);


    pthread_t TAthread;
    pthread_t StudThread[numStudents];

    sem_init(&full, 0, 0);

    sem_init(&empty, 0, SEATS);

    pthread_attr_init(&attr);

    counter = 0;

    pthread_create(&TAthread, &attr, ta, NULL);

    int i;
    for(i = 0; i < numStudents; i++)
	pthread_create(&StudThread[i], &attr, student, (void*) (i+1));

    sleep(runtime);
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&mutex);


    return 0;
}
