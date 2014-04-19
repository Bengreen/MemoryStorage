//
//  objectReadWriteLock.cpp
//  MemoryStorage
//
//  Created by Benjamin Greene on 12/04/2014.
//  Copyright (c) 2014 Benjamin Greene. All rights reserved.
//

#include "objectReadWriteLock.h"


objectReadWriteLock::objectReadWriteLock(std::string name) : name(name){
        //Doing proper initinialise to ensure that mutex is enabled on construction.
    
    int attr_ret  =pthread_rwlockattr_init(&mta);
    int mutex_ret = pthread_rwlock_init(&mutex, &mta);
    
    if (!attr_ret && !mutex_ret)
        std::cout<<"MUTEX: "<<name<<" created"<<std::endl;
}
objectReadWriteLock::~objectReadWriteLock() {
    int mutex_ret = pthread_rwlock_destroy(&mutex);
    
    if (!mutex_ret)
        std::cout<<"MUTEX: "<<name<<" destroyed"<<std::endl;
}
void objectReadWriteLock::readLock() {
        //std::cout<<"MUTEX: "<<name<<" locking"<<std::endl;
    pthread_rwlock_rdlock(&mutex);
    
}
void objectReadWriteLock::readRelease() {
        //std::cout<<"MUTEX: "<<name<<" release"<<std::endl;
    pthread_rwlock_unlock(&mutex);
}
void objectReadWriteLock::writeLock() {
        //std::cout<<"MUTEX: "<<name<<" locking"<<std::endl;
    pthread_rwlock_wrlock(&mutex);
}
void objectReadWriteLock::writeRelease() {
        //std::cout<<"MUTEX: "<<name<<" release"<<std::endl;
    pthread_rwlock_unlock(&mutex);
}
