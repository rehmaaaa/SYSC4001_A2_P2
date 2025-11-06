CC=gcc
CFLAGS=-O2 -Wall

all: p3_parent_wait p3_child p4_parent_shm p4_child_shm p5_parent_shm_sem p5_child_shm_sem

p3_parent_wait: p3_parent_wait_101268686_101311227.c
	$(CC) $(CFLAGS) -o $@ $<

p3_child: p3_child_101268686_101311227.c
	$(CC) $(CFLAGS) -o $@ $<

p4_parent_shm: p4_parent_shm_101268686_101311227.c
	$(CC) $(CFLAGS) -o $@ $<

p4_child_shm: p4_child_shm_101268686_101311227.c
	$(CC) $(CFLAGS) -o $@ $<

p5_parent_shm_sem: p5_parent_shm_sem_101268686_101311227.c
	$(CC) $(CFLAGS) -o $@ $<

p5_child_shm_sem: p5_child_shm_sem_101268686_101311227.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f p3_parent_wait p3_child p4_parent_shm p4_child_shm p5_parent_shm_sem p5_child_shm_sem *.o
