#ifndef __TSETLIN_MACHINE_H
#define __TSETLIN_MACHINE_H
#include<cstdint>
#include<random>
#include<time.h>
#include<vector>
#include <functional>
#define INITIAL_MEMORY_STATE 85
#define ITERATE_OVER_LITERALS(X)  iterateOverLiterals([&](uint32_t cByte,uint8_t mask,uint8_t notmask ,uint32_t l, uint8_t cBit){X})

class TSetlin{
  private:
    uint8_t *memory;
    uint32_t forgetTh;
    uint32_t memorizeTh;
    uint32_t numberOfPositiveLiterals;
    uint8_t literalRegister[2];
    uint8_t *conditionMask;
    uint8_t *forgetMask;
    uint32_t maskSize;
    void updateConditionMask();
  public:
     TSetlin(uint32_t forgetTh_  , uint32_t memorizeTh_  , uint32_t numberOfPositiveLiterals);
     bool literalIsValid(uint32_t number);
     bool getLiteralMemoryValue(uint32_t literal);
     bool operate(uint32_t  literal ,bool negative,bool decrease);
     bool decrease(uint32_t literal ,bool negative);
     bool increase(uint32_t literal ,bool negative);
     bool memorize(uint32_t literal , bool negative);
     bool forget(uint32_t literal , bool negative);
     void typeIFeedback(uint8_t *observation );
     void typeIIFeedback(uint8_t *Uint8Array);
     bool iterateOverLiterals(std::function <bool(uint32_t , 
                             uint8_t ,uint8_t ,uint32_t , uint8_t  )>callback);
     bool evaluateCondition(uint8_t* observation);
     uint32_t choose(uint32_t options);
     void train(std::vector<uint8_t*>& targetClass, std::vector<std::vector<uint8_t*>>& otherClasses, 
                uint32_t epochs);

     void dumpRules();
     

};

#endif 
