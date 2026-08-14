#include "esp32s3_isa.H"
#include "esp32s3_isa_init.cpp"
#include "esp32s3_bhv_macros.H"

using namespace esp32s3_parms;

void ac_behavior( begin ){
  lcount = 0;
  lbeg = 0;
  lend = 0;
  ps = 0;
}

void ac_behavior( end ){
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
void ac_behavior( Type_PIE ){}
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

// Loop
void ac_behavior( loop_op ){
  lcount = AR[s];
  lbeg = ac_pc;
  lend = (ac_pc - 4) + (ldisp << 2);
}

// PIE AI Vector Extension
void ac_behavior( ee_vadd_s16 ){
  uint64_t vs = QR[qs];
  uint64_t vt = QR[qt];
  int16_t s0 = vs & 0xFFFF, s1 = (vs >> 16) & 0xFFFF, s2 = (vs >> 32) & 0xFFFF, s3 = (vs >> 48) & 0xFFFF;
  int16_t t0 = vt & 0xFFFF, t1 = (vt >> 16) & 0xFFFF, t2 = (vt >> 32) & 0xFFFF, t3 = (vt >> 48) & 0xFFFF;
  uint64_t r0 = (uint16_t)(s0 + t0), r1 = (uint16_t)(s1 + t1), r2 = (uint16_t)(s2 + t2), r3 = (uint16_t)(s3 + t3);
  QR[qr] = r0 | (r1 << 16) | (r2 << 32) | (r3 << 48);
}

void ac_behavior( ee_vmul_s16 ){
  uint64_t vs = QR[qs];
  uint64_t vt = QR[qt];
  int16_t s0 = vs & 0xFFFF, s1 = (vs >> 16) & 0xFFFF, s2 = (vs >> 32) & 0xFFFF, s3 = (vs >> 48) & 0xFFFF;
  int16_t t0 = vt & 0xFFFF, t1 = (vt >> 16) & 0xFFFF, t2 = (vt >> 32) & 0xFFFF, t3 = (vt >> 48) & 0xFFFF;
  uint64_t r0 = (uint16_t)(s0 * t0), r1 = (uint16_t)(s1 * t1), r2 = (uint16_t)(s2 * t2), r3 = (uint16_t)(s3 * t3);
  QR[qr] = r0 | (r1 << 16) | (r2 << 32) | (r3 << 48);
}

void ac_behavior( ee_vdot_s8 ){
  uint64_t vs = QR[qs];
  uint64_t vt = QR[qt];
  int32_t dot = 0;
  for (int i = 0; i < 8; i++) {
    int8_t b1 = (vs >> (i * 8)) & 0xFF;
    int8_t b2 = (vt >> (i * 8)) & 0xFF;
    dot += (int32_t)b1 * (int32_t)b2;
  }
  AR[qr] += dot;
}

void ac_behavior( ee_vld_q ){
  uint32_t addr = AR[qs];
  QR[qr] = (uint64_t)DM.read(addr) | ((uint64_t)DM.read(addr + 4) << 32);
}

void ac_behavior( ee_vst_q ){
  uint32_t addr = AR[qs];
  DM.write(addr, (uint32_t)QR[qr]);
  DM.write(addr + 4, (uint32_t)(QR[qr] >> 32));
}

void ac_behavior( halt_op ){
  stop();
}
