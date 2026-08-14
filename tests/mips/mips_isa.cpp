#include "mips_isa.H"
#include "mips_isa_init.cpp"
#include "mips_bhv_macros.H"

using namespace mips_parms;

void ac_behavior( begin ){
  RB[0] = 0;
}

void ac_behavior( end ){}

void ac_behavior( instruction ){
  ac_pc = ac_pc + 4;
}

void ac_behavior( Type_R ){}
void ac_behavior( Type_I ){}
void ac_behavior( Type_J ){}

void ac_behavior( add ){
  RB[rd] = RB[rs] + RB[rt];
  RB[0] = 0;
}

void ac_behavior( sub ){
  RB[rd] = RB[rs] - RB[rt];
  RB[0] = 0;
}

void ac_behavior( and_op ){
  RB[rd] = RB[rs] & RB[rt];
  RB[0] = 0;
}

void ac_behavior( or_op ){
  RB[rd] = RB[rs] | RB[rt];
  RB[0] = 0;
}

void ac_behavior( xor_op ){
  RB[rd] = RB[rs] ^ RB[rt];
  RB[0] = 0;
}

void ac_behavior( nor_op ){
  RB[rd] = ~(RB[rs] | RB[rt]);
  RB[0] = 0;
}

void ac_behavior( slt ){
  RB[rd] = ((int32_t)RB[rs] < (int32_t)RB[rt]) ? 1 : 0;
  RB[0] = 0;
}

void ac_behavior( sltu ){
  RB[rd] = ((uint32_t)RB[rs] < (uint32_t)RB[rt]) ? 1 : 0;
  RB[0] = 0;
}

void ac_behavior( sll ){
  RB[rd] = RB[rt] << (shamt & 0x1F);
  RB[0] = 0;
}

void ac_behavior( srl ){
  RB[rd] = ((uint32_t)RB[rt]) >> (shamt & 0x1F);
  RB[0] = 0;
}

void ac_behavior( sra ){
  RB[rd] = ((int32_t)RB[rt]) >> (shamt & 0x1F);
  RB[0] = 0;
}

void ac_behavior( mult ){
  int64_t res = (int64_t)(int32_t)RB[rs] * (int64_t)(int32_t)RB[rt];
  hi = (uint32_t)(res >> 32);
  lo = (uint32_t)(res & 0xFFFFFFFF);
}

void ac_behavior( div_op ){
  if (RB[rt] != 0) {
    lo = (int32_t)RB[rs] / (int32_t)RB[rt];
    hi = (int32_t)RB[rs] % (int32_t)RB[rt];
  }
}

void ac_behavior( mfhi ){
  RB[rd] = hi;
  RB[0] = 0;
}

void ac_behavior( mflo ){
  RB[rd] = lo;
  RB[0] = 0;
}

void ac_behavior( jr ){
  ac_pc = RB[rs];
}

void ac_behavior( nop ){}

void ac_behavior( addi ){
  RB[rt] = RB[rs] + (int32_t)imm;
  RB[0] = 0;
}

void ac_behavior( addiu ){
  RB[rt] = RB[rs] + (int32_t)imm;
  RB[0] = 0;
}

void ac_behavior( andi ){
  RB[rt] = RB[rs] & (uint32_t)(uint16_t)imm;
  RB[0] = 0;
}

void ac_behavior( ori ){
  RB[rt] = RB[rs] | (uint32_t)(uint16_t)imm;
  RB[0] = 0;
}

void ac_behavior( xori ){
  RB[rt] = RB[rs] ^ (uint32_t)(uint16_t)imm;
  RB[0] = 0;
}

void ac_behavior( lui ){
  RB[rt] = (uint32_t)(uint16_t)imm << 16;
  RB[0] = 0;
}

void ac_behavior( lw ){
  uint32_t addr = RB[rs] + (int32_t)imm;
  RB[rt] = DM.read(addr);
  RB[0] = 0;
}

void ac_behavior( sw ){
  uint32_t addr = RB[rs] + (int32_t)imm;
  DM.write(addr, RB[rt]);
}

void ac_behavior( beq ){
  if (RB[rs] == RB[rt]) {
    ac_pc = (ac_pc - 4) + ((int32_t)imm << 2);
  }
}

void ac_behavior( bne ){
  if (RB[rs] != RB[rt]) {
    ac_pc = (ac_pc - 4) + ((int32_t)imm << 2);
  }
}

void ac_behavior( blez ){
  if ((int32_t)RB[rs] <= 0) {
    ac_pc = (ac_pc - 4) + ((int32_t)imm << 2);
  }
}

void ac_behavior( bgtz ){
  if ((int32_t)RB[rs] > 0) {
    ac_pc = (ac_pc - 4) + ((int32_t)imm << 2);
  }
}

void ac_behavior( j ){
  ac_pc = (addr << 2);
}

void ac_behavior( jal ){
  RB[31] = ac_pc;
  ac_pc = (addr << 2);
}

void ac_behavior( halt ){
  stop();
}
