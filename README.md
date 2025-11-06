# SYSC4001_A2_P2   
Teammates: 101268686, 101311227  

## Files

| Part | Description | Parent Program | Child Program |
|------|------------|----------------|--------------|
| 3 | wait + fork + exec | p3_parent_wait_101268686_101311227.c | p3_child_101268686_101311227.c |
| 4 | Shared memory (SysV SHM) | p4_parent_shm_101268686_101311227.c | p4_child_shm_101268686_101311227.c |
| 5 | Shared memory + semaphores (SysV) | p5_parent_shm_sem_101268686_101311227.c | p5_child_shm_sem_101268686_101311227.c |

---
## Build Commands

To compile everything:
```bash
make
make p3_parent_wait p3_child
make p4_parent_shm p4_child_shm
make p5_parent_shm_sem p5_child_shm_sem
make clean
./p3_parent_wait
./p4_parent_shm 3
./p5_parent_shm_sem 3

