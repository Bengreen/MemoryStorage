//
//  objectLock.h
//  MemoryStorage
//
//  Created by Benjamin Greene on 30/03/2014.
//  Copyright (c) 2014 Benjamin Greene. All rights reserved.
//

#ifndef __MemoryStorage__objectGlobalLock__
#define __MemoryStorage__objectGlobalLock__

#include <iostream>
#include <pthread.h>
#include <string>


class objectGlobalLock {
private:
    pthread_mutex_t    mutex;
    pthread_mutexattr_t   mta;
protected:
    std::string name;
public:
    objectGlobalLock(std::string name);
    ~objectGlobalLock();
    
    void readLock();
    void readRelease();
    void writeLock();
    void writeRelease();
private:
        //Remove out ALL of the default methods
    objectGlobalLock() = delete;
    objectGlobalLock(const objectGlobalLock& ref) = delete;
    objectGlobalLock& operator=(const objectGlobalLock& ref) = delete;
};


#endif /* defined(__MemoryStorage__objectGlobalLock__) */
