    //
    //  main.cpp
    //  MemoryStorage
    //
    //  Created by Benjamin Greene on 23/03/2014.
    //  Copyright (c) 2014 Benjamin Greene. All rights reserved.
    //

#include <iostream>
#include <pthread.h>

#include "column.hh"
#include <atomic>



static const int numThreadsWrite(1);
static const int numThreadsRead(10);
int entriesPreloaded(2000);
int entriesAddedPerThread(20000);

pthread_cond_t  cond;    /* condition to wait on / signal */
pthread_mutex_t mtx;     /* mutex for the above */
pthread_cond_t managerWait;


template<typename colType>
struct batchObject_t {
    colType* colObj;
    int colNumber;
    int entriesPerThread;
    pthread_t threadId;
    bool doRead;
    bool *waitStart;
    int *threadCount;
    int numRuns;
};

template<typename colType>
void* columnInsertBatch(void* columnObject) {
    
    batchObject_t<colType>* myObject((batchObject_t<colType>*)columnObject);
    
    
    pthread_mutex_lock(&mtx);
        //std::cout<<"Starting WRITE thread "<<myObject->colNumber<<std::endl;
    
    --(*myObject->threadCount);
    pthread_cond_signal(&managerWait);
    
    while (*myObject->waitStart) {
        pthread_cond_wait(&cond,&mtx);
    }
    pthread_mutex_unlock(&mtx);
    

    for(int i(0);i<entriesAddedPerThread;++i) {
        myObject->colObj->insert(i);
    }
    
    std::cout<<"Ending WRITE thread "<<myObject->colNumber<<std::endl;
    return 0;
}

template<typename colType>
void* columnRead(void* columnObject) {
    
    batchObject_t<colType>* myObject((batchObject_t<colType>*)columnObject);
    
    int count(0);
    
    pthread_mutex_lock(&mtx);
        //std::cout<<"Starting READ thread "<<myObject->colNumber<<std::endl;
    --(*myObject->threadCount);
    pthread_cond_signal(&managerWait);

    while (*myObject->waitStart) {
        pthread_cond_wait(&cond,&mtx);
    }
    pthread_mutex_unlock(&mtx);
    
    while (myObject->doRead) {
        myObject->colObj->sumAll();
        ++count;
    }
    
    myObject->numRuns=count;
    
    return 0;
}

template<typename colType>
void runReadWriteTest() {
    
    batchObject_t<colType> threadsWrite[numThreadsWrite],
    threadsRead[numThreadsRead];
    
    bool waitStart=true;
    int unsetThreads=0;
    
    pthread_mutex_init(&mtx,NULL);
    
    pthread_cond_init(&cond,NULL);
    pthread_cond_init(&managerWait,NULL);

    
    colType colObj("PARALLEL HOLDER");
    
    for(int i(0);i<entriesPreloaded;++i) {
        colObj.insert(i);
    }
    
        //pthread_t columnThreads[NUM_THREADS];
    
    for(int i(0);i<numThreadsWrite;++i) {
        ++unsetThreads;
        threadsWrite[i].entriesPerThread=entriesAddedPerThread;
        threadsWrite[i].colObj=&colObj;
        threadsWrite[i].colNumber=i;
        threadsWrite[i].waitStart=&waitStart;
        threadsWrite[i].threadCount=&unsetThreads;
        
        pthread_create(&threadsWrite[i].threadId, NULL, columnInsertBatch<colType>, &threadsWrite[i]);
    }
    
    for(int i(0);i<numThreadsRead;++i) {
        ++unsetThreads;
        threadsRead[i].doRead=true;
        threadsRead[i].colObj=&colObj;
        threadsRead[i].colNumber=i;
        threadsRead[i].waitStart=&waitStart;
        threadsRead[i].threadCount=&unsetThreads;
        
        pthread_create(&threadsRead[i].threadId, NULL, columnRead<colType>, &threadsRead[i]);
    }
    
        //Threads are all started.
        //Now release barrier
    
    clock_t startTime, writeTime, readTime;
    

        // wait for threads to 'stage'
    pthread_mutex_lock( &mtx);
    while (unsetThreads != 0) {
        pthread_cond_wait( &managerWait, &mtx);
    }
    startTime=clock();
    
    waitStart=false;
    pthread_cond_broadcast(&cond);
    
    pthread_mutex_unlock( &mtx);

    
    
    std::cout<<"released barrier"<<std::endl;
    
        //Threads running in parallel at this point
    
        //Collect stopped Writes
    for(int i(0);i<numThreadsWrite;++i) {
        pthread_join(threadsWrite[i].threadId, NULL);
    }
    writeTime=clock();
    
        //When Writes are complete then stop reads
    for(int i(0);i<numThreadsRead;++i) {
        threadsRead[i].doRead=false;
    }
    
        //Collect stopped Reads
    for(int i(0);i<numThreadsRead;++i) {
        pthread_join(threadsRead[i].threadId, NULL);
        std::cout<<"Ending READ thread "<<threadsRead[i].colNumber<<" with "<<threadsRead[i].numRuns<<" reads"<<std::endl;
    }
    readTime=clock();
    
    std::cout<<"Column contains :"<<colObj.size()<<std::endl;
    std::cout<<"Write : "<<(writeTime-startTime)/CLOCKS_PER_SEC<<std::endl;
    std::cout<<"Read  : "<<( readTime-startTime)/CLOCKS_PER_SEC<<std::endl;
    
    std::cout<<"END"<<std::endl;
}


int main(int argc, const char * argv[])
{
    
    std::cout << "Hello, World!\n";
    
    std::cout<<"objectGlobalLock"<<std::endl;
    runReadWriteTest<column<int,globalLock>>();
    
    
    std::cout<<"objectReadWriteLock"<<std::endl;
    runReadWriteTest<column<int,readWriteLock>>();
    
    /* Last thing that main() should do */
    
    return 0;
}



