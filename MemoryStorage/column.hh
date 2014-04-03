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

#include "objectLock.h"

template <typename storage_t>
class column : objectLock {
private:
    int writeNum;
    std::list<storage_t> myList;
public:
    column(std::string name): objectLock(name), writeNum(0) {
        std::cout<<"Constructing column["<<name<<"]"<<std::endl;
    }

    ~column() {
        std::cout<<"Destructing column["<<name<<"]"<<std::endl;
    }
    
    void writeInc() {
        lock();
        ++writeNum;
        release();
    }
    void writeDec() {
        lock();
        --writeNum;
        release();
    }
    
        // -------- Specialised for storage type
    void insert(storage_t& object) {
            lock();
        myList.push_back(object);
            release();
    }
    unsigned long size() const {
        return myList.size();
    }
    
        // ------- End of Specialised storage type
    
private:
    column() = delete;
    column(const column& other)=delete;
    column& operator=(const column& other)=delete;
};


#endif /* defined(__MemoryStorage__column__) */
