#define _XOPEN_SOURCE 700
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { int multiple, counter, done; } shm_data;

static int semid=-1;
static void sem_lock(void){ struct sembuf P={0,-1,0}; if(semop(semid,&P,1)==-1){perror("semop P"); _exit(2);} }
static void sem_unlock(void){ struct sembuf V={0,+1,0}; if(semop(semid,&V,1)==-1){perror("semop V"); _exit(2);} }

int main(void){
    key_t shm_key = ftok(".", 'S'); if(shm_key==-1){perror("ftok shm"); return 1;}
    key_t sem_key = ftok(".", 'M'); if(sem_key==-1){perror("ftok sem"); return 1;}

    int shmid = shmget(shm_key, sizeof(shm_data), 0600);           // parent creates
    if(shmid<0){perror("shmget child"); return 1;}
    shm_data* shp = (shm_data*)shmat(shmid,NULL,0);
    if(shp==(void*)-1){perror("shmat child"); return 1;}

    semid = semget(sem_key, 1, 0600);                              // parent creates
    if(semid<0){perror("semget child"); return 1;}

    // Wait for counter > 100 (reads under lock)
    for(;;){
        sem_lock(); int c=shp->counter, d=shp->done; sem_unlock();
        if(d) break;
        if(c>100) break;
        usleep(20000);
    }
    printf("[P2 child] starting.\n");

    int last=-1;
    for(;;){
        sem_lock();
        int c=shp->counter, m=shp->multiple, d=shp->done;
        if(!d && c!=last){ printf("[P2 child] sees counter=%d (multiple=%d)\n", c, m); last=c; }
        if(c>500){ shp->done=1; d=1; }
        sem_unlock();

        if(d) break;
        usleep(20000);
    }

    shmdt(shp);
    return 0;
}
