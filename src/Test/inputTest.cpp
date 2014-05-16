#include <iostream>
#include <fstream>
using namespace std;

// Prints PS2 controller input as it comes in

int main(){
  
  char holderChar;
  short int controllerCommand;

  ifstream in_stream;
  in_stream.open("/dev/input/js0");

  // first 9x16 chars are junk
  for(int i = 0; i!=144; i++){
    in_stream.get(holderChar);
  }

  //Cout commands in groups of 8 chars
  while(true){
    int i = 0;
    while(in_stream.get(holderChar)){
      controllerCommand = 0 + holderChar;
      cout << "[" << controllerCommand << "]";
      
      if(i == 7){
	cout <<", i: " << i + 1 << endl;
	break;
      }
      i++;
    }
  }
  
  return 0;
}
