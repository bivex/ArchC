#ifndef _APPLE_ARM_BHV_MACROS_H
#define _APPLE_ARM_BHV_MACROS_H

typedef ac_memport<apple_arm_parms::ac_word, apple_arm_parms::ac_Hword> ac_memory;

#define ac_behavior(instr) AC_BEHAVIOR_##instr ()

#define AC_BEHAVIOR_instruction() apple_arm_parms::apple_arm_isa::_behavior_instruction(uint8_t op)

#define AC_BEHAVIOR_begin() apple_arm_parms::apple_arm_isa::_behavior_begin()
#define AC_BEHAVIOR_end() apple_arm_parms::apple_arm_isa::_behavior_end()

#define AC_BEHAVIOR_Type_DP_Reg() apple_arm_parms::apple_arm_isa::_behavior_apple_arm_Type_DP_Reg(uint8_t op, uint8_t opt, uint8_t rm, uint8_t subop, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_Type_DP_Imm() apple_arm_parms::apple_arm_isa::_behavior_apple_arm_Type_DP_Imm(uint8_t op, uint16_t imm14, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_Type_LS_Imm() apple_arm_parms::apple_arm_isa::_behavior_apple_arm_Type_LS_Imm(uint8_t op, int16_t disp, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_Type_PAC() apple_arm_parms::apple_arm_isa::_behavior_apple_arm_Type_PAC(uint8_t op, uint8_t pac_op, uint8_t rn, uint8_t subop, uint8_t rd)
#define AC_BEHAVIOR_Type_Branch_Cond() apple_arm_parms::apple_arm_isa::_behavior_apple_arm_Type_Branch_Cond(uint8_t op, int32_t imm19, uint8_t o, uint8_t cond)
#define AC_BEHAVIOR_Type_Branch_Imm() apple_arm_parms::apple_arm_isa::_behavior_apple_arm_Type_Branch_Imm(uint8_t op, int32_t imm24)
#define AC_BEHAVIOR_Type_Halt() apple_arm_parms::apple_arm_isa::_behavior_apple_arm_Type_Halt(uint8_t op, uint32_t pad)

#define AC_BEHAVIOR_add_x() apple_arm_parms::apple_arm_isa::behavior_add_x(uint8_t op, uint8_t opt, uint8_t rm, uint8_t subop, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_sub_x() apple_arm_parms::apple_arm_isa::behavior_sub_x(uint8_t op, uint8_t opt, uint8_t rm, uint8_t subop, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_and_x() apple_arm_parms::apple_arm_isa::behavior_and_x(uint8_t op, uint8_t opt, uint8_t rm, uint8_t subop, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_eor_x() apple_arm_parms::apple_arm_isa::behavior_eor_x(uint8_t op, uint8_t opt, uint8_t rm, uint8_t subop, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_orr_x() apple_arm_parms::apple_arm_isa::behavior_orr_x(uint8_t op, uint8_t opt, uint8_t rm, uint8_t subop, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_mul_x() apple_arm_parms::apple_arm_isa::behavior_mul_x(uint8_t op, uint8_t opt, uint8_t rm, uint8_t subop, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_amx_fma() apple_arm_parms::apple_arm_isa::behavior_amx_fma(uint8_t op, uint8_t opt, uint8_t rm, uint8_t subop, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_add_imm() apple_arm_parms::apple_arm_isa::behavior_add_imm(uint8_t op, uint16_t imm14, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_subs_imm() apple_arm_parms::apple_arm_isa::behavior_subs_imm(uint8_t op, uint16_t imm14, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_movz_imm() apple_arm_parms::apple_arm_isa::behavior_movz_imm(uint8_t op, uint16_t imm14, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_ldr_x() apple_arm_parms::apple_arm_isa::behavior_ldr_x(uint8_t op, int16_t disp, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_str_x() apple_arm_parms::apple_arm_isa::behavior_str_x(uint8_t op, int16_t disp, uint8_t rn, uint8_t rd)
#define AC_BEHAVIOR_pacia() apple_arm_parms::apple_arm_isa::behavior_pacia(uint8_t op, uint8_t pac_op, uint8_t rn, uint8_t subop, uint8_t rd)
#define AC_BEHAVIOR_autia() apple_arm_parms::apple_arm_isa::behavior_autia(uint8_t op, uint8_t pac_op, uint8_t rn, uint8_t subop, uint8_t rd)
#define AC_BEHAVIOR_pacda() apple_arm_parms::apple_arm_isa::behavior_pacda(uint8_t op, uint8_t pac_op, uint8_t rn, uint8_t subop, uint8_t rd)
#define AC_BEHAVIOR_autda() apple_arm_parms::apple_arm_isa::behavior_autda(uint8_t op, uint8_t pac_op, uint8_t rn, uint8_t subop, uint8_t rd)
#define AC_BEHAVIOR_b_cond() apple_arm_parms::apple_arm_isa::behavior_b_cond(uint8_t op, int32_t imm19, uint8_t o, uint8_t cond)
#define AC_BEHAVIOR_b_imm() apple_arm_parms::apple_arm_isa::behavior_b_imm(uint8_t op, int32_t imm24)
#define AC_BEHAVIOR_bl_imm() apple_arm_parms::apple_arm_isa::behavior_bl_imm(uint8_t op, int32_t imm24)
#define AC_BEHAVIOR_halt() apple_arm_parms::apple_arm_isa::behavior_halt(uint8_t op, uint32_t pad)


#endif //_APPLE_ARM_BHV_MACROS_H

