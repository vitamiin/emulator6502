#include <iostream>


struct CPU 
{
  using Byte = unsigned char;
  using Word = unsigned short;

  Word PC;        // program counter / instruction pointer
  Word SP;        // stack pointer
  
  Byte A, X, Y;   // registers
  
  // status flags
  Byte C : 1;   // carry flag
  Byte Z : 1;   // zero flag
  Byte I : 1;   // interrupt disable
  Byte D : 1;   // decimal mode
  Byte B : 1;   // break command
  Byte V : 1;   // overflow flag
  Byte N : 1;   // negative flag
};


int main(){
  return 0;
}
