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



int numThreadsWrite(5);
int numThreadsRead(5);
int entriesAddedPerThread(10000);

struct batchObject_t {
    column<int>* colObj;
    int colNumber;
    int entriesPerThread;
    pthread_t threadId;
    bool doRead;
};


void* columnInsertBatch(void* columnObject) {
    
    batchObject_t* myObject((batchObject_t*)columnObject);
    
    std::cout<<"Starting WRITE thread "<<myObject->colNumber<<std::endl;
    
    for(int i(0);i<entriesAddedPerThread;++i) {
        myObject->colObj->insert(i);
    }
    
    std::cout<<"Ending WRITE thread "<<myObject->colNumber<<std::endl;
    return 0;
}

void* columnRead(void* columnObject) {
    
    batchObject_t* myObject((batchObject_t*)columnObject);
    
    std::cout<<"Starting READ thread "<<myObject->colNumber<<std::endl;
    int count(0);
    
    while (myObject->doRead) {
        myObject->colObj->sumAll();
        ++count;
    }
    
    
    std::cout<<"Ending READ thread "<<myObject->colNumber<<" with "<<count<<" reads"<<std::endl;
    return 0;
}


int main(int argc, const char * argv[])
{
        // TODO: Add thread running code
    std::cout << "Hello, World!\n";
    
  
    {
        batchObject_t threadsWrite[numThreadsWrite],
            threadsRead[numThreadsRead];
        
        
        
        column<int> colObj("PARALLEL HOLDER");
        
            //pthread_t columnThreads[NUM_THREADS];
        
        for(int i(0);i<numThreadsWrite;++i) {
            threadsWrite[i].entriesPerThread=entriesAddedPerThread;
            threadsWrite[i].colObj=&colObj;
            threadsWrite[i].colNumber=i;
            
            pthread_create(&threadsWrite[i].threadId, NULL, columnInsertBatch, &threadsWrite[i]);
        }
        
        for(int i(0);i<numThreadsRead;++i) {
            threadsRead[i].doRead=true;
            threadsRead[i].colObj=&colObj;
            threadsRead[i].colNumber=i;
            
            pthread_create(&threadsRead[i].threadId, NULL, columnRead, &threadsRead[i]);
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
    /* Last thing that main() should do */
    
    
    return 0;
}



