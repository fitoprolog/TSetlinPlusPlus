#include <opencv2/opencv.hpp> 
#include <iostream>
#include<TsetlinMachine.h>
#include<RegressiveMachine.h>
#include <vector>
#include<math.h>

void TestSingleTesetlin(){
  uint8_t FourWheels =1;
  uint8_t TransportsPeople=1<< 2;
  uint8_t Wings=1<< 4;
  uint8_t Yellow=1<< 6;
  uint8_t c1[] = {FourWheels | TransportsPeople,1};
  uint8_t c2[] = {FourWheels | TransportsPeople|Yellow,0};
  uint8_t c3[] = {FourWheels | TransportsPeople|Yellow,0};

  std::vector<uint8_t *> cars{c1,c2,c3};
  uint8_t p1[] = {FourWheels | TransportsPeople | Wings  ,0};
  uint8_t p2[] = {FourWheels |  Wings | Yellow ,1};
  uint8_t p3[] = {TransportsPeople|Wings ,1};

  std::vector<uint8_t *> planes{p1,p2,p3};
  std::vector<std::vector<uint8_t *>> others {planes};
  std::vector<std::vector<uint8_t *>> carso {cars};
  TSetlin TAM(9,1,5);
  srand(time(0));
  TAM.train(cars,others,100);
  TAM.dumpRules();
  for(auto observation : cars)
    std::cout << "isCar " << TAM.evaluateCondition(observation) << std::endl;

  for(auto observation : planes)
    std::cout << "isCar " << TAM.evaluateCondition(observation) << std::endl;

  TAM.dumpRules();
}

void TestRegressiveMachine()
{
  float Sin60Hz[100];
  std::cout << "[";
  for(int i=0; i!=100; i++){
    if (i) 
      std::cout <<", ";
    Sin60Hz[i]=sin(2.0*M_PI*60.0f*(float)i*0.0002f);
    std::cout << Sin60Hz[i];
  }
  std::cout << "]\n";
  
  RegressiveMachine RT(1,16);
  for (int i=0; i!= 1000000;i++){
    uint32_t input = i%100;
    float err=RT.learn((uint8_t*)&input,Sin60Hz[i%100]);
    std::cout << "D("<<input<<")"<<"="<< Sin60Hz[i%100] << std::endl;
    std::cout << "Error " << err << std::endl;
  }
  std::cout << "[";
  for(int i=0; i!=100; i++){
    uint32_t input = i%100;
    if (i) 
      std::cout <<", ";
    std::cout << RT.evaluate((uint8_t*)&input) ;
  }
  std::cout << "]\n";

}

int main(int argc, char **argv){

  TestRegressiveMachine();
}
