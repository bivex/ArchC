#include "apple_arm_isa.H"
#include "apple_arm_isa_init.cpp"
#include "apple_arm_bhv_macros.H"

using namespace apple_arm_parms;

static inline bool apple_eval_cond(uint8_t cond, uint64_t pstate) {
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

static inline void apple_set_flags(uint64_t res, uint64_t op1, uint64_t op2, bool is_sub, ac_reg<uint64_t>& pstate) {
  bool N = (res >> 63) & 1;
  bool Z = (res == 0);
  bool C = is_sub ? (op1 >= op2) : (res < op1);
  bool V = is_sub ? (((op1 ^ op2) & (op1 ^ res)) >> 63) & 1 : (((~(op1 ^ op2)) & (op1 ^ res)) >> 63) & 1;
  pstate = ((uint64_t)N << 31) | ((uint64_t)Z << 30) | ((uint64_t)C << 29) | ((uint64_t)V << 28);
}

static inline uint64_t compute_pac(uint64_t ptr, uint64_t modifier, uint64_t key) {
  // Apple QARMA64 / SIPHASH-like lightweight PAC simulation
  uint64_t bottom = ptr & 0x0000FFFFFFFFFFFFULL;
  uint64_t hash = (bottom ^ modifier ^ key);
  hash = (hash * 0x517cc1b727220a95ULL) ^ (hash >> 32);
  uint64_t pac_tag = (hash >> 48) & 0xFFFFULL;
  return bottom | (pac_tag << 48);
}

static inline uint64_t verify_aut(uint64_t signed_ptr, uint64_t modifier, uint64_t key) {
  uint64_t expected = compute_pac(signed_ptr, modifier, key);
  if ((signed_ptr >> 48) == (expected >> 48)) {
    return signed_ptr & 0x0000FFFFFFFFFFFFULL;
  } else {
    return signed_ptr | (0xDEADULL << 48);
  }
}

void ac_behavior(begin) {
  pac_key_ia = 0xA5A55A5A12345678ULL;
  pac_key_da = 0x5A5AA5A587654321ULL;
  amx_state = 1;
  tso_mode = 1;
  pstate = 0;
  X[31] = 0;
}

void ac_behavior(end) {}

void ac_behavior(instruction) {
  ac_pc = ac_pc + 4;
}

void ac_behavior(Type_DP_Reg) {}
void ac_behavior(Type_DP_Imm) {}
void ac_behavior(Type_LS_Imm) {}
void ac_behavior(Type_PAC) {}
void ac_behavior(Type_Branch_Cond) {}
void ac_behavior(Type_Branch_Imm) {}
void ac_behavior(Type_Halt) {}

void ac_behavior(add_x) {
  X[rd] = X[rn] + X[rm];
  X[31] = 0;
}

void ac_behavior(sub_x) {
  X[rd] = X[rn] - X[rm];
  X[31] = 0;
}

void ac_behavior(and_x) {
  X[rd] = X[rn] & X[rm];
  X[31] = 0;
}

void ac_behavior(orr_x) {
  X[rd] = X[rn] | X[rm];
  X[31] = 0;
}

void ac_behavior(eor_x) {
  X[rd] = X[rn] ^ X[rm];
  X[31] = 0;
}

void ac_behavior(mul_x) {
  X[rd] = X[rn] * X[rm];
  X[31] = 0;
}

void ac_behavior(amx_fma) {
  // Apple Matrix Coprocessor (AMX) Matrix-Vector Multiply-Accumulate
  uint64_t prod = X[rn] * X[rm];
  X[rd] = X[rd] + prod + 1ULL;
  X[31] = 0;
}

void ac_behavior(add_imm) {
  X[rd] = X[rn] + (uint64_t)imm14;
  X[31] = 0;
}

void ac_behavior(subs_imm) {
  uint64_t res = X[rn] - (uint64_t)imm14;
  apple_set_flags(res, X[rn], imm14, true, pstate);
  X[rd] = res;
  X[31] = 0;
}

void ac_behavior(movz_imm) {
  X[rd] = (uint64_t)imm14;
  X[31] = 0;
}

void ac_behavior(ldr_x) {
  uint64_t addr = X[rn] + (int64_t)disp;
  X[rd] = DM.read(addr);
  X[31] = 0;
}

void ac_behavior(str_x) {
  uint64_t addr = X[rn] + (int64_t)disp;
  DM.write(addr, X[rd]);
}

void ac_behavior(pacia) {
  X[rd] = compute_pac(X[rd], X[rn], pac_key_ia);
  X[31] = 0;
}

void ac_behavior(autia) {
  X[rd] = verify_aut(X[rd], X[rn], pac_key_ia);
  X[31] = 0;
}

void ac_behavior(pacda) {
  X[rd] = compute_pac(X[rd], X[rn], pac_key_da);
  X[31] = 0;
}

void ac_behavior(autda) {
  X[rd] = verify_aut(X[rd], X[rn], pac_key_da);
  X[31] = 0;
}

void ac_behavior(b_cond) {
  if (apple_eval_cond(cond, pstate)) {
    ac_pc = (ac_pc - 4) + (imm19 << 2);
  }
}

void ac_behavior(b_imm) {
  ac_pc = (ac_pc - 4) + (imm24 << 2);
}

void ac_behavior(bl_imm) {
  X[30] = ac_pc;
  ac_pc = (ac_pc - 4) + (imm24 << 2);
}

void ac_behavior(halt) {
  stop();
}
