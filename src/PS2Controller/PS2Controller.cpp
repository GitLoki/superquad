#include <iostream>

char getArdCommand(int controllerCommand){
  switch(controllerCommand){
  case 0: return 'd';
  case 1: return 'x';
  case 2: return 'w';
  case 3: return 'a';
  case 4: return '-';
  case 5: return 'e';
  case 6: return '+';
  case 7: return 'q';
  default: return '?';
  }
}
