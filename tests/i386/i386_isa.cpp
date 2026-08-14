#include "i386_isa.H"
#include "i386_isa_init.cpp"
#include "i386_bhv_macros.H"

using namespace i386_parms;

void ac_behavior( begin ){
  eflags = 0;
}

void ac_behavior( end ){}

void ac_behavior( instruction ){
  ac_pc = ac_pc + 4;
}

void ac_behavior( Type_RR ){}
void ac_behavior( Type_RI ){}
void ac_behavior( Type_RM ){}
void ac_behavior( Type_J ){}
void ac_behavior( Type_HLT ){}

void ac_behavior( add_rr ){
  RB[rm] = RB[rm] + RB[reg];
}

void ac_behavior( sub_rr ){
  uint32_t a = RB[rm];
  uint32_t b = RB[reg];
  uint32_t res = a - b;
  RB[rm] = res;
  uint32_t f = eflags & ~0x44U;
  if (res == 0) f |= (1 << 6); // ZF
  eflags = f;
}

void ac_behavior( and_rr ){
  RB[rm] = RB[rm] & RB[reg];
}

void ac_behavior( or_rr ){
  RB[rm] = RB[rm] | RB[reg];
}

void ac_behavior( xor_rr ){
  RB[rm] = RB[rm] ^ RB[reg];
}

void ac_behavior( mov_rr ){
  RB[rm] = RB[reg];
}

void ac_behavior( mov_ri ){
  RB[reg] = imm;
}

void ac_behavior( add_ri ){
  RB[reg] = RB[reg] + imm;
}

void ac_behavior( sub_ri ){
  uint32_t res = RB[reg] - imm;
  RB[reg] = res;
  uint32_t f = eflags & ~0x44U;
  if (res == 0) f |= (1 << 6); // ZF
  eflags = f;
}

void ac_behavior( cmp_ri ){
  uint32_t res = RB[reg] - imm;
  uint32_t f = eflags & ~0x44U;
  if (res == 0) f |= (1 << 6); // ZF
  eflags = f;
}

void ac_behavior( mov_rm ){
  uint32_t addr = RB[rm] + disp;
  RB[reg] = DM.read(addr);
}

void ac_behavior( mov_mr ){
  uint32_t addr = RB[rm] + disp;
  DM.write(addr, RB[reg]);
}

void ac_behavior( jmp ){
  ac_pc = (ac_pc - 4) + (offset << 2);
}

void ac_behavior( jne ){
  bool ZF = (eflags >> 6) & 1;
  if (!ZF) {
    ac_pc = (ac_pc - 4) + (offset << 2);
  }
}

void ac_behavior( je ){
  bool ZF = (eflags >> 6) & 1;
  if (ZF) {
    ac_pc = (ac_pc - 4) + (offset << 2);
  }
}

void ac_behavior( hlt ){
  stop();
}
