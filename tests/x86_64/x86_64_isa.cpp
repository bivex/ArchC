#include "x86_64_isa.H"
#include "x86_64_isa_init.cpp"
#include "x86_64_bhv_macros.H"

using namespace x86_64_parms;

void ac_behavior( begin ){
  rflags = 0;
  if (ac_pc == 0) ac_pc = 0x1000;
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
  RB[rm] = (uint64_t)RB[rm] + (uint64_t)RB[reg];
}

void ac_behavior( sub_rr ){
  uint64_t a = RB[rm];
  uint64_t b = RB[reg];
  uint64_t res = a - b;
  RB[rm] = res;
  uint64_t f = rflags & ~0x44ULL;
  if (res == 0) f |= (1ULL << 6); // ZF
  rflags = f;
}

void ac_behavior( and_rr ){
  RB[rm] = (uint64_t)RB[rm] & (uint64_t)RB[reg];
}

void ac_behavior( or_rr ){
  RB[rm] = (uint64_t)RB[rm] | (uint64_t)RB[reg];
}

void ac_behavior( xor_rr ){
  RB[rm] = (uint64_t)RB[rm] ^ (uint64_t)RB[reg];
}

void ac_behavior( mov_rr ){
  RB[rm] = RB[reg];
}

void ac_behavior( imul_rr ){
  RB[rm] = (int64_t)RB[rm] * (int64_t)RB[reg];
}

void ac_behavior( mov_ri ){
  RB[reg] = (int64_t)imm;
}

void ac_behavior( add_ri ){
  RB[reg] = (uint64_t)RB[reg] + (int64_t)imm;
}

void ac_behavior( sub_ri ){
  uint64_t res = (uint64_t)RB[reg] - (int64_t)imm;
  RB[reg] = res;
  uint64_t f = rflags & ~0x44ULL;
  if (res == 0) f |= (1ULL << 6); // ZF
  rflags = f;
}

void ac_behavior( cmp_ri ){
  uint64_t res = (uint64_t)RB[reg] - (int64_t)imm;
  uint64_t f = rflags & ~0x44ULL;
  if (res == 0) f |= (1ULL << 6); // ZF
  rflags = f;
}

void ac_behavior( mov_rm ){
  uint64_t addr = (uint64_t)RB[rm] + (int64_t)disp;
  RB[reg] = DM.read(addr);
}

void ac_behavior( mov_mr ){
  uint64_t addr = (uint64_t)RB[rm] + (int64_t)disp;
  DM.write(addr, RB[reg]);
}

void ac_behavior( jmp ){
  ac_pc = (ac_pc - 4) + (offset << 2);
}

void ac_behavior( jne ){
  bool ZF = (rflags >> 6) & 1;
  if (!ZF) {
    ac_pc = (ac_pc - 4) + (offset << 2);
  }
}

void ac_behavior( je ){
  bool ZF = (rflags >> 6) & 1;
  if (ZF) {
    ac_pc = (ac_pc - 4) + (offset << 2);
  }
}

void ac_behavior( hlt ){
  stop();
}
