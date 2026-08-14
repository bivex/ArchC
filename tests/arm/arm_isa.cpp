#include "arm_isa.H"
#include "arm_isa_init.cpp"
#include "arm_bhv_macros.H"

using namespace arm_parms;

static inline bool check_cond(uint32_t cond, uint32_t cpsr) {
  if (cond == 0xE) return true; // AL (Always)
  bool N = (cpsr >> 31) & 1;
  bool Z = (cpsr >> 30) & 1;
  bool C = (cpsr >> 29) & 1;
  bool V = (cpsr >> 28) & 1;
  switch (cond) {
    case 0x0: return Z;             // EQ
    case 0x1: return !Z;            // NE
    case 0x2: return C;             // CS / HS
    case 0x3: return !C;            // CC / LO
    case 0x4: return N;             // MI
    case 0x5: return !N;            // PL
    case 0x6: return V;             // VS
    case 0x7: return !V;            // VC
    case 0x8: return C && !Z;       // HI
    case 0x9: return !C || Z;       // LS
    case 0xA: return N == V;        // GE
    case 0xB: return N != V;        // LT
    case 0xC: return !Z && (N == V);// GT
    case 0xD: return Z || (N != V); // LE
    case 0xE: return true;          // AL
    default: return false;
  }
}

void ac_behavior( begin ){
  CPSR = 0;
}

void ac_behavior( end ){}

void ac_behavior( instruction ){
  ac_pc = ac_pc + 4;
}

void ac_behavior( Type_DP ){}
void ac_behavior( Type_MEM ){}
void ac_behavior( Type_BR ){}
void ac_behavior( Type_HLT ){}

void ac_behavior( add ){
  if (!check_cond(cond, CPSR)) return;
  RB[rd] = RB[rn] + op2;
  if (s) {
    uint32_t res = RB[rd];
    uint32_t flags = CPSR;
    flags = (flags & 0x0FFFFFFF) | ((res >> 31) << 31) | ((res == 0) << 30);
    CPSR = flags;
  }
}

void ac_behavior( sub ){
  if (!check_cond(cond, CPSR)) return;
  RB[rd] = RB[rn] - op2;
  if (s) {
    uint32_t res = RB[rd];
    uint32_t flags = CPSR;
    flags = (flags & 0x0FFFFFFF) | ((res >> 31) << 31) | ((res == 0) << 30);
    CPSR = flags;
  }
}

void ac_behavior( mov ){
  if (!check_cond(cond, CPSR)) return;
  RB[rd] = op2;
  if (s) {
    uint32_t res = RB[rd];
    uint32_t flags = CPSR;
    flags = (flags & 0x0FFFFFFF) | ((res >> 31) << 31) | ((res == 0) << 30);
    CPSR = flags;
  }
}

void ac_behavior( cmp_op ){
  if (!check_cond(cond, CPSR)) return;
  uint32_t a = RB[rn];
  uint32_t b = op2;
  uint32_t res = a - b;
  uint32_t flags = CPSR & 0x0FFFFFFF;
  if (res & 0x80000000) flags |= (1U << 31); // N
  if (res == 0) flags |= (1U << 30);          // Z
  if (a >= b) flags |= (1U << 29);            // C
  CPSR = flags;
}

void ac_behavior( and_op ){
  if (!check_cond(cond, CPSR)) return;
  RB[rd] = RB[rn] & op2;
}

void ac_behavior( orr ){
  if (!check_cond(cond, CPSR)) return;
  RB[rd] = RB[rn] | op2;
}

void ac_behavior( eor ){
  if (!check_cond(cond, CPSR)) return;
  RB[rd] = RB[rn] ^ op2;
}

void ac_behavior( ldr ){
  if (!check_cond(cond, CPSR)) return;
  uint32_t addr = RB[rn] + offset;
  RB[rd] = DM.read(addr);
}

void ac_behavior( str ){
  if (!check_cond(cond, CPSR)) return;
  uint32_t addr = RB[rn] + offset;
  DM.write(addr, RB[rd]);
}

void ac_behavior( b ){
  if (!check_cond(cond, CPSR)) return;
  ac_pc = (ac_pc - 4) + (offset << 2);
}

void ac_behavior( bl ){
  if (!check_cond(cond, CPSR)) return;
  RB[14] = ac_pc;
  ac_pc = (ac_pc - 4) + (offset << 2);
}

void ac_behavior( hlt ){
  stop();
}
