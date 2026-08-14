#include "c6x_isa.H"
#include "c6x_isa_init.cpp"
#include "c6x_bhv_macros.H"

using namespace c6x_parms;

static inline bool c6x_eval_cond(uint8_t creg, uint8_t z, ac_regbank<16, uint32_t, unsigned long long>& RBA, ac_regbank<16, uint32_t, unsigned long long>& RBB) {
  if (creg == 0) return true;
  uint32_t val = 0;
  switch (creg) {
    case 1: val = RBB[0]; break;
    case 2: val = RBB[1]; break;
    case 3: val = RBB[2]; break;
    case 4: val = RBA[1]; break;
    case 5: val = RBA[2]; break;
    case 6: val = RBA[0]; break;
    default: val = 1; break;
  }
  return (z == 1) ? (val == 0) : (val != 0);
}

void ac_behavior( begin ){
  csr = 0;
}

void ac_behavior( end ){}

void ac_behavior( instruction ){
  ac_pc = ac_pc + 4;
}

void ac_behavior( Type_VLIW_Reg ){}
void ac_behavior( Type_VLIW_Imm ){}
void ac_behavior( Type_Branch ){}
void ac_behavior( Type_Halt ){}

// Basic ALU
void ac_behavior( add_reg ){
  if (!c6x_eval_cond(creg, z, RBA, RBB)) return;
  uint32_t v1 = s ? RBB[src1] : RBA[src1];
  uint32_t v2 = s ? RBB[src2] : RBA[src2];
  if (s) RBB[dst] = v1 + v2; else RBA[dst] = v1 + v2;
}

void ac_behavior( sub_reg ){
  if (!c6x_eval_cond(creg, z, RBA, RBB)) return;
  uint32_t v1 = s ? RBB[src1] : RBA[src1];
  uint32_t v2 = s ? RBB[src2] : RBA[src2];
  if (s) RBB[dst] = v1 - v2; else RBA[dst] = v1 - v2;
}

void ac_behavior( and_reg ){
  if (!c6x_eval_cond(creg, z, RBA, RBB)) return;
  uint32_t v1 = s ? RBB[src1] : RBA[src1];
  uint32_t v2 = s ? RBB[src2] : RBA[src2];
  if (s) RBB[dst] = v1 & v2; else RBA[dst] = v1 & v2;
}

void ac_behavior( or_reg ){
  if (!c6x_eval_cond(creg, z, RBA, RBB)) return;
  uint32_t v1 = s ? RBB[src1] : RBA[src1];
  uint32_t v2 = s ? RBB[src2] : RBA[src2];
  if (s) RBB[dst] = v1 | v2; else RBA[dst] = v1 | v2;
}

void ac_behavior( xor_reg ){
  if (!c6x_eval_cond(creg, z, RBA, RBB)) return;
  uint32_t v1 = s ? RBB[src1] : RBA[src1];
  uint32_t v2 = s ? RBB[src2] : RBA[src2];
  if (s) RBB[dst] = v1 ^ v2; else RBA[dst] = v1 ^ v2;
}

// Saturated Math
void ac_behavior( sadd_reg ){
  if (!c6x_eval_cond(creg, z, RBA, RBB)) return;
  int64_t v1 = (int64_t)(int32_t)(s ? RBB[src1] : RBA[src1]);
  int64_t v2 = (int64_t)(int32_t)(s ? RBB[src2] : RBA[src2]);
  int64_t sum = v1 + v2;
  if (sum > 0x7FFFFFFFLL) { sum = 0x7FFFFFFFLL; csr |= 1; }
  else if (sum < -0x80000000LL) { sum = -0x80000000LL; csr |= 1; }
  if (s) RBB[dst] = (uint32_t)sum; else RBA[dst] = (uint32_t)sum;
}

void ac_behavior( ssub_reg ){
  if (!c6x_eval_cond(creg, z, RBA, RBB)) return;
  int64_t v1 = (int64_t)(int32_t)(s ? RBB[src1] : RBA[src1]);
  int64_t v2 = (int64_t)(int32_t)(s ? RBB[src2] : RBA[src2]);
  int64_t diff = v1 - v2;
  if (diff > 0x7FFFFFFFLL) { diff = 0x7FFFFFFFLL; csr |= 1; }
  else if (diff < -0x80000000LL) { diff = -0x80000000LL; csr |= 1; }
  if (s) RBB[dst] = (uint32_t)diff; else RBA[dst] = (uint32_t)diff;
}

void ac_behavior( mpy_reg ){
  if (!c6x_eval_cond(creg, z, RBA, RBB)) return;
  int32_t v1 = (int32_t)(int16_t)(s ? RBB[src1] : RBA[src1]);
  int32_t v2 = (int32_t)(int16_t)(s ? RBB[src2] : RBA[src2]);
  int32_t prod = v1 * v2;
  if (s) RBB[dst] = (uint32_t)prod; else RBA[dst] = (uint32_t)prod;
}

void ac_behavior( smpy_reg ){
  if (!c6x_eval_cond(creg, z, RBA, RBB)) return;
  int64_t v1 = (int64_t)(int16_t)(s ? RBB[src1] : RBA[src1]);
  int64_t v2 = (int64_t)(int16_t)(s ? RBB[src2] : RBA[src2]);
  int64_t prod = (v1 * v2) << 1;
  if (prod > 0x7FFFFFFFLL) { prod = 0x7FFFFFFFLL; csr |= 1; }
  if (s) RBB[dst] = (uint32_t)prod; else RBA[dst] = (uint32_t)prod;
}

// Memory
void ac_behavior( ldw ){
  if (!c6x_eval_cond(creg, z, RBA, RBB)) return;
  uint32_t addr = s ? RBB[src1] : RBA[src1];
  uint32_t val = DM.read(addr);
  if (s) RBB[dst] = val; else RBA[dst] = val;
}

void ac_behavior( stw ){
  if (!c6x_eval_cond(creg, z, RBA, RBB)) return;
  uint32_t addr = s ? RBB[src1] : RBA[src1];
  uint32_t val = s ? RBB[src2] : RBA[src2];
  DM.write(addr, val);
}

// Immediate
void ac_behavior( mvkl ){
  if (!c6x_eval_cond(creg, z, RBA, RBB)) return;
  RBA[dst] = (int32_t)cst;
}

void ac_behavior( mvkh ){
  if (!c6x_eval_cond(creg, z, RBA, RBB)) return;
  RBA[dst] = (RBA[dst] & 0xFFFFU) | ((uint32_t)cst << 16);
}

void ac_behavior( add_imm ){
  if (!c6x_eval_cond(creg, z, RBA, RBB)) return;
  RBA[dst] = RBA[dst] + (int32_t)cst;
}

void ac_behavior( sub_imm ){
  if (!c6x_eval_cond(creg, z, RBA, RBB)) return;
  RBA[dst] = RBA[dst] - (int32_t)cst;
}

// Branch
void ac_behavior( b_disp ){
  if (!c6x_eval_cond(creg, z, RBA, RBB)) return;
  ac_pc = (ac_pc - 4) + (disp << 2);
}

void ac_behavior( halt ){
  stop();
}
