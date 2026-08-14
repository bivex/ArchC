#include "m6502_isa.H"
#include "m6502_isa_init.cpp"
#include "m6502_bhv_macros.H"

using namespace m6502_parms;

static inline void m6502_set_zn(uint8_t val, ac_reg<uint8_t>& status) {
  bool Z = (val == 0);
  bool N = (val >> 7) & 1;
  status = (status & ~0x82) | (N << 7) | (Z << 1);
}

void ac_behavior( begin ){
  status = 0x20;
  sp_reg = 0xFF;
}

void ac_behavior( end ){}

void ac_behavior( instruction ){}

void ac_behavior( Type_Imp ){
  ac_pc = ac_pc + 1;
}

void ac_behavior( Type_Imm ){
  ac_pc = ac_pc + 2;
}

void ac_behavior( Type_ZP ){
  ac_pc = ac_pc + 2;
}

void ac_behavior( Type_Abs ){
  ac_pc = ac_pc + 3;
}

void ac_behavior( Type_Branch ){
  ac_pc = ac_pc + 2;
}

// Implied
void ac_behavior( tax ){
  x = a;
  m6502_set_zn(x, status);
}

void ac_behavior( txa ){
  a = x;
  m6502_set_zn(a, status);
}

void ac_behavior( tay ){
  y = a;
  m6502_set_zn(y, status);
}

void ac_behavior( tya ){
  a = y;
  m6502_set_zn(a, status);
}

void ac_behavior( inx ){
  x = x + 1;
  m6502_set_zn(x, status);
}

void ac_behavior( dex ){
  x = x - 1;
  m6502_set_zn(x, status);
}

void ac_behavior( iny ){
  y = y + 1;
  m6502_set_zn(y, status);
}

void ac_behavior( dey ){
  y = y - 1;
  m6502_set_zn(y, status);
}

void ac_behavior( clc ){
  status = status & ~0x01;
}

void ac_behavior( sec ){
  status = status | 0x01;
}

void ac_behavior( nop_op ){}

void ac_behavior( brk_op ){
  stop();
}

// Immediate
void ac_behavior( lda_imm ){
  a = imm;
  m6502_set_zn(a, status);
}

void ac_behavior( ldx_imm ){
  x = imm;
  m6502_set_zn(x, status);
}

void ac_behavior( ldy_imm ){
  y = imm;
  m6502_set_zn(y, status);
}

void ac_behavior( adc_imm ){
  uint8_t c = status & 1;
  uint16_t sum = (uint16_t)a + (uint16_t)imm + c;
  bool V = (~(a ^ imm) & (a ^ (uint8_t)sum) & 0x80) != 0;
  bool C = sum > 0xFF;
  a = (uint8_t)sum;
  status = (status & ~0xC3) | ((C ? 1 : 0) << 0) | (V ? 0x40 : 0);
  m6502_set_zn(a, status);
}

void ac_behavior( sbc_imm ){
  uint8_t c = status & 1;
  uint16_t diff = (uint16_t)a - (uint16_t)imm - (1 - c);
  bool V = ((a ^ imm) & (a ^ (uint8_t)diff) & 0x80) != 0;
  bool C = diff < 0x100;
  a = (uint8_t)diff;
  status = (status & ~0xC3) | ((C ? 1 : 0) << 0) | (V ? 0x40 : 0);
  m6502_set_zn(a, status);
}

void ac_behavior( and_imm ){
  a = a & imm;
  m6502_set_zn(a, status);
}

void ac_behavior( ora_imm ){
  a = a | imm;
  m6502_set_zn(a, status);
}

void ac_behavior( eor_imm ){
  a = a ^ imm;
  m6502_set_zn(a, status);
}

void ac_behavior( cmp_imm ){
  uint8_t res = a - imm;
  status = (status & ~0x83) | ((a >= imm) ? 1 : 0);
  m6502_set_zn(res, status);
}

void ac_behavior( cpx_imm ){
  uint8_t res = x - imm;
  status = (status & ~0x83) | ((x >= imm) ? 1 : 0);
  m6502_set_zn(res, status);
}

void ac_behavior( cpy_imm ){
  uint8_t res = y - imm;
  status = (status & ~0x83) | ((y >= imm) ? 1 : 0);
  m6502_set_zn(res, status);
}

// Zero-Page
void ac_behavior( lda_zp ){
  a = DM.read_byte(zp);
  m6502_set_zn(a, status);
}

void ac_behavior( sta_zp ){
  DM.write_byte(zp, a);
}

void ac_behavior( ldx_zp ){
  x = DM.read_byte(zp);
  m6502_set_zn(x, status);
}

void ac_behavior( stx_zp ){
  DM.write_byte(zp, x);
}

void ac_behavior( ldy_zp ){
  y = DM.read_byte(zp);
  m6502_set_zn(y, status);
}

void ac_behavior( sty_zp ){
  DM.write_byte(zp, y);
}

// Absolute
void ac_behavior( lda_abs ){
  a = DM.read_byte(addr);
  m6502_set_zn(a, status);
}

void ac_behavior( sta_abs ){
  DM.write_byte(addr, a);
}

void ac_behavior( jmp_abs ){
  ac_pc = addr;
}

// Branch
void ac_behavior( bne ){
  bool Z = (status >> 1) & 1;
  if (!Z) {
    ac_pc = ac_pc + bdisp;
  }
}

void ac_behavior( beq ){
  bool Z = (status >> 1) & 1;
  if (Z) {
    ac_pc = ac_pc + bdisp;
  }
}

void ac_behavior( bcs ){
  bool C = status & 1;
  if (C) {
    ac_pc = ac_pc + bdisp;
  }
}

void ac_behavior( bcc ){
  bool C = status & 1;
  if (!C) {
    ac_pc = ac_pc + bdisp;
  }
}

void ac_behavior( bmi ){
  bool N = (status >> 7) & 1;
  if (N) {
    ac_pc = ac_pc + bdisp;
  }
}

void ac_behavior( bpl ){
  bool N = (status >> 7) & 1;
  if (!N) {
    ac_pc = ac_pc + bdisp;
  }
}
