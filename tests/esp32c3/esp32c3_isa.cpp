#include "esp32c3_isa.H"
#include "esp32c3_isa_init.cpp"
#include "esp32c3_bhv_macros.H"

using namespace esp32c3_parms;

void ac_behavior( begin ){
  RB[0] = 0;
  mstatus = 0;
}

void ac_behavior( end ){}

void ac_behavior( instruction ){
  ac_pc = ac_pc + 4;
  RB[0] = 0;
}

void ac_behavior( Type_R ){}
void ac_behavior( Type_I ){}
void ac_behavior( Type_S ){}
void ac_behavior( Type_B ){}
void ac_behavior( Type_U ){}
void ac_behavior( Type_J ){}
void ac_behavior( Type_H ){}

// Type_R (RV32I + RV32M)
void ac_behavior( add ){
  RB[rd] = RB[rs1] + RB[rs2];
}

void ac_behavior( sub ){
  RB[rd] = RB[rs1] - RB[rs2];
}

void ac_behavior( and_op ){
  RB[rd] = RB[rs1] & RB[rs2];
}

void ac_behavior( or_op ){
  RB[rd] = RB[rs1] | RB[rs2];
}

void ac_behavior( xor_op ){
  RB[rd] = RB[rs1] ^ RB[rs2];
}

void ac_behavior( sll_op ){
  RB[rd] = RB[rs1] << (RB[rs2] & 0x1F);
}

void ac_behavior( srl_op ){
  RB[rd] = RB[rs1] >> (RB[rs2] & 0x1F);
}

void ac_behavior( sra_op ){
  RB[rd] = (int32_t)RB[rs1] >> (RB[rs2] & 0x1F);
}

void ac_behavior( slt ){
  RB[rd] = ((int32_t)RB[rs1] < (int32_t)RB[rs2]) ? 1 : 0;
}

void ac_behavior( sltu ){
  RB[rd] = (RB[rs1] < RB[rs2]) ? 1 : 0;
}

void ac_behavior( mul_op ){
  RB[rd] = (int32_t)RB[rs1] * (int32_t)RB[rs2];
}

void ac_behavior( mulh_op ){
  RB[rd] = ((int64_t)(int32_t)RB[rs1] * (int64_t)(int32_t)RB[rs2]) >> 32;
}

void ac_behavior( mulhu_op ){
  RB[rd] = ((uint64_t)RB[rs1] * (uint64_t)RB[rs2]) >> 32;
}

void ac_behavior( div_op ){
  if (RB[rs2] != 0) RB[rd] = (int32_t)RB[rs1] / (int32_t)RB[rs2];
}

void ac_behavior( divu_op ){
  if (RB[rs2] != 0) RB[rd] = RB[rs1] / RB[rs2];
}

void ac_behavior( rem_op ){
  if (RB[rs2] != 0) RB[rd] = (int32_t)RB[rs1] % (int32_t)RB[rs2];
}

void ac_behavior( remu_op ){
  if (RB[rs2] != 0) RB[rd] = RB[rs1] % RB[rs2];
}

// Type_I
void ac_behavior( addi ){
  RB[rd] = RB[rs1] + imm12;
}

void ac_behavior( andi ){
  RB[rd] = RB[rs1] & imm12;
}

void ac_behavior( ori ){
  RB[rd] = RB[rs1] | imm12;
}

void ac_behavior( xori ){
  RB[rd] = RB[rs1] ^ imm12;
}

void ac_behavior( slli ){
  RB[rd] = RB[rs1] << (imm12 & 0x1F);
}

void ac_behavior( srli ){
  RB[rd] = RB[rs1] >> (imm12 & 0x1F);
}

void ac_behavior( slti ){
  RB[rd] = ((int32_t)RB[rs1] < imm12) ? 1 : 0;
}

void ac_behavior( sltiu ){
  RB[rd] = (RB[rs1] < (uint32_t)imm12) ? 1 : 0;
}

void ac_behavior( jalr ){
  uint32_t target = (RB[rs1] + imm12) & ~1;
  RB[rd] = ac_pc;
  ac_pc = target;
}

void ac_behavior( lw ){
  RB[rd] = DM.read(RB[rs1] + imm12);
}

void ac_behavior( lh ){
  RB[rd] = (int16_t)DM.read_half(RB[rs1] + imm12);
}

void ac_behavior( lb ){
  RB[rd] = (int8_t)DM.read_byte(RB[rs1] + imm12);
}

// Type_S
void ac_behavior( sw ){
  int32_t offset = (imm7 << 5) | (imm5 & 0x1F);
  DM.write(RB[rs1] + offset, RB[rs2]);
}

void ac_behavior( sh ){
  int32_t offset = (imm7 << 5) | (imm5 & 0x1F);
  DM.write_half(RB[rs1] + offset, RB[rs2] & 0xFFFF);
}

void ac_behavior( sb ){
  int32_t offset = (imm7 << 5) | (imm5 & 0x1F);
  DM.write_byte(RB[rs1] + offset, RB[rs2] & 0xFF);
}

// Type_B
void ac_behavior( beq ){
  int32_t offset = (imm12 << 12) | (imm11 << 11) | (imm10 << 5) | (imm4 << 1);
  if (RB[rs1] == RB[rs2]) ac_pc = (ac_pc - 4) + offset;
}

void ac_behavior( bne ){
  int32_t offset = (imm12 << 12) | (imm11 << 11) | (imm10 << 5) | (imm4 << 1);
  if (RB[rs1] != RB[rs2]) ac_pc = (ac_pc - 4) + offset;
}

void ac_behavior( blt ){
  int32_t offset = (imm12 << 12) | (imm11 << 11) | (imm10 << 5) | (imm4 << 1);
  if ((int32_t)RB[rs1] < (int32_t)RB[rs2]) ac_pc = (ac_pc - 4) + offset;
}

void ac_behavior( bge ){
  int32_t offset = (imm12 << 12) | (imm11 << 11) | (imm10 << 5) | (imm4 << 1);
  if ((int32_t)RB[rs1] >= (int32_t)RB[rs2]) ac_pc = (ac_pc - 4) + offset;
}

void ac_behavior( bltu ){
  int32_t offset = (imm12 << 12) | (imm11 << 11) | (imm10 << 5) | (imm4 << 1);
  if (RB[rs1] < RB[rs2]) ac_pc = (ac_pc - 4) + offset;
}

void ac_behavior( bgeu ){
  int32_t offset = (imm12 << 12) | (imm11 << 11) | (imm10 << 5) | (imm4 << 1);
  if (RB[rs1] >= RB[rs2]) ac_pc = (ac_pc - 4) + offset;
}

// Type_U
void ac_behavior( lui ){
  RB[rd] = imm20 << 12;
}

void ac_behavior( auipc ){
  RB[rd] = (ac_pc - 4) + (imm20 << 12);
}

// Type_J
void ac_behavior( jal ){
  int32_t offset = (imm20 << 20) | (imm11 << 11) | (imm8 << 12) | (imm10 << 1);
  RB[rd] = ac_pc;
  ac_pc = (ac_pc - 4) + offset;
}

// Type_H
void ac_behavior( halt ){
  stop();
}
