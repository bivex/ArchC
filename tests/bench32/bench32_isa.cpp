#include "bench32_isa.H"
#include "bench32_isa_init.cpp"
#include "bench32_bhv_macros.H"

using namespace bench32_parms;

void ac_behavior( begin ) {
  RB[0] = 0;
}

void ac_behavior( end ) {}

void ac_behavior( instruction ) {
  ac_pc = ac_pc + 4;
}

void ac_behavior( Type_R ) {}
void ac_behavior( Type_I ) {}
void ac_behavior( Type_J ) {}

void ac_behavior( add ) {
  RB[rd] = RB[rs] + RB[rt];
  RB[0] = 0;
}

void ac_behavior( sub ) {
  RB[rd] = RB[rs] - RB[rt];
  RB[0] = 0;
}

void ac_behavior( and_op ) {
  RB[rd] = RB[rs] & RB[rt];
  RB[0] = 0;
}

void ac_behavior( or_op ) {
  RB[rd] = RB[rs] | RB[rt];
  RB[0] = 0;
}

void ac_behavior( xor_op ) {
  RB[rd] = RB[rs] ^ RB[rt];
  RB[0] = 0;
}

void ac_behavior( slt ) {
  RB[rd] = ((int32_t)RB[rs] < (int32_t)RB[rt]) ? 1 : 0;
  RB[0] = 0;
}

void ac_behavior( sll ) {
  RB[rd] = RB[rt] << shamt;
  RB[0] = 0;
}

void ac_behavior( srl ) {
  RB[rd] = ((uint32_t)RB[rt]) >> shamt;
  RB[0] = 0;
}

void ac_behavior( nop ) {}

void ac_behavior( addi ) {
  int16_t simm = (int16_t)imm;
  RB[rt] = RB[rs] + simm;
  RB[0] = 0;
}

void ac_behavior( andi ) {
  RB[rt] = RB[rs] & imm;
  RB[0] = 0;
}

void ac_behavior( ori ) {
  RB[rt] = RB[rs] | imm;
  RB[0] = 0;
}

void ac_behavior( lw ) {
  int16_t simm = (int16_t)imm;
  uint32_t addr = RB[rs] + simm;
  RB[rt] = DM.read(addr);
  RB[0] = 0;
}

void ac_behavior( sw ) {
  int16_t simm = (int16_t)imm;
  uint32_t addr = RB[rs] + simm;
  DM.write(addr, RB[rt]);
}

void ac_behavior( beq ) {
  if (RB[rs] == RB[rt]) {
    int16_t simm = (int16_t)imm;
    ac_pc = (ac_pc - 4) + (simm << 2);
  }
}

void ac_behavior( bne ) {
  if (RB[rs] != RB[rt]) {
    int16_t simm = (int16_t)imm;
    ac_pc = (ac_pc - 4) + (simm << 2);
  }
}

void ac_behavior( j ) {
  ac_pc = (addr << 2);
}

void ac_behavior( halt ) {
  stop();
}
