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




#define NUM_THREADS     5

void *PrintHello(void *threadid)
{
    long tid;
    tid = (long)threadid;
    std::cout<<"Hello World! Its me, thread "<<tid<<"!"<<std::endl;
        //    printf("Hello World! It's me, thread #%ld!\n", tid);
    pthread_exit(NULL);
}



int main(int argc, const char * argv[])
{
    // TODO: Add thread running code
    std::cout << "Hello, World!\n";
    
    
    
    
    pthread_t threads[NUM_THREADS];
    int rc;

    for(long t=0; t<NUM_THREADS; t++){

        std::cout<<"Creating thread "<<t<<std::endl;
        
        rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
        if (rc){
            std::cout<<"ERROR: return code :"<<rc<<std::endl;
            exit(-1);
        }
    }
    
    
    std::cout<<"GOT HERE"<<std::endl;
    
    column<int> *colObj;
    
    colObj= new column<int>("test");

    delete colObj;
    
    std::cout<<"END"<<std::endl;
    
    /* Last thing that main() should do */
    pthread_exit(NULL);

    
    return 0;
}



