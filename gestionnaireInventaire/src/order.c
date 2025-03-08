#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <utils.h>
#include <order.h>
#include <path.h>

void *threadWaitOrder(void *arg){
    while(1){
        pause();
    }
}

int openSharedMemory(void **pShm){
    // Open or create the shared memory segment
    int shmFd = shm_open(SHM_ORDER, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    CHECK(shmFd, "Failed to open shared memory");

    // Associate the memory segment with the process's address space
    CHECK(ftruncate(shmFd, SHM_SIZE), "Failed to set the size of shared memory");

    *pShm = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
    CHECK_NULL(*pShm, "Failed to map shared memory");

    // Close the file descriptor after mapping
    CHECK(close(shmFd), "Failed to close shared memory file descriptor");

    return shmFd;
} 

void newOrder(){
    // read shared memory
    void *pShm = NULL;
    int shmFd = openSharedMemory(&pShm);
    

    // Open or create the shared memory segment

    
    // Copy the content into a buffer
    char buffer[SHM_SIZE];
    memcpy(buffer, pShm, SHM_SIZE);

    DEBUG_PRINT("New order received: %s\n", buffer);

    // Unmap and close the shared memory segment
    munmap(pShm, SHM_SIZE);
    close(shmFd);

    
    // Process the order
    
}