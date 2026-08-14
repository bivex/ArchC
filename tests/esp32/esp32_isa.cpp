#include "esp32_isa.H"
#include "esp32_isa_init.cpp"
#include "esp32_bhv_macros.H"

using namespace esp32_parms;

void ac_behavior( begin ){
  lcount = 0;
  lbeg = 0;
  lend = 0;
  ps = 0;
}

void ac_behavior( end ){
  // Zero-overhead loop hardware handling
  if (lend != 0 && ac_pc == lend) {
    if (lcount > 1) {
      lcount = lcount - 1;
      ac_pc = lbeg;
    } else {
      lend = 0;
    }
  }
}

void ac_behavior( instruction ){
  ac_pc = ac_pc + 4;
}

void ac_behavior( Type_RRR ){}
void ac_behavior( Type_RRI8 ){}
void ac_behavior( Type_RI16 ){}
void ac_behavior( Type_L32I ){}
void ac_behavior( Type_Branch ){}
void ac_behavior( Type_Jump ){}
void ac_behavior( Type_Loop ){}
void ac_behavior( Type_Halt ){}

// RRR
void ac_behavior( add_op ){
  AR[r] = AR[s] + AR[t];
}

void ac_behavior( sub_op ){
  AR[r] = AR[s] - AR[t];
}

void ac_behavior( and_op ){
  AR[r] = AR[s] & AR[t];
}

void ac_behavior( or_op ){
  AR[r] = AR[s] | AR[t];
}

void ac_behavior( xor_op ){
  AR[r] = AR[s] ^ AR[t];
}

void ac_behavior( mull_op ){
  AR[r] = (int32_t)AR[s] * (int32_t)AR[t];
}

// RRI8 / RI16
void ac_behavior( addi_op ){
  AR[t] = AR[s] + (int32_t)imm8;
}

void ac_behavior( addmi_op ){
  AR[t] = AR[s] + ((int32_t)imm8 << 8);
}

void ac_behavior( movi_op ){
  AR[t] = (int32_t)imm16;
}

// Memory
void ac_behavior( l32i_op ){
  uint32_t addr = AR[s] + (disp << 2);
  AR[t] = DM.read(addr);
}

void ac_behavior( s32i_op ){
  uint32_t addr = AR[s] + (disp << 2);
  DM.write(addr, AR[t]);
}

// Branch
void ac_behavior( bne_op ){
  if (AR[s] != AR[t]) {
    ac_pc = (ac_pc - 4) + (bdisp << 2);
  }
}

void ac_behavior( beq_op ){
  if (AR[s] == AR[t]) {
    ac_pc = (ac_pc - 4) + (bdisp << 2);
  }
}

void ac_behavior( blt_op ){
  if ((int32_t)AR[s] < (int32_t)AR[t]) {
    ac_pc = (ac_pc - 4) + (bdisp << 2);
  }
}

void ac_behavior( bge_op ){
  if ((int32_t)AR[s] >= (int32_t)AR[t]) {
    ac_pc = (ac_pc - 4) + (bdisp << 2);
  }
}

// Jump
void ac_behavior( j_op ){
  ac_pc = (ac_pc - 4) + (target << 2);
}

void ac_behavior( call0_op ){
  AR[0] = ac_pc;
  ac_pc = (ac_pc - 4) + (target << 2);
}

// Hardware Zero-Overhead Loop
void ac_behavior( loop_op ){
  lcount = AR[s];
  lbeg = ac_pc;
  lend = (ac_pc - 4) + (ldisp << 2);
}

void ac_behavior( halt_op ){
  stop();
}
