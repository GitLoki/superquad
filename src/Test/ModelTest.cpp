#include <iostream>
#include "../../include/PhysicsModel/PhysicsModel.hpp"
#include <fstream>
#include <unistd.h>

using namespace std;

void test_a(double before[3], double after[3]){
  if(after[1] < before[1])  cout << "Roll right successful ('a'), y coordinate increased" << endl; 
  else cout << "Roll right failed ('a'), y coordinate not increased" << endl;
}

void test_d(double before[3], double after[3]){
  if(after[1] > before[1])  cout << "Roll left successful ('d'), y coordinate decreased" << endl; 
  else cout << "Roll left failed ('d'), y coordinate not decreased" << endl;
}

void test_w(double before[3], double after[3]){
  if(after[0] < before[0])  cout << "Pitch forward successful ('w'), x coordinate decreased" << endl; 
  else cout << "Pitch forward failed ('w'), y coordinate not decreased" << endl;
}

void test_x(double before[3], double after[3]){
  if(after[0] > before[0])  cout << "Pitch backward successful ('x'), y coordinate increased" << endl; 
  else cout << "Pitch backward failed ('x'), y coordinate not increased" << endl;
}

void test_q(double before[3], double after[3]){
  if(after[0] < before[0] && after[1] < before[1])  cout << "Rotate right successful ('q'), x and y coordinates reduced and direction changed" << endl; 
  else cout << "Rotate right failed ('q'), x and y coordinates not reduced, direction not correctly changed" << endl;
}

void test_e(double before[3], double after[3]){
  if(after[0] > before[0] && after[1] > before[1])  cout << "Rotate left successful ('e'), x and y coordinates increased and direction changed" << endl; 
   else cout << "Rotate left failed ('e'), x and y coordinates not reduced, direction not correctly changed" << endl;
}

void test_up(double before[3], double after[3]){
  if(after[2] > before[2])  cout << "Increasing altitude successful ('+'), z coordinate increased" << endl; 
  else cout << "Increasing altitude failed ('+'), z coordinate not increased" << endl;
}

void test_down(double before[3], double after[3]){
  if(after[2] < before[2])  cout << "Decreasing altitude successful ('-'), z coordinate decreased" << endl; 
  else cout << "Decreasing altitude failed ('-'), z coordinate not decreased" << endl;
}



int main () {

  PhysicsModel testMod = PhysicsModel();
  testMod.init();
  char testMoves[] = {'+','+','+','+','+','+','+','+','+','+','+',
		      '+','+','+','+','+','+','+','+','+','+','+',
		      'd','d','d','a','a','a','w','w','w','x','x',
		      'q','q','q','e','e','e',
		      '-','-','-','-','-','-','-','-','-','-','-',
		      '-','-','-','-','-','-','-','-','-','-','-'};
  // Used to see result of move
  double positionBefore[sizeof(testMoves)/sizeof(testMoves[0])][3];
  double positionAfter[sizeof(testMoves)/sizeof(testMoves[0])][3];  
  usleep(2000000);
  
  if(testMod.err != simx_error_noerror){ 
    cout << "Test failed: " << testMod.err << endl;
    return 0;
  }
  
  for(int i = 0; i < (sizeof(testMoves)/sizeof(testMoves[0])); i++){
    testMod.getPosition(positionBefore[i]); 
    testMod.sendCommand(testMoves[i]);
    usleep(500000);            // Slowed down for clarity
    testMod.getPosition(positionAfter[i]);
  }
  
 for(int i = 0; i < (sizeof(testMoves)/sizeof(testMoves[0])); i++){
   switch(testMoves[i]){
   case 'a' : test_a(positionBefore[i], positionAfter[i]);
     break;
   case 'd' : test_d(positionBefore[i], positionAfter[i]);
     break;
   case 'w' : test_w(positionBefore[i], positionAfter[i]);
     break;
   case 'x' : test_x(positionBefore[i], positionAfter[i]);
     break;
   case 'q' : test_q(positionBefore[i], positionAfter[i]);
     break;
   case 'e' : test_e(positionBefore[i], positionAfter[i]);
     break;   
   case '+' : test_up(positionBefore[i], positionAfter[i]);
     break;
   case '-' : test_down(positionBefore[i], positionAfter[i]);
     break;
   }
 }
 return 0;    
}
