#ifndef _RISCV_BHV_MACROS_H
#define _RISCV_BHV_MACROS_H

typedef ac_memport<riscv_parms::ac_word, riscv_parms::ac_Hword> ac_memory;

#define ac_behavior(instr) AC_BEHAVIOR_##instr ()

#define AC_BEHAVIOR_instruction() riscv_parms::riscv_isa::_behavior_instruction(uint8_t opcode)

#define AC_BEHAVIOR_begin() riscv_parms::riscv_isa::_behavior_begin()
#define AC_BEHAVIOR_end() riscv_parms::riscv_isa::_behavior_end()

#define AC_BEHAVIOR_Type_R() riscv_parms::riscv_isa::_behavior_riscv_Type_R(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_Type_I() riscv_parms::riscv_isa::_behavior_riscv_Type_I(int16_t imm, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_Type_Shift() riscv_parms::riscv_isa::_behavior_riscv_Type_Shift(uint8_t funct7, uint8_t shamt, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_Type_S() riscv_parms::riscv_isa::_behavior_riscv_Type_S(int8_t imm_hi, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t imm_lo, uint8_t opcode)
#define AC_BEHAVIOR_Type_B() riscv_parms::riscv_isa::_behavior_riscv_Type_B(int8_t imm_hi, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t imm_lo, uint8_t opcode)
#define AC_BEHAVIOR_Type_U() riscv_parms::riscv_isa::_behavior_riscv_Type_U(uint32_t imm, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_Type_J() riscv_parms::riscv_isa::_behavior_riscv_Type_J(int32_t imm, uint8_t rd, uint8_t opcode)

#define AC_BEHAVIOR_add() riscv_parms::riscv_isa::behavior_add(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_sub() riscv_parms::riscv_isa::behavior_sub(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_sll() riscv_parms::riscv_isa::behavior_sll(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_slt() riscv_parms::riscv_isa::behavior_slt(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_sltu() riscv_parms::riscv_isa::behavior_sltu(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_xor_op() riscv_parms::riscv_isa::behavior_xor_op(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_srl() riscv_parms::riscv_isa::behavior_srl(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_sra() riscv_parms::riscv_isa::behavior_sra(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_or_op() riscv_parms::riscv_isa::behavior_or_op(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_and_op() riscv_parms::riscv_isa::behavior_and_op(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_addi() riscv_parms::riscv_isa::behavior_addi(int16_t imm, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_slti() riscv_parms::riscv_isa::behavior_slti(int16_t imm, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_sltiu() riscv_parms::riscv_isa::behavior_sltiu(int16_t imm, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_xori() riscv_parms::riscv_isa::behavior_xori(int16_t imm, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_ori() riscv_parms::riscv_isa::behavior_ori(int16_t imm, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_andi() riscv_parms::riscv_isa::behavior_andi(int16_t imm, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_lw() riscv_parms::riscv_isa::behavior_lw(int16_t imm, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_jalr() riscv_parms::riscv_isa::behavior_jalr(int16_t imm, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_slli() riscv_parms::riscv_isa::behavior_slli(uint8_t funct7, uint8_t shamt, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_srli() riscv_parms::riscv_isa::behavior_srli(uint8_t funct7, uint8_t shamt, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_srai() riscv_parms::riscv_isa::behavior_srai(uint8_t funct7, uint8_t shamt, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_sw() riscv_parms::riscv_isa::behavior_sw(int8_t imm_hi, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t imm_lo, uint8_t opcode)
#define AC_BEHAVIOR_beq() riscv_parms::riscv_isa::behavior_beq(int8_t imm_hi, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t imm_lo, uint8_t opcode)
#define AC_BEHAVIOR_bne() riscv_parms::riscv_isa::behavior_bne(int8_t imm_hi, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t imm_lo, uint8_t opcode)
#define AC_BEHAVIOR_blt() riscv_parms::riscv_isa::behavior_blt(int8_t imm_hi, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t imm_lo, uint8_t opcode)
#define AC_BEHAVIOR_bge() riscv_parms::riscv_isa::behavior_bge(int8_t imm_hi, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t imm_lo, uint8_t opcode)
#define AC_BEHAVIOR_bltu() riscv_parms::riscv_isa::behavior_bltu(int8_t imm_hi, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t imm_lo, uint8_t opcode)
#define AC_BEHAVIOR_bgeu() riscv_parms::riscv_isa::behavior_bgeu(int8_t imm_hi, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t imm_lo, uint8_t opcode)
#define AC_BEHAVIOR_lui() riscv_parms::riscv_isa::behavior_lui(uint32_t imm, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_auipc() riscv_parms::riscv_isa::behavior_auipc(uint32_t imm, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_jal() riscv_parms::riscv_isa::behavior_jal(int32_t imm, uint8_t rd, uint8_t opcode)
#define AC_BEHAVIOR_halt() riscv_parms::riscv_isa::behavior_halt(int32_t imm, uint8_t rd, uint8_t opcode)


#endif //_RISCV_BHV_MACROS_H

