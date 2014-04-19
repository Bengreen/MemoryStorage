//
//  objectReadWriteLock.h
//  MemoryStorage
//
//  Created by Benjamin Greene on 12/04/2014.
//  Copyright (c) 2014 Benjamin Greene. All rights reserved.
//

#ifndef __MemoryStorage__objectReadWriteLock__
#define __MemoryStorage__objectReadWriteLock__

#include <iostream>
#include <pthread.h>
#include <string>


class objectReadWriteLock {
private:
    pthread_rwlock_t    mutex;
    pthread_rwlockattr_t   mta;
protected:
    std::string name;
public:
    objectReadWriteLock(std::string name);
    ~objectReadWriteLock();
    
    void readLock();
    void readRelease();
    void writeLock();
    void writeRelease();
private:
        //Remove out ALL of the default methods
    objectReadWriteLock() = delete;
    objectReadWriteLock(const objectReadWriteLock& ref) = delete;
    objectReadWriteLock& operator=(const objectReadWriteLock& ref) = delete;
};




#endif /* defined(__MemoryStorage__objectReadWriteLock__) */
