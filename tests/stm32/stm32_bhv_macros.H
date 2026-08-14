#ifndef _STM32_BHV_MACROS_H
#define _STM32_BHV_MACROS_H

typedef ac_memport<stm32_parms::ac_word, stm32_parms::ac_Hword> ac_memory;

#define ac_behavior(instr) AC_BEHAVIOR_##instr ()

#define AC_BEHAVIOR_instruction() stm32_parms::stm32_isa::_behavior_instruction(uint8_t op)

#define AC_BEHAVIOR_begin() stm32_parms::stm32_isa::_behavior_begin()
#define AC_BEHAVIOR_end() stm32_parms::stm32_isa::_behavior_end()

#define AC_BEHAVIOR_Type_Imm8() stm32_parms::stm32_isa::_behavior_stm32_Type_Imm8(uint8_t op, uint8_t subop1, uint8_t rd1, uint8_t imm1)
#define AC_BEHAVIOR_Type_Alu3() stm32_parms::stm32_isa::_behavior_stm32_Type_Alu3(uint8_t op, uint8_t subop2, uint8_t rm2, uint8_t rn2, uint8_t rd2)
#define AC_BEHAVIOR_Type_Alu2() stm32_parms::stm32_isa::_behavior_stm32_Type_Alu2(uint8_t op, uint8_t subop3, uint8_t rm3, uint8_t rdn3)
#define AC_BEHAVIOR_Type_Mem() stm32_parms::stm32_isa::_behavior_stm32_Type_Mem(uint8_t op, uint8_t subop4, uint8_t imm4, uint8_t rn4, uint8_t rt4)
#define AC_BEHAVIOR_Type_BCond() stm32_parms::stm32_isa::_behavior_stm32_Type_BCond(uint8_t op, uint8_t cond5, int8_t bdisp5)
#define AC_BEHAVIOR_Type_BUnc() stm32_parms::stm32_isa::_behavior_stm32_Type_BUnc(uint8_t op, uint8_t subop6, int16_t bdisp6)
#define AC_BEHAVIOR_Type_Halt() stm32_parms::stm32_isa::_behavior_stm32_Type_Halt(uint8_t op, uint16_t pad7)

#define AC_BEHAVIOR_movs_imm() stm32_parms::stm32_isa::behavior_movs_imm(uint8_t op, uint8_t subop1, uint8_t rd1, uint8_t imm1)
#define AC_BEHAVIOR_adds_imm() stm32_parms::stm32_isa::behavior_adds_imm(uint8_t op, uint8_t subop1, uint8_t rd1, uint8_t imm1)
#define AC_BEHAVIOR_subs_imm() stm32_parms::stm32_isa::behavior_subs_imm(uint8_t op, uint8_t subop1, uint8_t rd1, uint8_t imm1)
#define AC_BEHAVIOR_cmp_imm() stm32_parms::stm32_isa::behavior_cmp_imm(uint8_t op, uint8_t subop1, uint8_t rd1, uint8_t imm1)
#define AC_BEHAVIOR_adds_reg() stm32_parms::stm32_isa::behavior_adds_reg(uint8_t op, uint8_t subop2, uint8_t rm2, uint8_t rn2, uint8_t rd2)
#define AC_BEHAVIOR_subs_reg() stm32_parms::stm32_isa::behavior_subs_reg(uint8_t op, uint8_t subop2, uint8_t rm2, uint8_t rn2, uint8_t rd2)
#define AC_BEHAVIOR_ands_reg() stm32_parms::stm32_isa::behavior_ands_reg(uint8_t op, uint8_t subop3, uint8_t rm3, uint8_t rdn3)
#define AC_BEHAVIOR_eors_reg() stm32_parms::stm32_isa::behavior_eors_reg(uint8_t op, uint8_t subop3, uint8_t rm3, uint8_t rdn3)
#define AC_BEHAVIOR_orrs_reg() stm32_parms::stm32_isa::behavior_orrs_reg(uint8_t op, uint8_t subop3, uint8_t rm3, uint8_t rdn3)
#define AC_BEHAVIOR_muls_reg() stm32_parms::stm32_isa::behavior_muls_reg(uint8_t op, uint8_t subop3, uint8_t rm3, uint8_t rdn3)
#define AC_BEHAVIOR_ldr_imm() stm32_parms::stm32_isa::behavior_ldr_imm(uint8_t op, uint8_t subop4, uint8_t imm4, uint8_t rn4, uint8_t rt4)
#define AC_BEHAVIOR_str_imm() stm32_parms::stm32_isa::behavior_str_imm(uint8_t op, uint8_t subop4, uint8_t imm4, uint8_t rn4, uint8_t rt4)
#define AC_BEHAVIOR_beq() stm32_parms::stm32_isa::behavior_beq(uint8_t op, uint8_t cond5, int8_t bdisp5)
#define AC_BEHAVIOR_bne() stm32_parms::stm32_isa::behavior_bne(uint8_t op, uint8_t cond5, int8_t bdisp5)
#define AC_BEHAVIOR_bcs() stm32_parms::stm32_isa::behavior_bcs(uint8_t op, uint8_t cond5, int8_t bdisp5)
#define AC_BEHAVIOR_bcc() stm32_parms::stm32_isa::behavior_bcc(uint8_t op, uint8_t cond5, int8_t bdisp5)
#define AC_BEHAVIOR_bmi() stm32_parms::stm32_isa::behavior_bmi(uint8_t op, uint8_t cond5, int8_t bdisp5)
#define AC_BEHAVIOR_bpl() stm32_parms::stm32_isa::behavior_bpl(uint8_t op, uint8_t cond5, int8_t bdisp5)
#define AC_BEHAVIOR_bge() stm32_parms::stm32_isa::behavior_bge(uint8_t op, uint8_t cond5, int8_t bdisp5)
#define AC_BEHAVIOR_blt() stm32_parms::stm32_isa::behavior_blt(uint8_t op, uint8_t cond5, int8_t bdisp5)
#define AC_BEHAVIOR_b_unc() stm32_parms::stm32_isa::behavior_b_unc(uint8_t op, uint8_t subop6, int16_t bdisp6)
#define AC_BEHAVIOR_bkpt_halt() stm32_parms::stm32_isa::behavior_bkpt_halt(uint8_t op, uint16_t pad7)


#endif //_STM32_BHV_MACROS_H

