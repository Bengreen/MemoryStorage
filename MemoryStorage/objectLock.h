//
//  objectLock.h
//  MemoryStorage
//
//  Created by Benjamin Greene on 30/03/2014.
//  Copyright (c) 2014 Benjamin Greene. All rights reserved.
//

#ifndef __MemoryStorage__objectLock__
#define __MemoryStorage__objectLock__

#include <iostream>
#include <pthread.h>
#include <string>


class objectLock {
private:
    pthread_mutex_t    mutex;
    pthread_mutexattr_t   mta;
protected:
    std::string name;
public:
    objectLock(std::string name);
    ~objectLock();
    
    void lock();
    void release();
private:
        //Remove out ALL of the default methods
    objectLock() = delete;
    objectLock(const objectLock& ref) = delete;
    objectLock& operator=(const objectLock& ref) = delete;
};


#endif /* defined(__MemoryStorage__objectLock__) */
