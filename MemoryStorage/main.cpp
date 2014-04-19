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
static const int numThreadsRead(5);
int entriesAddedPerThread(10000);

template<typename colType>
struct batchObject_t {
    colType* colObj;
    int colNumber;
    int entriesPerThread;
    pthread_t threadId;
    bool doRead;
};

template<typename colType>
void* columnInsertBatch(void* columnObject) {
    
    batchObject_t<colType>* myObject((batchObject_t<colType>*)columnObject);
    
    std::cout<<"Starting WRITE thread "<<myObject->colNumber<<std::endl;
    
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
    
    while (myObject->doRead) {
        myObject->colObj->sumAll();
        ++count;
    }
    
    
    std::cout<<"Ending READ thread "<<myObject->colNumber<<" with "<<count<<" reads"<<std::endl;
    return 0;
}

template<typename colType>
void runReadWriteTest() {
    
    batchObject_t<colType> threadsWrite[numThreadsWrite],
    threadsRead[numThreadsRead];
    
    colType colObj("PARALLEL HOLDER");
    
        //pthread_t columnThreads[NUM_THREADS];
    
    for(int i(0);i<numThreadsWrite;++i) {
        threadsWrite[i].entriesPerThread=entriesAddedPerThread;
        threadsWrite[i].colObj=&colObj;
        threadsWrite[i].colNumber=i;
        
        pthread_create(&threadsWrite[i].threadId, NULL, columnInsertBatch<colType>, &threadsWrite[i]);
    }
    
    for(int i(0);i<numThreadsRead;++i) {
        threadsRead[i].doRead=true;
        threadsRead[i].colObj=&colObj;
        threadsRead[i].colNumber=i;
        
        pthread_create(&threadsRead[i].threadId, NULL, columnRead<colType>, &threadsRead[i]);
    }
    
    
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
    }
    
    std::cout<<"Column contains :"<<colObj.size()<<std::endl;
    
    std::cout<<"END"<<std::endl;
    
    
}


int main(int argc, const char * argv[])
{
        // TODO: Add thread running code
    std::cout << "Hello, World!\n";
    
    std::cout<<"objectGlobalLock"<<std::endl;

    runReadWriteTest<column<int,globalLock>>();
    
    std::cout<<"objectReadWriteLock"<<std::endl;
    
    runReadWriteTest<column<int,readWriteLock>>();
    
    /* Last thing that main() should do */
    
    
    return 0;
}



