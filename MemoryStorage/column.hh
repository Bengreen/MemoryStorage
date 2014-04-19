    //
    //  column.h
    //  MemoryStorage
    //
    //  Created by Benjamin Greene on 30/03/2014.
    //  Copyright (c) 2014 Benjamin Greene. All rights reserved.
    //

#ifndef __MemoryStorage__column__
#define __MemoryStorage__column__

#include <iostream>
#include <iostream>
#include <list>

#include "objectGlobalLock.h"
#include "objectReadWriteLock.h"

enum storageType {
    globalLock,
    readWriteLock,
    atomicWrite
};


template<storageType stoType>
class sto_traits {
};

template<>
class sto_traits<globalLock> {
public:
    typedef  objectGlobalLock stoType;
};







template<typename storage_t,
storageType stoType=globalLock>
class column {};



template <typename storage_t>
class column<storage_t, globalLock>: objectGlobalLock {
private:
    int writeNum;
    typedef std::list<storage_t> listType_t;
    listType_t myList;
public:
    column(std::string name): objectGlobalLock(name), writeNum(0) {
        std::cout<<"Constructing column["<<name<<"]"<<std::endl;
    }
    
    ~column() {
        std::cout<<"Destructing column["<<name<<"]"<<std::endl;
    }
    
    void writeInc() {
        writeLock();
        ++writeNum;
        writeRelease();
    }
    void writeDec() {
        writeLock();
        --writeNum;
        writeRelease();
    }
    
        // -------- Specialised for storage type
    void insert(storage_t& object) {
        writeLock();
        myList.push_back(object);
        writeRelease();
    }
    unsigned long size() const {
        return myList.size();
    }
    
    int sumAll() {
        int sum(0);
        readLock();
        for (typename listType_t::iterator myIter(myList.begin()); myIter!=myList.end(); ++myIter) {
            sum+=*myIter;
        }
        readRelease();
        
        return sum;
    }
    
        // ------- End of Specialised storage type
    
private:
    column() = delete;
    column(const column& other)=delete;
    column& operator=(const column& other)=delete;
};


template <typename storage_t>
class column<storage_t, readWriteLock>: objectReadWriteLock {
private:
    int writeNum;
    typedef std::list<storage_t> listType_t;
    listType_t myList;
public:
    column(std::string name): objectReadWriteLock(name), writeNum(0) {
        std::cout<<"Constructing column["<<name<<"]"<<std::endl;
    }
    
    ~column() {
        std::cout<<"Destructing column["<<name<<"]"<<std::endl;
    }
    
    void writeInc() {
        writeLock();
        ++writeNum;
        writeRelease();
    }
    void writeDec() {
        writeLock();
        --writeNum;
        writeRelease();
    }
    
        // -------- Specialised for storage type
    void insert(storage_t& object) {
        writeLock();
        myList.push_back(object);
        writeRelease();
    }
    unsigned long size() const {
        return myList.size();
    }
    
    int sumAll() {
        int sum(0);
        readLock();
        for (typename listType_t::iterator myIter(myList.begin()); myIter!=myList.end(); ++myIter) {
            sum+=*myIter;
        }
        readRelease();
        
        return sum;
    }
    
        // ------- End of Specialised storage type
    
private:
    column() = delete;
    column(const column& other)=delete;
    column& operator=(const column& other)=delete;
};



#endif /* defined(__MemoryStorage__column__) */
