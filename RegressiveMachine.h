#ifndef REGRESSIVE_MACHINE_H
#define REGRESSIVE_MACHINE_H
#include<TsetlinMachine.h>
class RegressiveMachine{
  private:
    float scale; 
    float *Weights; 
    TSetlin **TAMs;
    uint32_t resolution;
  public:
    RegressiveMachine(float T,uint32_t resolution);
    float learn(uint8_t *input , float expectedValue);
    float evaluate(uint8_t *input);
};
#endif 
