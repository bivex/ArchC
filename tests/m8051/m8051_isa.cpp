#include "m8051_isa.H"
#include "m8051_isa_init.cpp"
#include "m8051_bhv_macros.H"

using namespace m8051_parms;

void ac_behavior( begin ){
  acc = 0;
  psw = 0;
}

void ac_behavior( end ){}

void ac_behavior( instruction ){
  ac_pc = ac_pc + 1;
}

void ac_behavior( Type_1B ){}
void ac_behavior( Type_2B ){
  ac_pc = ac_pc + 1;
}
void ac_behavior( Type_2BR ){
  ac_pc = ac_pc + 1;
}
void ac_behavior( Type_REL ){
  ac_pc = ac_pc + 1;
}
void ac_behavior( Type_DJNZ ){
  ac_pc = ac_pc + 1;
}
void ac_behavior( Type_HLT ){}

void ac_behavior( add_a_rn ){
  acc = (uint8_t)(acc + RB[rn1]);
}

void ac_behavior( sub_a_rn ){
  acc = (uint8_t)(acc - RB[rn1]);
}

void ac_behavior( anl_a_rn ){
  acc = (uint8_t)(acc & RB[rn1]);
}

void ac_behavior( orl_a_rn ){
  acc = (uint8_t)(acc | RB[rn1]);
}

void ac_behavior( xrl_a_rn ){
  acc = (uint8_t)(acc ^ RB[rn1]);
}

void ac_behavior( mov_a_rn ){
  acc = RB[rn1];
}

void ac_behavior( mov_rn_a ){
  RB[rn1] = acc;
}

void ac_behavior( inc_rn ){
  RB[rn1] = (uint8_t)(RB[rn1] + 1);
}

void ac_behavior( dec_rn ){
  RB[rn1] = (uint8_t)(RB[rn1] - 1);
}

void ac_behavior( mov_a_imm ){
  acc = imm2;
}

void ac_behavior( add_a_imm ){
  acc = (uint8_t)(acc + imm2);
}

void ac_behavior( mov_rn_imm ){
  RB[rn3] = imm3;
}

void ac_behavior( sjmp ){
  ac_pc = (ac_pc - 2) + offset4;
}

void ac_behavior( jz ){
  if (acc == 0) {
    ac_pc = (ac_pc - 2) + offset4;
  }
}

void ac_behavior( jnz ){
  if (acc != 0) {
    ac_pc = (ac_pc - 2) + offset4;
  }
}

void ac_behavior( djnz_rn ){
  uint8_t val = (uint8_t)(RB[rn5] - 1);
  RB[rn5] = val;
  if (val != 0) {
    ac_pc = (ac_pc - 2) + offset5;
  }
}

void ac_behavior( halt ){
  stop();
}
