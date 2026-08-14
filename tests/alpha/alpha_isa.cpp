#include "alpha_isa.H"
#include "alpha_isa_init.cpp"
#include "alpha_bhv_macros.H"

using namespace alpha_parms;

void ac_behavior( begin ){
  RB[31] = 0;
}

void ac_behavior( end ){}

void ac_behavior( instruction ){
  ac_pc = ac_pc + 4;
}

void ac_behavior( Type_OpReg ){}
void ac_behavior( Type_OpLit ){}
void ac_behavior( Type_Memory ){}
void ac_behavior( Type_Branch ){}
void ac_behavior( Type_Halt ){}

// Operate
void ac_behavior( addq ){
  RB[rc] = (uint64_t)RB[ra] + (uint64_t)RB[rb];
  RB[31] = 0;
}

void ac_behavior( subq ){
  RB[rc] = (uint64_t)RB[ra] - (uint64_t)RB[rb];
  RB[31] = 0;
}

void ac_behavior( mulq ){
  RB[rc] = (int64_t)RB[ra] * (int64_t)RB[rb];
  RB[31] = 0;
}

void ac_behavior( cmpeq ){
  RB[rc] = (RB[ra] == RB[rb]) ? 1ULL : 0ULL;
  RB[31] = 0;
}

void ac_behavior( cmplt ){
  RB[rc] = ((int64_t)RB[ra] < (int64_t)RB[rb]) ? 1ULL : 0ULL;
  RB[31] = 0;
}

void ac_behavior( cmple ){
  RB[rc] = ((int64_t)RB[ra] <= (int64_t)RB[rb]) ? 1ULL : 0ULL;
  RB[31] = 0;
}

void ac_behavior( cmpult ){
  RB[rc] = ((uint64_t)RB[ra] < (uint64_t)RB[rb]) ? 1ULL : 0ULL;
  RB[31] = 0;
}

void ac_behavior( cmpule ){
  RB[rc] = ((uint64_t)RB[ra] <= (uint64_t)RB[rb]) ? 1ULL : 0ULL;
  RB[31] = 0;
}

void ac_behavior( and_op ){
  RB[rc] = (uint64_t)RB[ra] & (uint64_t)RB[rb];
  RB[31] = 0;
}

void ac_behavior( bis_op ){
  RB[rc] = (uint64_t)RB[ra] | (uint64_t)RB[rb];
  RB[31] = 0;
}

void ac_behavior( xor_op ){
  RB[rc] = (uint64_t)RB[ra] ^ (uint64_t)RB[rb];
  RB[31] = 0;
}

void ac_behavior( bic_op ){
  RB[rc] = (uint64_t)RB[ra] & ~(uint64_t)RB[rb];
  RB[31] = 0;
}

void ac_behavior( cmoveq ){
  if (RB[ra] == 0) RB[rc] = RB[rb];
  RB[31] = 0;
}

void ac_behavior( cmovne ){
  if (RB[ra] != 0) RB[rc] = RB[rb];
  RB[31] = 0;
}

void ac_behavior( addq_i ){
  RB[rc] = (uint64_t)RB[ra] + (uint64_t)lit;
  RB[31] = 0;
}

void ac_behavior( subq_i ){
  RB[rc] = (uint64_t)RB[ra] - (uint64_t)lit;
  RB[31] = 0;
}

// Memory
void ac_behavior( lda ){
  RB[ra] = (uint64_t)RB[rb] + (int64_t)disp;
  RB[31] = 0;
}

void ac_behavior( ldah ){
  RB[ra] = (uint64_t)RB[rb] + ((int64_t)disp << 16);
  RB[31] = 0;
}

void ac_behavior( ldq ){
  uint64_t addr = (uint64_t)RB[rb] + (int64_t)disp;
  RB[ra] = DM.read(addr);
  RB[31] = 0;
}

void ac_behavior( stq ){
  uint64_t addr = (uint64_t)RB[rb] + (int64_t)disp;
  DM.write(addr, RB[ra]);
}

void ac_behavior( ldl ){
  uint64_t addr = (uint64_t)RB[rb] + (int64_t)disp;
  RB[ra] = (int64_t)(int32_t)DM.read(addr);
  RB[31] = 0;
}

void ac_behavior( stl ){
  uint64_t addr = (uint64_t)RB[rb] + (int64_t)disp;
  DM.write(addr, (uint32_t)RB[ra]);
}

// Branch
void ac_behavior( br ){
  RB[ra] = ac_pc;
  ac_pc = (ac_pc - 4) + (bdisp << 2);
  RB[31] = 0;
}

void ac_behavior( bne ){
  if (RB[ra] != 0) {
    ac_pc = (ac_pc - 4) + (bdisp << 2);
  }
}

void ac_behavior( beq ){
  if (RB[ra] == 0) {
    ac_pc = (ac_pc - 4) + (bdisp << 2);
  }
}

void ac_behavior( blt ){
  if ((int64_t)RB[ra] < 0) {
    ac_pc = (ac_pc - 4) + (bdisp << 2);
  }
}

void ac_behavior( bge ){
  if ((int64_t)RB[ra] >= 0) {
    ac_pc = (ac_pc - 4) + (bdisp << 2);
  }
}

void ac_behavior( halt ){
  stop();
}
