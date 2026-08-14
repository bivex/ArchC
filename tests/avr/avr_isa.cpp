#include "avr_isa.H"
#include "avr_isa_init.cpp"
#include "avr_bhv_macros.H"

using namespace avr_parms;

static inline void avr_set_flags_add(uint8_t res, uint8_t op1, uint8_t op2, ac_reg<uint8_t>& sreg) {
  bool C = ((uint16_t)op1 + (uint16_t)op2) > 0xFF;
  bool Z = (res == 0);
  bool N = (res >> 7) & 1;
  bool V = (((~(op1 ^ op2)) & (op1 ^ res)) >> 7) & 1;
  bool S = N ^ V;
  sreg = (sreg & ~0x1F) | (S << 4) | (V << 3) | (N << 2) | (Z << 1) | C;
}

static inline void avr_set_flags_sub(uint8_t res, uint8_t op1, uint8_t op2, ac_reg<uint8_t>& sreg) {
  bool C = op1 < op2;
  bool Z = (res == 0);
  bool N = (res >> 7) & 1;
  bool V = (((op1 ^ op2) & (op1 ^ res)) >> 7) & 1;
  bool S = N ^ V;
  sreg = (sreg & ~0x1F) | (S << 4) | (V << 3) | (N << 2) | (Z << 1) | C;
}

void ac_behavior( begin ){
  sreg = 0;
}

void ac_behavior( end ){}

void ac_behavior( instruction ){
  ac_pc = ac_pc + 2;
}

void ac_behavior( Type_2R ){}
void ac_behavior( Type_1R ){}
void ac_behavior( Type_Imm ){}
void ac_behavior( Type_Branch ){}
void ac_behavior( Type_RJMP ){}
void ac_behavior( Type_Ptr ){}
void ac_behavior( Type_Halt ){}

// 2-Register
void ac_behavior( add_r ){
  uint8_t op1 = RB[rd];
  uint8_t op2 = RB[rr];
  uint8_t res = op1 + op2;
  avr_set_flags_add(res, op1, op2, sreg);
  RB[rd] = res;
}

void ac_behavior( adc_r ){
  uint8_t c = sreg & 1;
  uint8_t op1 = RB[rd];
  uint8_t op2 = RB[rr];
  uint8_t res = op1 + op2 + c;
  avr_set_flags_add(res, op1, op2 + c, sreg);
  RB[rd] = res;
}

void ac_behavior( sub_r ){
  uint8_t op1 = RB[rd];
  uint8_t op2 = RB[rr];
  uint8_t res = op1 - op2;
  avr_set_flags_sub(res, op1, op2, sreg);
  RB[rd] = res;
}

void ac_behavior( sbc_r ){
  uint8_t c = sreg & 1;
  uint8_t op1 = RB[rd];
  uint8_t op2 = RB[rr];
  uint8_t res = op1 - op2 - c;
  avr_set_flags_sub(res, op1, op2 + c, sreg);
  RB[rd] = res;
}

void ac_behavior( and_r ){
  uint8_t res = RB[rd] & RB[rr];
  sreg = (sreg & ~0x1E) | (((res >> 7) & 1) << 2) | ((res == 0) << 1);
  RB[rd] = res;
}

void ac_behavior( or_r ){
  uint8_t res = RB[rd] | RB[rr];
  sreg = (sreg & ~0x1E) | (((res >> 7) & 1) << 2) | ((res == 0) << 1);
  RB[rd] = res;
}

void ac_behavior( eor_r ){
  uint8_t res = RB[rd] ^ RB[rr];
  sreg = (sreg & ~0x1E) | (((res >> 7) & 1) << 2) | ((res == 0) << 1);
  RB[rd] = res;
}

void ac_behavior( cp_r ){
  uint8_t op1 = RB[rd];
  uint8_t op2 = RB[rr];
  uint8_t res = op1 - op2;
  avr_set_flags_sub(res, op1, op2, sreg);
}

void ac_behavior( mov_r ){
  RB[rd] = RB[rr];
}

