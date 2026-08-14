#include "riscv_isa.H"
#include "riscv_isa_init.cpp"
#include "riscv_bhv_macros.H"

using namespace riscv_parms;

void ac_behavior( begin ){
  RB[0] = 0;
}

void ac_behavior( end ){}

void ac_behavior( instruction ){
  ac_pc = ac_pc + 4;
}

void ac_behavior( Type_R ){}
void ac_behavior( Type_I ){}
void ac_behavior( Type_Shift ){}
void ac_behavior( Type_S ){}
void ac_behavior( Type_B ){}
void ac_behavior( Type_U ){}
void ac_behavior( Type_J ){}

void ac_behavior( add ){
  RB[rd] = RB[rs1] + RB[rs2];
  RB[0] = 0;
}

void ac_behavior( sub ){
  RB[rd] = RB[rs1] - RB[rs2];
  RB[0] = 0;
}

void ac_behavior( sll ){
  RB[rd] = RB[rs1] << (RB[rs2] & 0x1F);
  RB[0] = 0;
}

void ac_behavior( slt ){
  RB[rd] = ((int32_t)RB[rs1] < (int32_t)RB[rs2]) ? 1 : 0;
  RB[0] = 0;
}

void ac_behavior( sltu ){
  RB[rd] = ((uint32_t)RB[rs1] < (uint32_t)RB[rs2]) ? 1 : 0;
  RB[0] = 0;
}

void ac_behavior( xor_op ){
  RB[rd] = RB[rs1] ^ RB[rs2];
  RB[0] = 0;
}

void ac_behavior( srl ){
  RB[rd] = ((uint32_t)RB[rs1]) >> (RB[rs2] & 0x1F);
  RB[0] = 0;
}

void ac_behavior( sra ){
  RB[rd] = ((int32_t)RB[rs1]) >> (RB[rs2] & 0x1F);
  RB[0] = 0;
}

void ac_behavior( or_op ){
  RB[rd] = RB[rs1] | RB[rs2];
  RB[0] = 0;
}

void ac_behavior( and_op ){
  RB[rd] = RB[rs1] & RB[rs2];
  RB[0] = 0;
}

void ac_behavior( addi ){
  RB[rd] = RB[rs1] + (int32_t)imm;
  RB[0] = 0;
}

void ac_behavior( slti ){
  RB[rd] = ((int32_t)RB[rs1] < (int32_t)imm) ? 1 : 0;
  RB[0] = 0;
}

void ac_behavior( sltiu ){
  RB[rd] = ((uint32_t)RB[rs1] < (uint32_t)(int32_t)imm) ? 1 : 0;
  RB[0] = 0;
}

void ac_behavior( xori ){
  RB[rd] = RB[rs1] ^ (int32_t)imm;
  RB[0] = 0;
}

void ac_behavior( ori ){
  RB[rd] = RB[rs1] | (int32_t)imm;
  RB[0] = 0;
}

void ac_behavior( andi ){
  RB[rd] = RB[rs1] & (int32_t)imm;
  RB[0] = 0;
}

void ac_behavior( slli ){
  RB[rd] = RB[rs1] << (shamt & 0x1F);
  RB[0] = 0;
}

void ac_behavior( srli ){
  RB[rd] = ((uint32_t)RB[rs1]) >> (shamt & 0x1F);
  RB[0] = 0;
}

void ac_behavior( srai ){
  RB[rd] = ((int32_t)RB[rs1]) >> (shamt & 0x1F);
  RB[0] = 0;
}

void ac_behavior( lw ){
  uint32_t addr = RB[rs1] + (int32_t)imm;
  RB[rd] = DM.read(addr);
  RB[0] = 0;
}

void ac_behavior( sw ){
  int32_t offset = ((int32_t)imm_hi << 5) | (imm_lo & 0x1F);
  uint32_t addr = RB[rs1] + offset;
  DM.write(addr, RB[rs2]);
}

void ac_behavior( beq ){
  if (RB[rs1] == RB[rs2]) {
    int32_t offset = ((int32_t)imm_hi << 5) | (imm_lo & 0x1F);
    ac_pc = (ac_pc - 4) + (offset << 2);
  }
}

void ac_behavior( bne ){
  if (RB[rs1] != RB[rs2]) {
    int32_t offset = ((int32_t)imm_hi << 5) | (imm_lo & 0x1F);
    ac_pc = (ac_pc - 4) + (offset << 2);
  }
}

void ac_behavior( blt ){
  if ((int32_t)RB[rs1] < (int32_t)RB[rs2]) {
    int32_t offset = ((int32_t)imm_hi << 5) | (imm_lo & 0x1F);
    ac_pc = (ac_pc - 4) + (offset << 2);
  }
}

void ac_behavior( bge ){
  if ((int32_t)RB[rs1] >= (int32_t)RB[rs2]) {
    int32_t offset = ((int32_t)imm_hi << 5) | (imm_lo & 0x1F);
    ac_pc = (ac_pc - 4) + (offset << 2);
  }
}

void ac_behavior( bltu ){
  if ((uint32_t)RB[rs1] < (uint32_t)RB[rs2]) {
    int32_t offset = ((int32_t)imm_hi << 5) | (imm_lo & 0x1F);
    ac_pc = (ac_pc - 4) + (offset << 2);
  }
}

void ac_behavior( bgeu ){
  if ((uint32_t)RB[rs1] >= (uint32_t)RB[rs2]) {
    int32_t offset = ((int32_t)imm_hi << 5) | (imm_lo & 0x1F);
    ac_pc = (ac_pc - 4) + (offset << 2);
  }
}

void ac_behavior( lui ){
  RB[rd] = (uint32_t)imm << 12;
  RB[0] = 0;
}

void ac_behavior( auipc ){
  RB[rd] = (ac_pc - 4) + ((uint32_t)imm << 12);
  RB[0] = 0;
}

void ac_behavior( jal ){
  RB[rd] = ac_pc;
  RB[0] = 0;
  ac_pc = (ac_pc - 4) + ((int32_t)imm << 1);
}

void ac_behavior( jalr ){
  uint32_t target = (RB[rs1] + (int32_t)imm) & ~1U;
  RB[rd] = ac_pc;
  RB[0] = 0;
  ac_pc = target;
}

void ac_behavior( halt ){
  stop();
}
