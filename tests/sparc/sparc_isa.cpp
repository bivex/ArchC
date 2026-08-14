#include "sparc_isa.H"
#include "sparc_isa_init.cpp"
#include "sparc_bhv_macros.H"

using namespace sparc_parms;

void ac_behavior( begin ){
  RB[0] = 0;
  psr = 0;
}

void ac_behavior( end ){}

void ac_behavior( instruction ){
  ac_pc = ac_pc + 4;
}

void ac_behavior( Type_F2 ){}
void ac_behavior( Type_F3R ){}
void ac_behavior( Type_F3I ){}
void ac_behavior( Type_BR ){}
void ac_behavior( Type_HLT ){}

void ac_behavior( add_r ){
  RB[rd] = RB[rs1] + RB[rs2];
  RB[0] = 0;
}

void ac_behavior( add_i ){
  RB[rd] = RB[rs1] + simm13;
  RB[0] = 0;
}

void ac_behavior( sub_r ){
  uint32_t a = RB[rs1];
  uint32_t b = RB[rs2];
  uint32_t res = a - b;
  RB[rd] = res;
  RB[0] = 0;
  // Update icc Z flag (bit 22 of psr)
  if (res == 0) psr = psr | (1U << 22);
  else psr = psr & ~(1U << 22);
}

void ac_behavior( sub_i ){
  uint32_t a = RB[rs1];
  uint32_t b = simm13;
  uint32_t res = a - b;
  RB[rd] = res;
  RB[0] = 0;
  if (res == 0) psr = psr | (1U << 22);
  else psr = psr & ~(1U << 22);
}

void ac_behavior( and_r ){
  RB[rd] = RB[rs1] & RB[rs2];
  RB[0] = 0;
}

void ac_behavior( and_i ){
  RB[rd] = RB[rs1] & simm13;
  RB[0] = 0;
}

void ac_behavior( or_r ){
  RB[rd] = RB[rs1] | RB[rs2];
  RB[0] = 0;
}

void ac_behavior( or_i ){
  RB[rd] = RB[rs1] | simm13;
  RB[0] = 0;
}

void ac_behavior( xor_r ){
  RB[rd] = RB[rs1] ^ RB[rs2];
  RB[0] = 0;
}

void ac_behavior( xor_i ){
  RB[rd] = RB[rs1] ^ simm13;
  RB[0] = 0;
}

void ac_behavior( sll_r ){
  RB[rd] = RB[rs1] << (RB[rs2] & 0x1F);
  RB[0] = 0;
}

void ac_behavior( sll_i ){
  RB[rd] = RB[rs1] << (simm13 & 0x1F);
  RB[0] = 0;
}

void ac_behavior( srl_r ){
  RB[rd] = ((uint32_t)RB[rs1]) >> (RB[rs2] & 0x1F);
  RB[0] = 0;
}

void ac_behavior( srl_i ){
  RB[rd] = ((uint32_t)RB[rs1]) >> (simm13 & 0x1F);
  RB[0] = 0;
}

void ac_behavior( sra_r ){
  RB[rd] = ((int32_t)RB[rs1]) >> (RB[rs2] & 0x1F);
  RB[0] = 0;
}

void ac_behavior( sra_i ){
  RB[rd] = ((int32_t)RB[rs1]) >> (simm13 & 0x1F);
  RB[0] = 0;
}

void ac_behavior( ld_i ){
  uint32_t addr = RB[rs1] + simm13;
  RB[rd] = DM.read(addr);
  RB[0] = 0;
}

void ac_behavior( st_i ){
  uint32_t addr = RB[rs1] + simm13;
  DM.write(addr, RB[rd]);
}

void ac_behavior( sethi ){
  RB[rd] = (imm22 & 0x3FFFFF) << 10;
  RB[0] = 0;
}

void ac_behavior( be ){
  bool Z = (psr >> 22) & 1;
  if (Z) {
    ac_pc = (ac_pc - 4) + (disp22 << 2);
  }
}

void ac_behavior( bne ){
  bool Z = (psr >> 22) & 1;
  if (!Z) {
    ac_pc = (ac_pc - 4) + (disp22 << 2);
  }
}

void ac_behavior( ba ){
  ac_pc = (ac_pc - 4) + (disp22 << 2);
}

void ac_behavior( halt ){
  stop();
}
