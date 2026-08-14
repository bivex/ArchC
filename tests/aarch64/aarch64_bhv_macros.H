#ifndef _AARCH64_BHV_MACROS_H
#define _AARCH64_BHV_MACROS_H

typedef ac_memport<aarch64_parms::ac_word, aarch64_parms::ac_Hword> ac_memory;

#define ac_behavior(instr) AC_BEHAVIOR_##instr ()

#define AC_BEHAVIOR_instruction() aarch64_parms::aarch64_isa::_behavior_instruction(uint8_t op)

#define AC_BEHAVIOR_begin() aarch64_parms::aarch64_isa::_behavior_begin()
#define AC_BEHAVIOR_end() aarch64_parms::aarch64_isa::_behavior_end()

#define AC_BEHAVIOR_Type_DP_Reg() aarch64_parms::aarch64_isa::_behavior_aarch64_Type_DP_Reg(uint8_t op, uint8_t opt, uint8_t rm, uint8_t imm6, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_Type_DP_Imm() aarch64_parms::aarch64_isa::_behavior_aarch64_Type_DP_Imm(uint8_t op, uint16_t imm14, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_Type_LS_Imm() aarch64_parms::aarch64_isa::_behavior_aarch64_Type_LS_Imm(uint8_t op, int16_t disp, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_Type_Branch_Cond() aarch64_parms::aarch64_isa::_behavior_aarch64_Type_Branch_Cond(uint8_t op, int32_t imm19, uint8_t o, uint8_t cond)
#define AC_BEHAVIOR_Type_Branch_Imm() aarch64_parms::aarch64_isa::_behavior_aarch64_Type_Branch_Imm(uint8_t op, int32_t imm24)
#define AC_BEHAVIOR_Type_Branch_Reg() aarch64_parms::aarch64_isa::_behavior_aarch64_Type_Branch_Reg(uint16_t op, uint8_t rn, uint16_t pad)
#define AC_BEHAVIOR_Type_Halt() aarch64_parms::aarch64_isa::_behavior_aarch64_Type_Halt(uint8_t op, uint32_t pad)

#define AC_BEHAVIOR_add_reg() aarch64_parms::aarch64_isa::behavior_add_reg(uint8_t op, uint8_t opt, uint8_t rm, uint8_t imm6, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_adds_reg() aarch64_parms::aarch64_isa::behavior_adds_reg(uint8_t op, uint8_t opt, uint8_t rm, uint8_t imm6, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_sub_reg() aarch64_parms::aarch64_isa::behavior_sub_reg(uint8_t op, uint8_t opt, uint8_t rm, uint8_t imm6, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_subs_reg() aarch64_parms::aarch64_isa::behavior_subs_reg(uint8_t op, uint8_t opt, uint8_t rm, uint8_t imm6, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_and_reg() aarch64_parms::aarch64_isa::behavior_and_reg(uint8_t op, uint8_t opt, uint8_t rm, uint8_t imm6, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_orr_reg() aarch64_parms::aarch64_isa::behavior_orr_reg(uint8_t op, uint8_t opt, uint8_t rm, uint8_t imm6, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_eor_reg() aarch64_parms::aarch64_isa::behavior_eor_reg(uint8_t op, uint8_t opt, uint8_t rm, uint8_t imm6, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_mul_reg() aarch64_parms::aarch64_isa::behavior_mul_reg(uint8_t op, uint8_t opt, uint8_t rm, uint8_t imm6, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_add_imm() aarch64_parms::aarch64_isa::behavior_add_imm(uint8_t op, uint16_t imm14, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_adds_imm() aarch64_parms::aarch64_isa::behavior_adds_imm(uint8_t op, uint16_t imm14, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_sub_imm() aarch64_parms::aarch64_isa::behavior_sub_imm(uint8_t op, uint16_t imm14, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_subs_imm() aarch64_parms::aarch64_isa::behavior_subs_imm(uint8_t op, uint16_t imm14, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_movz_imm() aarch64_parms::aarch64_isa::behavior_movz_imm(uint8_t op, uint16_t imm14, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_ldr_x() aarch64_parms::aarch64_isa::behavior_ldr_x(uint8_t op, int16_t disp, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_str_x() aarch64_parms::aarch64_isa::behavior_str_x(uint8_t op, int16_t disp, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_ldr_w() aarch64_parms::aarch64_isa::behavior_ldr_w(uint8_t op, int16_t disp, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_str_w() aarch64_parms::aarch64_isa::behavior_str_w(uint8_t op, int16_t disp, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_b_cond() aarch64_parms::aarch64_isa::behavior_b_cond(uint8_t op, int32_t imm19, uint8_t o, uint8_t cond)
#define AC_BEHAVIOR_b_imm() aarch64_parms::aarch64_isa::behavior_b_imm(uint8_t op, int32_t imm24)
#define AC_BEHAVIOR_bl_imm() aarch64_parms::aarch64_isa::behavior_bl_imm(uint8_t op, int32_t imm24)
#define AC_BEHAVIOR_br_reg() aarch64_parms::aarch64_isa::behavior_br_reg(uint16_t op, uint8_t rn, uint16_t pad)
#define AC_BEHAVIOR_ret_reg() aarch64_parms::aarch64_isa::behavior_ret_reg(uint16_t op, uint8_t rn, uint16_t pad)
#define AC_BEHAVIOR_halt() aarch64_parms::aarch64_isa::behavior_halt(uint8_t op, uint32_t pad)


#endif //_AARCH64_BHV_MACROS_H

