#ifndef _NRF52_BHV_MACROS_H
#define _NRF52_BHV_MACROS_H

typedef ac_memport<nrf52_parms::ac_word, nrf52_parms::ac_Hword> ac_memory;

#define ac_behavior(instr) AC_BEHAVIOR_##instr ()

#define AC_BEHAVIOR_instruction() nrf52_parms::nrf52_isa::_behavior_instruction(uint8_t op)

#define AC_BEHAVIOR_begin() nrf52_parms::nrf52_isa::_behavior_begin()
#define AC_BEHAVIOR_end() nrf52_parms::nrf52_isa::_behavior_end()

#define AC_BEHAVIOR_Type_Imm8() nrf52_parms::nrf52_isa::_behavior_nrf52_Type_Imm8(uint8_t op, uint8_t subop1, uint8_t rd1, uint8_t imm1)
#define AC_BEHAVIOR_Type_Alu3() nrf52_parms::nrf52_isa::_behavior_nrf52_Type_Alu3(uint8_t op, uint8_t subop2, uint8_t rm2, uint8_t rn2, uint8_t rd2)
#define AC_BEHAVIOR_Type_Alu2() nrf52_parms::nrf52_isa::_behavior_nrf52_Type_Alu2(uint8_t op, uint8_t subop3, uint8_t rm3, uint8_t rdn3)
#define AC_BEHAVIOR_Type_Mem() nrf52_parms::nrf52_isa::_behavior_nrf52_Type_Mem(uint8_t op, uint8_t subop4, uint8_t imm4, uint8_t rn4, uint8_t rt4)
#define AC_BEHAVIOR_Type_BCond() nrf52_parms::nrf52_isa::_behavior_nrf52_Type_BCond(uint8_t op, uint8_t cond5, int8_t bdisp5)
#define AC_BEHAVIOR_Type_BUnc() nrf52_parms::nrf52_isa::_behavior_nrf52_Type_BUnc(uint8_t op, int16_t bdisp6)
#define AC_BEHAVIOR_Type_Special() nrf52_parms::nrf52_isa::_behavior_nrf52_Type_Special(uint8_t op, uint8_t subop7, uint8_t imm7)

#define AC_BEHAVIOR_movs_imm() nrf52_parms::nrf52_isa::behavior_movs_imm(uint8_t op, uint8_t subop1, uint8_t rd1, uint8_t imm1)
#define AC_BEHAVIOR_cmp_imm() nrf52_parms::nrf52_isa::behavior_cmp_imm(uint8_t op, uint8_t subop1, uint8_t rd1, uint8_t imm1)
#define AC_BEHAVIOR_adds_imm() nrf52_parms::nrf52_isa::behavior_adds_imm(uint8_t op, uint8_t subop1, uint8_t rd1, uint8_t imm1)
#define AC_BEHAVIOR_subs_imm() nrf52_parms::nrf52_isa::behavior_subs_imm(uint8_t op, uint8_t subop1, uint8_t rd1, uint8_t imm1)
#define AC_BEHAVIOR_adds_reg() nrf52_parms::nrf52_isa::behavior_adds_reg(uint8_t op, uint8_t subop2, uint8_t rm2, uint8_t rn2, uint8_t rd2)
#define AC_BEHAVIOR_subs_reg() nrf52_parms::nrf52_isa::behavior_subs_reg(uint8_t op, uint8_t subop2, uint8_t rm2, uint8_t rn2, uint8_t rd2)
#define AC_BEHAVIOR_ands_reg() nrf52_parms::nrf52_isa::behavior_ands_reg(uint8_t op, uint8_t subop3, uint8_t rm3, uint8_t rdn3)
#define AC_BEHAVIOR_eors_reg() nrf52_parms::nrf52_isa::behavior_eors_reg(uint8_t op, uint8_t subop3, uint8_t rm3, uint8_t rdn3)
#define AC_BEHAVIOR_orrs_reg() nrf52_parms::nrf52_isa::behavior_orrs_reg(uint8_t op, uint8_t subop3, uint8_t rm3, uint8_t rdn3)
#define AC_BEHAVIOR_muls_reg() nrf52_parms::nrf52_isa::behavior_muls_reg(uint8_t op, uint8_t subop3, uint8_t rm3, uint8_t rdn3)
#define AC_BEHAVIOR_ldr_imm() nrf52_parms::nrf52_isa::behavior_ldr_imm(uint8_t op, uint8_t subop4, uint8_t imm4, uint8_t rn4, uint8_t rt4)
#define AC_BEHAVIOR_str_imm() nrf52_parms::nrf52_isa::behavior_str_imm(uint8_t op, uint8_t subop4, uint8_t imm4, uint8_t rn4, uint8_t rt4)
#define AC_BEHAVIOR_beq() nrf52_parms::nrf52_isa::behavior_beq(uint8_t op, uint8_t cond5, int8_t bdisp5)
#define AC_BEHAVIOR_bne() nrf52_parms::nrf52_isa::behavior_bne(uint8_t op, uint8_t cond5, int8_t bdisp5)
#define AC_BEHAVIOR_bcs() nrf52_parms::nrf52_isa::behavior_bcs(uint8_t op, uint8_t cond5, int8_t bdisp5)
#define AC_BEHAVIOR_bcc() nrf52_parms::nrf52_isa::behavior_bcc(uint8_t op, uint8_t cond5, int8_t bdisp5)
#define AC_BEHAVIOR_bmi() nrf52_parms::nrf52_isa::behavior_bmi(uint8_t op, uint8_t cond5, int8_t bdisp5)
#define AC_BEHAVIOR_bpl() nrf52_parms::nrf52_isa::behavior_bpl(uint8_t op, uint8_t cond5, int8_t bdisp5)
#define AC_BEHAVIOR_bge() nrf52_parms::nrf52_isa::behavior_bge(uint8_t op, uint8_t cond5, int8_t bdisp5)
#define AC_BEHAVIOR_blt() nrf52_parms::nrf52_isa::behavior_blt(uint8_t op, uint8_t cond5, int8_t bdisp5)
#define AC_BEHAVIOR_b_unc() nrf52_parms::nrf52_isa::behavior_b_unc(uint8_t op, int16_t bdisp6)
#define AC_BEHAVIOR_wfe() nrf52_parms::nrf52_isa::behavior_wfe(uint8_t op, uint8_t subop7, uint8_t imm7)
#define AC_BEHAVIOR_sev() nrf52_parms::nrf52_isa::behavior_sev(uint8_t op, uint8_t subop7, uint8_t imm7)
#define AC_BEHAVIOR_wfi() nrf52_parms::nrf52_isa::behavior_wfi(uint8_t op, uint8_t subop7, uint8_t imm7)
#define AC_BEHAVIOR_bkpt_halt() nrf52_parms::nrf52_isa::behavior_bkpt_halt(uint8_t op, uint8_t subop7, uint8_t imm7)


#endif //_NRF52_BHV_MACROS_H

