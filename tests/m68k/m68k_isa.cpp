#include "m68k_isa.H"
#include "m68k_isa_init.cpp"
#include "m68k_bhv_macros.H"

using namespace m68k_parms;

static inline bool m68k_eval_cond(uint8_t cond, uint32_t sr) {
  bool N = (sr >> 3) & 1;
  bool Z = (sr >> 2) & 1;
  bool V = (sr >> 1) & 1;
  bool C = sr & 1;
  switch (cond) {
    case 0x0: return true;            // BRA / T
    case 0x1: return false;           // F
    case 0x6: return !Z;              // BNE
    case 0x7: return Z;               // BEQ
    case 0xC: return (N == V);        // BGE
    case 0xD: return (N != V);        // BLT
    case 0xE: return (!Z && (N == V));// BGT
    case 0xF: return (Z || (N != V)); // BLE
    default: return true;
  }
}

static inline void m68k_set_flags_add(uint32_t res, uint32_t op1, uint32_t op2, ac_reg<uint32_t>& sr) {
  bool C = ((uint64_t)op1 + (uint64_t)op2) > 0xFFFFFFFFULL;
  bool Z = (res == 0);
  bool N = (res >> 31) & 1;
  bool V = (((~(op1 ^ op2)) & (op1 ^ res)) >> 31) & 1;
  sr = (sr & ~0x1F) | (C << 4) | (N << 3) | (Z << 2) | (V << 1) | C;
}

static inline void m68k_set_flags_sub(uint32_t res, uint32_t op1, uint32_t op2, ac_reg<uint32_t>& sr) {
  bool C = op1 < op2;
  bool Z = (res == 0);
  bool N = (res >> 31) & 1;
  bool V = (((op1 ^ op2) & (op1 ^ res)) >> 31) & 1;
  sr = (sr & ~0x1F) | (C << 4) | (N << 3) | (Z << 2) | (V << 1) | C;
}

void ac_behavior( begin ){
  sr = 0;
}

void ac_behavior( end ){}

void ac_behavior( instruction ){
  ac_pc = ac_pc + 2;
}

void ac_behavior( Type_MoveQ ){}
void ac_behavior( Type_ALU_Reg ){}
void ac_behavior( Type_Quick ){}
void ac_behavior( Type_Branch ){}
void ac_behavior( Type_Move_Mem ){}
void ac_behavior( Type_Halt ){}

// Instructions
void ac_behavior( moveq ){
  RD[rd] = (int32_t)data;
  sr = (sr & ~0x1F) | (((data < 0) ? 1 : 0) << 3) | (((data == 0) ? 1 : 0) << 2);
}

void ac_behavior( add_l ){
  uint32_t op1 = RD[rd];
  uint32_t op2 = RD[reg];
  uint32_t res = op1 + op2;
  m68k_set_flags_add(res, op1, op2, sr);
  RD[rd] = res;
}

void ac_behavior( sub_l ){
  uint32_t op1 = RD[rd];
  uint32_t op2 = RD[reg];
  uint32_t res = op1 - op2;
  m68k_set_flags_sub(res, op1, op2, sr);
  RD[rd] = res;
}

void ac_behavior( mul_l ){
  int32_t op1 = (int32_t)(int16_t)RD[rd];
  int32_t op2 = (int32_t)(int16_t)RD[reg];
  int32_t res = op1 * op2;
  RD[rd] = (uint32_t)res;
  sr = (sr & ~0x1F) | (((res < 0) ? 1 : 0) << 3) | (((res == 0) ? 1 : 0) << 2);
}

void ac_behavior( and_l ){
  uint32_t res = RD[rd] & RD[reg];
  RD[rd] = res;
  sr = (sr & ~0x1F) | (((res >> 31) & 1) << 3) | ((res == 0) << 2);
}

void ac_behavior( or_l ){
  uint32_t res = RD[rd] | RD[reg];
  RD[rd] = res;
  sr = (sr & ~0x1F) | (((res >> 31) & 1) << 3) | ((res == 0) << 2);
}

void ac_behavior( eor_l ){
  uint32_t res = RD[rd] ^ RD[reg];
  RD[rd] = res;
  sr = (sr & ~0x1F) | (((res >> 31) & 1) << 3) | ((res == 0) << 2);
}

void ac_behavior( addq_l ){
  uint32_t q = (qdata == 0) ? 8 : qdata;
  uint32_t op1 = RD[reg];
  uint32_t res = op1 + q;
  m68k_set_flags_add(res, op1, q, sr);
  RD[reg] = res;
}

void ac_behavior( subq_l ){
  uint32_t q = (qdata == 0) ? 8 : qdata;
  uint32_t op1 = RD[reg];
  uint32_t res = op1 - q;
  m68k_set_flags_sub(res, op1, q, sr);
  RD[reg] = res;
}

void ac_behavior( bra ){
  ac_pc = (ac_pc - 2) + 2 + bdisp;
}

void ac_behavior( bne ){
  if (m68k_eval_cond(0x6, sr)) {
    ac_pc = (ac_pc - 2) + 2 + bdisp;
  }
}

void ac_behavior( beq ){
  if (m68k_eval_cond(0x7, sr)) {
    ac_pc = (ac_pc - 2) + 2 + bdisp;
  }
}

void ac_behavior( blt ){
  if (m68k_eval_cond(0xD, sr)) {
    ac_pc = (ac_pc - 2) + 2 + bdisp;
  }
}

void ac_behavior( bge ){
  if (m68k_eval_cond(0xC, sr)) {
    ac_pc = (ac_pc - 2) + 2 + bdisp;
  }
}

void ac_behavior( move_to_mem ){
  uint32_t addr = RA[dst_reg];
  DM.write(addr, RD[src_reg]);
}

void ac_behavior( move_from_mem ){
  uint32_t addr = RA[src_reg];
  RD[dst_reg] = DM.read(addr);
}

void ac_behavior( halt ){
  stop();
}
