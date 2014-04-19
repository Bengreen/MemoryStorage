    //
    //  objectLock.cpp
    //  MemoryStorage
    //
    //  Created by Benjamin Greene on 30/03/2014.
    //  Copyright (c) 2014 Benjamin Greene. All rights reserved.
    //

#include "objectGlobalLock.h"

objectGlobalLock::objectGlobalLock(std::string name) : name(name){
        //Doing proper initinialise to ensure that mutex is enabled on construction.
    
    int attr_ret  =pthread_mutexattr_init(&mta);
    int mutex_ret = pthread_mutex_init(&mutex, &mta);
    
    if (!attr_ret && !mutex_ret)
        std::cout<<"MUTEX: "<<name<<" created"<<std::endl;
}
objectGlobalLock::~objectGlobalLock() {
    int mutex_ret = pthread_mutex_destroy(&mutex);
    
    if (!mutex_ret)
        std::cout<<"MUTEX: "<<name<<" destroyed"<<std::endl;
}
void objectGlobalLock::readLock() {
        //std::cout<<"MUTEX: "<<name<<" locking"<<std::endl;
    pthread_mutex_lock(&mutex);
    
}
void objectGlobalLock::readRelease() {
        //std::cout<<"MUTEX: "<<name<<" release"<<std::endl;
    pthread_mutex_unlock(&mutex);
}
void objectGlobalLock::writeLock() {
        //std::cout<<"MUTEX: "<<name<<" locking"<<std::endl;
    pthread_mutex_lock(&mutex);
    
}
void objectGlobalLock::writeRelease() {
        //std::cout<<"MUTEX: "<<name<<" release"<<std::endl;
    pthread_mutex_unlock(&mutex);
}


