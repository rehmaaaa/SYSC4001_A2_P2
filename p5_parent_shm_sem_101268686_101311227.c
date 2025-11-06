#define _XOPEN_SOURCE 700
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { int multiple, counter, done; } shm_data;

static int semid=-1;
static void sem_lock(void){ struct sembuf P={0,-1,0}; if(semop(semid,&P,1)==-1){perror("semop P"); _exit(2);} }
static void sem_unlock(void){ struct sembuf V={0,+1,0}; if(semop(semid,&V,1)==-1){perror("semop V"); _exit(2);} }

int main(int argc, char **argv){
    int multiple = (argc>1)? atoi(argv[1]) : 3;

    key_t shm_key = ftok(".", 'S'); if(shm_key==-1){perror("ftok shm"); return 1;}
    key_t sem_key = ftok(".", 'M'); if(sem_key==-1){perror("ftok sem"); return 1;}

    int shmid = shmget(shm_key, sizeof(shm_data), IPC_CREAT|0600);
    if(shmid<0){perror("shmget parent"); return 1;}
    shm_data* shp = (shm_data*)shmat(shmid,NULL,0);
    if(shp==(void*)-1){perror("shmat parent"); return 1;}

    semid = semget(sem_key, 1, IPC_CREAT|0600);
    if(semid<0){perror("semget parent"); return 1;}
    if(semctl(semid, 0, SETVAL, 1)==-1){perror("semctl SETVAL"); return 1;}

    // Initialize shared vars under lock
    sem_lock(); shp->multiple = multiple; shp->counter = 0; shp->done = 0; sem_unlock();

    pid_t pid = fork();
    if(pid<0){perror("fork"); return 1;}
    if(pid==0){
        execl("./p5_child_shm_sem", "p5_child_shm_sem", (char*)NULL);
        perror("execl p5_child_shm_sem"); _exit(127);
    }

    printf("[P1 parent] started (multiple=%d)\n", multiple);
    for(;;){
        sem_lock();
        if(shp->done){ sem_unlock(); break; }
        shp->counter += shp->multiple;
        int c=shp->counter;
        printf("[P1 parent] counter=%d\n", c);
        if(c>500){ shp->done=1; sem_unlock(); break; }
        sem_unlock();
        usleep(30000);
    }

    int status=0; waitpid(pid,&status,0);

    // Cleanup IPC
    shmdt(shp);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);
    printf("[P1 parent] cleaned up shm & semaphore. Exiting.\n");
    return 0;
}
