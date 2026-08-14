#include "aarch64_isa.H"
#include "aarch64_isa_init.cpp"
#include "aarch64_bhv_macros.H"

using namespace aarch64_parms;

static inline bool aarch64_eval_cond(uint8_t cond, uint64_t pstate) {
  bool N = (pstate >> 31) & 1;
  bool Z = (pstate >> 30) & 1;
  bool C = (pstate >> 29) & 1;
  bool V = (pstate >> 28) & 1;
  switch (cond) {
    case 0x0: return Z;                      // EQ
    case 0x1: return !Z;                     // NE
    case 0x2: return C;                      // CS / HS
    case 0x3: return !C;                     // CC / LO
    case 0x4: return N;                      // MI
    case 0x5: return !N;                     // PL
    case 0x6: return V;                      // VS
    case 0x7: return !V;                     // VC
    case 0x8: return C && !Z;                // HI
    case 0x9: return !C || Z;                // LS
    case 0xA: return N == V;                 // GE
    case 0xB: return N != V;                 // LT
    case 0xC: return !Z && (N == V);         // GT
    case 0xD: return Z || (N != V);          // LE
    case 0xE: return true;                   // AL
    case 0xF: return true;                   // NV
    default: return true;
  }
}

static inline void aarch64_set_flags(uint64_t res, uint64_t op1, uint64_t op2, bool is_sub, ac_reg<uint64_t>& pstate) {
  bool N = (res >> 63) & 1;
  bool Z = (res == 0);
  bool C = is_sub ? (op1 >= op2) : (res < op1);
  bool V = is_sub ? (((op1 ^ op2) & (op1 ^ res)) >> 63) & 1 : (((~(op1 ^ op2)) & (op1 ^ res)) >> 63) & 1;
  pstate = ((uint64_t)N << 31) | ((uint64_t)Z << 30) | ((uint64_t)C << 29) | ((uint64_t)V << 28);
}

void ac_behavior( begin ){
  RB[31] = 0;
  pstate = 0;
}

void ac_behavior( end ){}

void ac_behavior( instruction ){
  ac_pc = ac_pc + 4;
}

void ac_behavior( Type_DP_Reg ){}
void ac_behavior( Type_DP_Imm ){}
void ac_behavior( Type_LS_Imm ){}
void ac_behavior( Type_Branch_Cond ){}
void ac_behavior( Type_Branch_Imm ){}
void ac_behavior( Type_Branch_Reg ){}
void ac_behavior( Type_Halt ){}

// Data Processing (Register)
void ac_behavior( add_reg ){
  RB[rd] = (uint64_t)RB[rn] + (uint64_t)RB[rm];
  RB[31] = 0;
}

void ac_behavior( adds_reg ){
  uint64_t res = (uint64_t)RB[rn] + (uint64_t)RB[rm];
  aarch64_set_flags(res, RB[rn], RB[rm], false, pstate);
  RB[rd] = res;
  RB[31] = 0;
}

void ac_behavior( sub_reg ){
  RB[rd] = (uint64_t)RB[rn] - (uint64_t)RB[rm];
  RB[31] = 0;
}

void ac_behavior( subs_reg ){
  uint64_t res = (uint64_t)RB[rn] - (uint64_t)RB[rm];
  aarch64_set_flags(res, RB[rn], RB[rm], true, pstate);
  RB[rd] = res;
  RB[31] = 0;
}

void ac_behavior( and_reg ){
  RB[rd] = (uint64_t)RB[rn] & (uint64_t)RB[rm];
  RB[31] = 0;
}

void ac_behavior( orr_reg ){
  RB[rd] = (uint64_t)RB[rn] | (uint64_t)RB[rm];
  RB[31] = 0;
}

void ac_behavior( eor_reg ){
  RB[rd] = (uint64_t)RB[rn] ^ (uint64_t)RB[rm];
  RB[31] = 0;
}

void ac_behavior( mul_reg ){
  RB[rd] = (uint64_t)RB[rn] * (uint64_t)RB[rm];
  RB[31] = 0;
}

// Data Processing (Immediate)
void ac_behavior( add_imm ){
  RB[rd] = (uint64_t)RB[rn] + (uint64_t)imm14;
  RB[31] = 0;
}

void ac_behavior( adds_imm ){
  uint64_t res = (uint64_t)RB[rn] + (uint64_t)imm14;
  aarch64_set_flags(res, RB[rn], imm14, false, pstate);
  RB[rd] = res;
  RB[31] = 0;
}

void ac_behavior( sub_imm ){
  RB[rd] = (uint64_t)RB[rn] - (uint64_t)imm14;
  RB[31] = 0;
}

void ac_behavior( subs_imm ){
  uint64_t res = (uint64_t)RB[rn] - (uint64_t)imm14;
  aarch64_set_flags(res, RB[rn], imm14, true, pstate);
  RB[rd] = res;
  RB[31] = 0;
}

void ac_behavior( movz_imm ){
  RB[rd] = (uint64_t)imm14;
  RB[31] = 0;
}

// Load / Store
void ac_behavior( ldr_x ){
  uint64_t addr = (uint64_t)RB[rn] + (int64_t)disp;
  RB[rd] = DM.read(addr);
  RB[31] = 0;
}

void ac_behavior( str_x ){
  uint64_t addr = (uint64_t)RB[rn] + (int64_t)disp;
  DM.write(addr, RB[rd]);
}

void ac_behavior( ldr_w ){
  uint64_t addr = (uint64_t)RB[rn] + (int64_t)disp;
  RB[rd] = (uint32_t)DM.read(addr);
  RB[31] = 0;
}

void ac_behavior( str_w ){
  uint64_t addr = (uint64_t)RB[rn] + (int64_t)disp;
  DM.write(addr, (uint32_t)RB[rd]);
}

// Branch
void ac_behavior( b_cond ){
  if (aarch64_eval_cond(cond, pstate)) {
    ac_pc = (ac_pc - 4) + (imm19 << 2);
  }
}

void ac_behavior( b_imm ){
  ac_pc = (ac_pc - 4) + (imm24 << 2);
}

void ac_behavior( bl_imm ){
  RB[30] = ac_pc;
  ac_pc = (ac_pc - 4) + (imm24 << 2);
}

void ac_behavior( br_reg ){
  ac_pc = RB[rn];
}

void ac_behavior( ret_reg ){
  ac_pc = RB[30];
}

void ac_behavior( halt ){
  stop();
}
