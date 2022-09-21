#include<TsetlinMachine.h>
#include<functional>
#include<string.h>
#include<iostream>
#include <bitset>

TSetlin::TSetlin(uint32_t forgetTh_  , uint32_t  memorizeTh_  , uint32_t numberOfPositiveLiterals_){
  memory = new uint8_t[numberOfPositiveLiterals_];
  memset(memory,85,(size_t) numberOfPositiveLiterals_);
  forgetTh   = forgetTh_;
  memorizeTh = memorizeTh_;
  numberOfPositiveLiterals = numberOfPositiveLiterals_;
  maskSize = numberOfPositiveLiterals*2;
  uint32_t inBytes = maskSize/8;
  if (maskSize > (inBytes*8))
    inBytes++;
  maskSize=inBytes;
  conditionMask = new uint8_t[inBytes];
  forgetMask    = new uint8_t[inBytes];
}
bool TSetlin::literalIsValid(uint32_t literal){
   return literal < numberOfPositiveLiterals && literal >= 0;
}

bool TSetlin::getLiteralMemoryValue(uint32_t literal){
  if (!literalIsValid(literal)){
      return false;
  }
  uint8_t cbyte = memory[literal];
  literalRegister[0]=cbyte &0x0F;
  literalRegister[1]=cbyte >> 4;
  return true;
}

bool TSetlin::operate(uint32_t  literal ,bool negative,bool decrease){
    bool got = getLiteralMemoryValue(literal);
    if(!got){
      return false;
    }
    uint8_t f= literalRegister[negative ?  1 : 0 ] ;
    if (f ==10 && !decrease || f==0 && decrease) {
      return false;
    }
    f = decrease ? f-1 : f+1;
    if (negative){
      memory[literal] = (memory[literal] & 0x0F) | (f <<4);
    }else{
      memory[literal] = (memory[literal] & 0xF0) | f;
    }
    return true;
}

bool TSetlin::decrease(uint32_t literal ,bool negative){
  return operate(literal, negative, true);
}

bool TSetlin::increase(uint32_t literal ,bool negative){
  return operate(literal, negative, false);
}

bool TSetlin::memorize(uint32_t literal , bool negative){
  if (random()%10> memorizeTh) return false;
  return increase(literal,negative);
}

bool TSetlin::forget(uint32_t literal , bool negative){
  if (random()%10 > forgetTh) return false;
  return decrease(literal,negative);
}

bool TSetlin::iterateOverLiterals(std::function<bool(uint32_t , 
      uint8_t ,uint8_t ,uint32_t , uint8_t  )> callback){
  for(uint32_t l=0; l != numberOfPositiveLiterals;l++){
    uint32_t cByte  = l/4;
    uint8_t cBit    = l%4;
    uint8_t mask    = 1 << (cBit*2);
    uint8_t notmask = 1 << (cBit*2+1);
    if (!callback(cByte,mask,notmask,l,cBit))
      return false;
  }
  return true;
}


bool TSetlin::evaluateCondition(uint8_t* observation){
  return ITERATE_OVER_LITERALS(
      if ((conditionMask[cByte] & mask) == mask && (observation[cByte] & mask)  != mask)
        return false;
      if ((conditionMask[cByte] & notmask) == notmask && (observation[cByte] & mask)  == mask)
        return false;
      return true;
  );
}

void TSetlin::typeIIFeedback(uint8_t *observation ){
  if (!evaluateCondition(observation)) return;
  ITERATE_OVER_LITERALS(
    if ((observation[cByte] & mask) !=mask)
      increase(l,false);
    else
      increase(l,true);
    return true;
  );
  updateConditionMask();
}

void TSetlin::typeIFeedback(uint8_t *observation){
  memset(forgetMask,0xff,maskSize);
  if (evaluateCondition(observation)){
    ITERATE_OVER_LITERALS(
        if ((observation[cByte] & mask)  == mask){
          memorize(l,false);
          forgetMask[cByte] ^=mask;
        }else {
          memorize(l,true);
          forgetMask[cByte] ^=notmask;
        }
        return true;
   );
  }
  ITERATE_OVER_LITERALS( 
      if ((forgetMask[cByte] & mask)  == mask){
        forget(l,false);
      }
      if ((forgetMask[cByte] & notmask)  == notmask){
        forget(l,true);
      }
      return true;
  );
  updateConditionMask();
}
uint32_t TSetlin::choose(uint32_t options){
  return random()%options;
}
void TSetlin::train(std::vector<uint8_t*>& targetClass, std::vector<std::vector<uint8_t*>>& otherClasses, 
    uint32_t epochs){
  updateConditionMask();
  for (int e=0; e !=epochs; e++){
    if (random()%2){
      typeIFeedback(targetClass[choose(targetClass.size())]);
    }else{
      std::vector<uint8_t*> other = otherClasses[choose(otherClasses.size())];
      typeIIFeedback(other[choose(other.size())]);
    }
  }
}

void TSetlin::updateConditionMask(){
  memset(conditionMask,0,maskSize);
   ITERATE_OVER_LITERALS(
      bool exists =getLiteralMemoryValue(l);
      if (exists)
        conditionMask[cByte] |= ((literalRegister[0] >= 6 ? 1:0) << cBit*2) | 
                                ((literalRegister[1] >=6 ? 1:0) << (cBit*2+1)); 
      return true;
   );
}

void TSetlin::dumpRules(){
  updateConditionMask();
  for(int b=0; maskSize != b ; b++){
    uint8_t byte = conditionMask[b];
    std::cout <<  std::bitset<8>(byte) << " ";
  }
  std::cout << std::endl;
  for(int l=0; numberOfPositiveLiterals!=l; l++){
    getLiteralMemoryValue(l);
    std::cout << (int)literalRegister[0]  << " " << (int)literalRegister[1] << std::endl;
  }
  std::cout << std::endl;
}
