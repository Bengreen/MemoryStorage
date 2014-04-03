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



#define NUM_THREADS     5
#define NUM_ENTRIES_PER_THREAD 100000

struct batchObject_t {
    column<int>* colObj;
    int colNumber;
    int entriesPerThread;
    pthread_t threadId;
};


void* columnInsertBatch(void* columnObject) {
    
    batchObject_t* myObject((batchObject_t*)columnObject);
    
    std::cout<<"Starting thread "<<myObject->colNumber<<std::endl;
    
    for(int i(0);i<NUM_ENTRIES_PER_THREAD;++i) {
        myObject->colObj->insert(i);
    }
    
    std::cout<<"Ending thread "<<myObject->colNumber<<std::endl;
    return 0;
}


int main(int argc, const char * argv[])
{
        // TODO: Add thread running code
    std::cout << "Hello, World!\n";
    
  
    {
        batchObject_t objects[NUM_THREADS];
        
        column<int> colObj("PARALLEL HOLDER");
        
            //pthread_t columnThreads[NUM_THREADS];
        
        for(int i(0);i<NUM_THREADS;++i) {
            objects[i].entriesPerThread=NUM_ENTRIES_PER_THREAD;
            objects[i].colObj=&colObj;
            objects[i].colNumber=i;
            
            pthread_create(&objects[i].threadId, NULL, columnInsertBatch, &objects[i]);
        }
            //Threads running in parallel at this point
        
        for(int i(0);i<NUM_THREADS;++i) {
            pthread_join(objects[i].threadId, NULL);
        }
        
        std::cout<<"Column contains :"<<colObj.size()<<std::endl;
        
        std::cout<<"END"<<std::endl;
    }
    /* Last thing that main() should do */
    
    
    return 0;
}



