#include <iostream>

using Byte = unsigned char;
using Word = unsigned short;

using u32 = unsigned int;

struct Mem
{
  static constexpr u32 MAX_MEM = 1024 * 64;
  Byte Data[MAX_MEM];

  void Initialize()
  {
    for (u32 i = 0; i < MAX_MEM; i++){
      Data[i] = 0;
    }
  }

  Byte operator[](u32 address) const
  {
    if (address >= MAX_MEM)
      return 0;       // todo: throw access violation if addr is out of bounds
      
    return Data[address];
  }

  Byte& operator[](u32 address)
  {
    // throw access violation if address is out of bounds
    return Data[address];
  }

};


struct CPU 
{
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
  
  //opcodes
  static constexpr Byte INS_LDA_IMMEDIATE = 0xA9,
                        INS_LDA_ZP        = 0xA5,   // zero page
                        INS_LDA_ZPX       = 0xB5;   // zero page X

  void LDASetStatus()
  {
    Z = (A == 0);
    N = (A & 0b10000000) > 0;
  }

  void Reset(Mem& memory)
  {
    PC = 0xFFFC;
    SP = 0x0100;
    C = Z = I = D = B = V = N = 0;
    A = X = Y = 0;

    memory.Initialize();
  }

  // fetches current byte pointed to by program counter
  Byte FetchByte(u32& cycles, const Mem& memory)
  {
    if (cycles <= 0)
      return 0;

    const auto data = memory[PC];
    if(PC < Mem::MAX_MEM - 1)
      PC++;
      
    cycles--;
    
    return data;
  }

  // simply reads an address from memory
  Byte ReadByte(u32& cycles, const Mem& memory, const Byte address)
  {
    if (cycles <= 0)
      return 0;

    const auto data = memory[address];
    cycles--;
    
    return data;
  }

  void Execute(u32 cycles, const Mem& memory)
  {
    while(cycles)
    {
      const auto instruction = FetchByte(cycles, memory);
      switch(instruction)
      {
        case INS_LDA_IMMEDIATE:
          {
            const auto value = FetchByte(cycles, memory); 
            A = value;
            LDASetStatus();
          }
          break;
          
        case INS_LDA_ZP:
          {
            const auto zero_page_addr = FetchByte(cycles, memory);
            if (zero_page_addr < 0 || zero_page_addr >= Mem::MAX_MEM)
              break;

            A = ReadByte(cycles, memory, zero_page_addr);
            LDASetStatus();
          }
          break;

        case INS_LDA_ZPX:
          {
            auto zero_page_addr = FetchByte(cycles, memory);
            if (zero_page_addr + X >= Mem::MAX_MEM)
              break;

            zero_page_addr += X;
            cycles--;
            A = ReadByte(cycles, memory, zero_page_addr);
            LDASetStatus();
          }
          break;

        default:
          std::cout << "Invalid instruction: " << instruction << std::endl;
          break;
      }
    }
  }

};


int main(){
  Mem mem;
  CPU cpu;
 
  cpu.Reset(mem);

  cpu.X = 0x0001; 
  mem[0xFFFC] = CPU::INS_LDA_ZPX;
  mem[0xFFFD] = 0x42;
  mem[0x0043] = 0x84;

  cpu.Execute(4, mem);
  
  std::cout << "Value of A: " << (int)cpu.A << std::endl; 

  return 0;
}
