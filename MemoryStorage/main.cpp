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



static const int numThreadsWrite(5);
static const int numThreadsRead(10);
int entriesAddedPerThread(20000);

template<typename colType>
struct batchObject_t {
    colType* colObj;
    int colNumber;
    int entriesPerThread;
    pthread_t threadId;
    bool doRead;
    bool *waitStart;
    int numRuns;
};

template<typename colType>
void* columnInsertBatch(void* columnObject) {
    
    batchObject_t<colType>* myObject((batchObject_t<colType>*)columnObject);
    
    std::cout<<"Starting WRITE thread "<<myObject->colNumber<<std::endl;
    
    while(myObject->waitStart)
        ;
    
    for(int i(0);i<entriesAddedPerThread;++i) {
        myObject->colObj->insert(i);
    }
    
    std::cout<<"Ending WRITE thread "<<myObject->colNumber<<std::endl;
    return 0;
}

template<typename colType>
void* columnRead(void* columnObject) {
    
    batchObject_t<colType>* myObject((batchObject_t<colType>*)columnObject);
    
    std::cout<<"Starting READ thread "<<myObject->colNumber<<std::endl;
    int count(0);
    
    while(myObject->waitStart)
        ;
    
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
    
    colType colObj("PARALLEL HOLDER");
    
        //pthread_t columnThreads[NUM_THREADS];
    
    for(int i(0);i<numThreadsWrite;++i) {
        threadsWrite[i].entriesPerThread=entriesAddedPerThread;
        threadsWrite[i].colObj=&colObj;
        threadsWrite[i].colNumber=i;
        threadsWrite[i].waitStart=&waitStart;
        
        pthread_create(&threadsWrite[i].threadId, NULL, columnInsertBatch<colType>, &threadsWrite[i]);
    }
    
    for(int i(0);i<numThreadsRead;++i) {
        threadsRead[i].doRead=true;
        threadsRead[i].colObj=&colObj;
        threadsRead[i].colNumber=i;
        threadsRead[i].waitStart=&waitStart;
        
        pthread_create(&threadsRead[i].threadId, NULL, columnRead<colType>, &threadsRead[i]);
    }
    
        //Threads are all started.
        //Now release barrier
    waitStart=false;
    
        //Threads running in parallel at this point
    
        //Collect stopped Writes
    for(int i(0);i<numThreadsWrite;++i) {
        pthread_join(threadsWrite[i].threadId, NULL);
    }
    
        //When Writes are complete then stop reads
    for(int i(0);i<numThreadsRead;++i) {
        threadsRead[i].doRead=false;
    }
    
        //Collect stopped Reads
    for(int i(0);i<numThreadsRead;++i) {
        pthread_join(threadsRead[i].threadId, NULL);
        std::cout<<"Ending READ thread "<<threadsRead[i].colNumber<<" with "<<threadsRead[i].numRuns<<" reads"<<std::endl;
    }
    
    std::cout<<"Column contains :"<<colObj.size()<<std::endl;
    
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



