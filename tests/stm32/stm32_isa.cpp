#include "stm32_isa.H"
#include "stm32_isa_init.cpp"
#include "stm32_bhv_macros.H"

using namespace stm32_parms;

static inline void stm32_set_zn(uint32_t val, ac_reg<uint32_t>& xpsr) {
  bool N = (val >> 31) & 1;
  bool Z = (val == 0);
  xpsr = (xpsr & ~0xC0000000) | (N ? 0x80000000 : 0) | (Z ? 0x40000000 : 0);
}

static inline uint32_t stm32_add_flags(uint32_t a, uint32_t b, ac_reg<uint32_t>& xpsr) {
  uint64_t res64 = (uint64_t)a + (uint64_t)b;
  uint32_t res = (uint32_t)res64;
  bool N = (res >> 31) & 1;
  bool Z = (res == 0);
  bool C = res64 > 0xFFFFFFFFULL;
  bool V = (~(a ^ b) & (a ^ res) & 0x80000000) != 0;
  xpsr = (xpsr & ~0xF0000000) | (N ? 0x80000000 : 0) | (Z ? 0x40000000 : 0) | (C ? 0x20000000 : 0) | (V ? 0x10000000 : 0);
  return res;
}

static inline uint32_t stm32_sub_flags(uint32_t a, uint32_t b, ac_reg<uint32_t>& xpsr) {
  uint32_t res = a - b;
  bool N = (res >> 31) & 1;
  bool Z = (res == 0);
  bool C = a >= b;
  bool V = ((a ^ b) & (a ^ res) & 0x80000000) != 0;
  xpsr = (xpsr & ~0xF0000000) | (N ? 0x80000000 : 0) | (Z ? 0x40000000 : 0) | (C ? 0x20000000 : 0) | (V ? 0x10000000 : 0);
  return res;
}

void ac_behavior( begin ){
  ac_pc = ac_pc & ~1;
  R[13] = 0x20005000; // Top of 20KB SRAM
  xpsr = 0x01000000;  // Thumb bit set
  primask = 0;
  control = 0;
}

void ac_behavior( end ){}

void ac_behavior( instruction ){
  ac_pc = ac_pc + 2;
}

void ac_behavior( Type_Imm8 ){}
void ac_behavior( Type_Alu3 ){}
void ac_behavior( Type_Alu2 ){}
void ac_behavior( Type_Mem ){}
void ac_behavior( Type_BCond ){}
void ac_behavior( Type_BUnc ){}
void ac_behavior( Type_Halt ){}

// Imm8
void ac_behavior( movs_imm ){
  R[rd1] = imm1;
  stm32_set_zn(R[rd1], xpsr);
}

void ac_behavior( adds_imm ){
  R[rd1] = stm32_add_flags(R[rd1], imm1, xpsr);
}

void ac_behavior( subs_imm ){
  R[rd1] = stm32_sub_flags(R[rd1], imm1, xpsr);
}

void ac_behavior( cmp_imm ){
  stm32_sub_flags(R[rd1], imm1, xpsr);
}

// Alu3
void ac_behavior( adds_reg ){
  R[rd2] = stm32_add_flags(R[rn2], R[rm2], xpsr);
}

void ac_behavior( subs_reg ){
  R[rd2] = stm32_sub_flags(R[rn2], R[rm2], xpsr);
}

// Alu2
void ac_behavior( ands_reg ){
  R[rdn3] = R[rdn3] & R[rm3];
  stm32_set_zn(R[rdn3], xpsr);
}

void ac_behavior( eors_reg ){
  R[rdn3] = R[rdn3] ^ R[rm3];
  stm32_set_zn(R[rdn3], xpsr);
}

void ac_behavior( orrs_reg ){
  R[rdn3] = R[rdn3] | R[rm3];
  stm32_set_zn(R[rdn3], xpsr);
}

void ac_behavior( muls_reg ){
  R[rdn3] = (int32_t)R[rdn3] * (int32_t)R[rm3];
  stm32_set_zn(R[rdn3], xpsr);
}

// Mem
void ac_behavior( ldr_imm ){
  uint32_t addr = R[rn4] + (imm4 << 2);
  R[rt4] = DM.read(addr);
}

void ac_behavior( str_imm ){
  uint32_t addr = R[rn4] + (imm4 << 2);
  DM.write(addr, R[rt4]);
}

// Branch Cond
void ac_behavior( beq ){
  bool Z = (xpsr >> 30) & 1;
  if (Z) {
    ac_pc = ac_pc + 2 + (bdisp5 * 2);
  }
}

void ac_behavior( bne ){
  bool Z = (xpsr >> 30) & 1;
  if (!Z) {
    ac_pc = ac_pc + 2 + (bdisp5 * 2);
  }
}

void ac_behavior( bcs ){
  bool C = (xpsr >> 29) & 1;
  if (C) {
    ac_pc = ac_pc + 2 + (bdisp5 * 2);
  }
}

void ac_behavior( bcc ){
  bool C = (xpsr >> 29) & 1;
  if (!C) {
    ac_pc = ac_pc + 2 + (bdisp5 * 2);
  }
}

void ac_behavior( bmi ){
  bool N = (xpsr >> 31) & 1;
  if (N) {
    ac_pc = ac_pc + 2 + (bdisp5 * 2);
  }
}

void ac_behavior( bpl ){
  bool N = (xpsr >> 31) & 1;
  if (!N) {
    ac_pc = ac_pc + 2 + (bdisp5 * 2);
  }
}

void ac_behavior( bge ){
  bool N = (xpsr >> 31) & 1;
  bool V = (xpsr >> 28) & 1;
  if (N == V) {
    ac_pc = ac_pc + 2 + (bdisp5 * 2);
  }
}

void ac_behavior( blt ){
  bool N = (xpsr >> 31) & 1;
  bool V = (xpsr >> 28) & 1;
  if (N != V) {
    ac_pc = ac_pc + 2 + (bdisp5 * 2);
  }
}

void ac_behavior( b_unc ){
  ac_pc = ac_pc + 2 + (bdisp6 * 2);
}

void ac_behavior( bkpt_halt ){
  stop();
}
