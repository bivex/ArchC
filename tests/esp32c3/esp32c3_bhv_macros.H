#ifndef _ESP32C3_BHV_MACROS_H
#define _ESP32C3_BHV_MACROS_H

typedef ac_memport<esp32c3_parms::ac_word, esp32c3_parms::ac_Hword> ac_memory;

#define ac_behavior(instr) AC_BEHAVIOR_##instr ()

#define AC_BEHAVIOR_instruction() esp32c3_parms::esp32c3_isa::_behavior_instruction(uint8_t op)

#define AC_BEHAVIOR_begin() esp32c3_parms::esp32c3_isa::_behavior_begin()
#define AC_BEHAVIOR_end() esp32c3_parms::esp32c3_isa::_behavior_end()

#define AC_BEHAVIOR_Type_R() esp32c3_parms::esp32c3_isa::_behavior_esp32c3_Type_R(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_Type_I() esp32c3_parms::esp32c3_isa::_behavior_esp32c3_Type_I(int16_t imm12, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_Type_S() esp32c3_parms::esp32c3_isa::_behavior_esp32c3_Type_S(int8_t imm7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t imm5, uint8_t op)
#define AC_BEHAVIOR_Type_B() esp32c3_parms::esp32c3_isa::_behavior_esp32c3_Type_B(int8_t imm12, uint8_t imm10, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t imm4, uint8_t imm11, uint8_t op)
#define AC_BEHAVIOR_Type_U() esp32c3_parms::esp32c3_isa::_behavior_esp32c3_Type_U(uint32_t imm20, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_Type_J() esp32c3_parms::esp32c3_isa::_behavior_esp32c3_Type_J(int8_t imm20, uint16_t imm10, uint8_t imm11, uint8_t imm8, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_Type_H() esp32c3_parms::esp32c3_isa::_behavior_esp32c3_Type_H(uint8_t op, uint32_t pad)

#define AC_BEHAVIOR_add() esp32c3_parms::esp32c3_isa::behavior_add(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_sub() esp32c3_parms::esp32c3_isa::behavior_sub(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_and_op() esp32c3_parms::esp32c3_isa::behavior_and_op(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_or_op() esp32c3_parms::esp32c3_isa::behavior_or_op(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_xor_op() esp32c3_parms::esp32c3_isa::behavior_xor_op(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_sll_op() esp32c3_parms::esp32c3_isa::behavior_sll_op(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_srl_op() esp32c3_parms::esp32c3_isa::behavior_srl_op(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_sra_op() esp32c3_parms::esp32c3_isa::behavior_sra_op(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_slt() esp32c3_parms::esp32c3_isa::behavior_slt(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_sltu() esp32c3_parms::esp32c3_isa::behavior_sltu(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_mul_op() esp32c3_parms::esp32c3_isa::behavior_mul_op(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_mulh_op() esp32c3_parms::esp32c3_isa::behavior_mulh_op(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_mulhu_op() esp32c3_parms::esp32c3_isa::behavior_mulhu_op(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_div_op() esp32c3_parms::esp32c3_isa::behavior_div_op(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_divu_op() esp32c3_parms::esp32c3_isa::behavior_divu_op(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_rem_op() esp32c3_parms::esp32c3_isa::behavior_rem_op(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_remu_op() esp32c3_parms::esp32c3_isa::behavior_remu_op(uint8_t funct7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_addi() esp32c3_parms::esp32c3_isa::behavior_addi(int16_t imm12, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_andi() esp32c3_parms::esp32c3_isa::behavior_andi(int16_t imm12, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_ori() esp32c3_parms::esp32c3_isa::behavior_ori(int16_t imm12, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_xori() esp32c3_parms::esp32c3_isa::behavior_xori(int16_t imm12, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_slli() esp32c3_parms::esp32c3_isa::behavior_slli(int16_t imm12, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_srli() esp32c3_parms::esp32c3_isa::behavior_srli(int16_t imm12, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_slti() esp32c3_parms::esp32c3_isa::behavior_slti(int16_t imm12, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_sltiu() esp32c3_parms::esp32c3_isa::behavior_sltiu(int16_t imm12, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_jalr() esp32c3_parms::esp32c3_isa::behavior_jalr(int16_t imm12, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_lw() esp32c3_parms::esp32c3_isa::behavior_lw(int16_t imm12, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_lh() esp32c3_parms::esp32c3_isa::behavior_lh(int16_t imm12, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_lb() esp32c3_parms::esp32c3_isa::behavior_lb(int16_t imm12, uint8_t rs1, uint8_t funct3, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_sw() esp32c3_parms::esp32c3_isa::behavior_sw(int8_t imm7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t imm5, uint8_t op)
#define AC_BEHAVIOR_sh() esp32c3_parms::esp32c3_isa::behavior_sh(int8_t imm7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t imm5, uint8_t op)
#define AC_BEHAVIOR_sb() esp32c3_parms::esp32c3_isa::behavior_sb(int8_t imm7, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t imm5, uint8_t op)
#define AC_BEHAVIOR_beq() esp32c3_parms::esp32c3_isa::behavior_beq(int8_t imm12, uint8_t imm10, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t imm4, uint8_t imm11, uint8_t op)
#define AC_BEHAVIOR_bne() esp32c3_parms::esp32c3_isa::behavior_bne(int8_t imm12, uint8_t imm10, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t imm4, uint8_t imm11, uint8_t op)
#define AC_BEHAVIOR_blt() esp32c3_parms::esp32c3_isa::behavior_blt(int8_t imm12, uint8_t imm10, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t imm4, uint8_t imm11, uint8_t op)
#define AC_BEHAVIOR_bge() esp32c3_parms::esp32c3_isa::behavior_bge(int8_t imm12, uint8_t imm10, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t imm4, uint8_t imm11, uint8_t op)
#define AC_BEHAVIOR_bltu() esp32c3_parms::esp32c3_isa::behavior_bltu(int8_t imm12, uint8_t imm10, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t imm4, uint8_t imm11, uint8_t op)
#define AC_BEHAVIOR_bgeu() esp32c3_parms::esp32c3_isa::behavior_bgeu(int8_t imm12, uint8_t imm10, uint8_t rs2, uint8_t rs1, uint8_t funct3, uint8_t imm4, uint8_t imm11, uint8_t op)
#define AC_BEHAVIOR_lui() esp32c3_parms::esp32c3_isa::behavior_lui(uint32_t imm20, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_auipc() esp32c3_parms::esp32c3_isa::behavior_auipc(uint32_t imm20, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_jal() esp32c3_parms::esp32c3_isa::behavior_jal(int8_t imm20, uint16_t imm10, uint8_t imm11, uint8_t imm8, uint8_t rd, uint8_t op)
#define AC_BEHAVIOR_halt() esp32c3_parms::esp32c3_isa::behavior_halt(uint8_t op, uint32_t pad)


#endif //_ESP32C3_BHV_MACROS_H