// 1-Register
void ac_behavior( inc_r ){
  uint8_t op1 = RB[rd];
  uint8_t res = op1 + 1;
  bool Z = (res == 0);
  bool N = (res >> 7) & 1;
  bool V = (op1 == 0x7F);
  sreg = (sreg & ~0x1E) | ((N ^ V) << 4) | (V << 3) | (N << 2) | (Z << 1);
  RB[rd] = res;
}

void ac_behavior( dec_r ){
  uint8_t op1 = RB[rd];
  uint8_t res = op1 - 1;
  bool Z = (res == 0);
  bool N = (res >> 7) & 1;
  bool V = (op1 == 0x80);
  sreg = (sreg & ~0x1E) | ((N ^ V) << 4) | (V << 3) | (N << 2) | (Z << 1);
  RB[rd] = res;
}

void ac_behavior( clr_r ){
  RB[rd] = 0;
  sreg = (sreg & ~0x1E) | (1 << 1);
}

// Immediate
void ac_behavior( ldi_i ){
  uint8_t val = (k_hi << 4) | k_lo;
  RB[rd_hi + 16] = val;
}

void ac_behavior( subi_i ){
  uint8_t r = rd_hi + 16;
  uint8_t op1 = RB[r];
  uint8_t op2 = (k_hi << 4) | k_lo;
  uint8_t res = op1 - op2;
  avr_set_flags_sub(res, op1, op2, sreg);
  RB[r] = res;
}

void ac_behavior( andi_i ){
  uint8_t r = rd_hi + 16;
  uint8_t op2 = (k_hi << 4) | k_lo;
  uint8_t res = RB[r] & op2;
  sreg = (sreg & ~0x1E) | (((res >> 7) & 1) << 2) | ((res == 0) << 1);
  RB[r] = res;
}

void ac_behavior( ori_i ){
  uint8_t r = rd_hi + 16;
  uint8_t op2 = (k_hi << 4) | k_lo;
  uint8_t res = RB[r] | op2;
  sreg = (sreg & ~0x1E) | (((res >> 7) & 1) << 2) | ((res == 0) << 1);
  RB[r] = res;
}

void ac_behavior( cpi_i ){
  uint8_t r = rd_hi + 16;
  uint8_t op1 = RB[r];
  uint8_t op2 = (k_hi << 4) | k_lo;
  uint8_t res = op1 - op2;
  avr_set_flags_sub(res, op1, op2, sreg);
}

// Branch
void ac_behavior( brne ){
  bool Z = (sreg >> 1) & 1;
  if (!Z) {
    ac_pc = (ac_pc - 2) + (k_br << 1);
  }
}

void ac_behavior( breq ){
  bool Z = (sreg >> 1) & 1;
  if (Z) {
    ac_pc = (ac_pc - 2) + (k_br << 1);
  }
}

void ac_behavior( brcs ){
  bool C = sreg & 1;
  if (C) {
    ac_pc = (ac_pc - 2) + (k_br << 1);
  }
}

void ac_behavior( brcc ){
  bool C = sreg & 1;
  if (!C) {
    ac_pc = (ac_pc - 2) + (k_br << 1);
  }
}

void ac_behavior( rjmp ){
  ac_pc = (ac_pc - 2) + (k_jmp << 1);
}

// Indirect
void ac_behavior( ld_x ){
  uint16_t addr = ((uint16_t)RB[27] << 8) | RB[26];
  RB[rd] = DM.read_byte(addr);
}

void ac_behavior( st_x ){
  uint16_t addr = ((uint16_t)RB[27] << 8) | RB[26];
  DM.write_byte(addr, RB[rd]);
}

void ac_behavior( ld_z ){
  uint16_t addr = ((uint16_t)RB[31] << 8) | RB[30];
  RB[rd] = DM.read_byte(addr);
}

void ac_behavior( st_z ){
  uint16_t addr = ((uint16_t)RB[31] << 8) | RB[30];
  DM.write_byte(addr, RB[rd]);
}

void ac_behavior( halt ){
  stop();
}
