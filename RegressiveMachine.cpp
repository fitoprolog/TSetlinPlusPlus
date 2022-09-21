#include <RegressiveMachine.h>
#include<string.h>
#include <math.h>
#include<iostream>

RegressiveMachine::RegressiveMachine(float T,uint32_t resolution_){
  TAMs = new TSetlin*[resolution_];
  for(int i=0; i!= resolution_; i++){
    TAMs[i]=new TSetlin(9,1,resolution_);
  }
  srand(time(0));
  Weights = new float[resolution_];
  memset(Weights,0,sizeof(float)*resolution_);
  resolution = resolution_;
  scale=T;
}

float RegressiveMachine::learn(uint8_t* input, float y){
  float y_ = evaluate(input);
  std::cout << "Y_=" << y_ << std::endl;
  float error = y_-y;
  float p = fabsf(error)/(float)resolution;
  for(int j=0; j != resolution; j++){
    bool pj = ((float)(random()%100000)/100000.0) < p;  
    if ( error <0 && TAMs[j]->evaluateCondition(input) && pj){
      Weights[j]++;
    }else if (error >0 && pj && Weights[j] > 0 ){
      Weights[j]--;
    }
    if (error <0)
      TAMs[j]->typeIFeedback(input);
    else if (y>y_)
      TAMs[j]->typeIIFeedback(input);
  }
  return error;
}


float RegressiveMachine::evaluate(uint8_t* input){
  float ret=0;
  for(int t=0; t!=resolution;t++)
    ret+=Weights[t]*TAMs[t]->evaluateCondition(input);
  return ret/(float)resolution;
}
