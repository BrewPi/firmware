#include "SmartPtrContainer.h"

class TestTarget;
template class SmartPtrContainer<TestTarget>;

class TestTarget{
public:
    TestTarget(int b_) : b(b_){}
    ~TestTarget() = default;

    int16_t b;
};

class TestDriver;
template class SmartPtrContainer<TestDriver>;

class TestDriver{
    using CtTestTarget = SmartPtrContainer<TestTarget>;

public:
   TestDriver(CtTestTarget & target_) : target(target_), a(1){
    }
    ~TestDriver() = default;

   CtTestTarget & target;
   int16_t a;

   int16_t calc(){
       return a * target->b; // demo using reference as if it was a raw pointer to TestTarget
   }
};

