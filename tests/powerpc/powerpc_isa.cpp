#include "powerpc_isa.H"
#include "powerpc_isa_init.cpp"
#include "powerpc_bhv_macros.H"

using namespace powerpc_parms;

void ac_behavior( begin ){
  cr = 0;
}

void ac_behavior( end ){}

void ac_behavior( instruction ){
  ac_pc = ac_pc + 4;
}

void ac_behavior( Type_XO ){}
void ac_behavior( Type_D ){}
void ac_behavior( Type_B ){}
void ac_behavior( Type_I ){}
void ac_behavior( Type_HLT ){}

void ac_behavior( add ){
  RB[rt] = RB[ra] + RB[rb];
}

void ac_behavior( subf ){
  RB[rt] = RB[rb] - RB[ra];
}

void ac_behavior( and_op ){
  RB[ra] = RB[rt] & RB[rb];
}

void ac_behavior( or_op ){
  RB[ra] = RB[rt] | RB[rb];
}

void ac_behavior( xor_op ){
  RB[ra] = RB[rt] ^ RB[rb];
}

void ac_behavior( slw ){
  RB[ra] = RB[rt] << (RB[rb] & 0x1F);
}

void ac_behavior( srw ){
  RB[ra] = ((uint32_t)RB[rt]) >> (RB[rb] & 0x1F);
}

void ac_behavior( sraw ){
  RB[ra] = ((int32_t)RB[rt]) >> (RB[rb] & 0x1F);
}

void ac_behavior( addi ){
  uint32_t a = (ra == 0) ? 0 : RB[ra];
  RB[rt] = a + d;
}

void ac_behavior( addic_dot ){
  uint32_t a = (ra == 0) ? 0 : RB[ra];
  uint32_t res = a + d;
  RB[rt] = res;
  uint32_t c = cr & 0x0FFFFFFF;
  if ((int32_t)res < 0) c |= (1U << 31);
  else if (res > 0) c |= (1U << 30);
  else c |= (1U << 29);
  cr = c;
}

void ac_behavior( addis ){
  uint32_t a = (ra == 0) ? 0 : RB[ra];
  RB[rt] = a + (d << 16);
}

void ac_behavior( ori ){
  RB[ra] = RB[rt] | (uint16_t)d;
}

void ac_behavior( andi_dot ){
  uint32_t res = RB[rt] & (uint16_t)d;
  RB[ra] = res;
  // Update CR0
  uint32_t c = cr & 0x0FFFFFFF;
  if ((int32_t)res < 0) c |= (1U << 31);
  else if (res > 0) c |= (1U << 30);
  else c |= (1U << 29); // Equal to 0
  cr = c;
}

void ac_behavior( lwz ){
  uint32_t a = (ra == 0) ? 0 : RB[ra];
  RB[rt] = DM.read(a + d);
}

void ac_behavior( stw ){
  uint32_t a = (ra == 0) ? 0 : RB[ra];
  DM.write(a + d, RB[rt]);
}

void ac_behavior( bc ){
  // Simplified branch on condition: if bo == 16 (branch if false) or 12 (branch if true) or 4
  bool Z = (cr >> 29) & 1; // EQ flag in CR0
  bool take = false;
  if (bo == 4 && !Z) take = true; // bne
  else if (bo == 12 && Z) take = true; // beq
  else if (bo == 20) take = true; // always
  else if (bo == 16 && !Z) take = true; // bne

  if (take) {
    ac_pc = (ac_pc - 4) + (bd << 2);
  }
}

void ac_behavior( b ){
  ac_pc = (ac_pc - 4) + (li << 2);
}

void ac_behavior( halt ){
  stop();
}
