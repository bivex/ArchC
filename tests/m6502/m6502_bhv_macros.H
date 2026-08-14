#ifndef _M6502_BHV_MACROS_H
#define _M6502_BHV_MACROS_H

typedef ac_memport<m6502_parms::ac_word, m6502_parms::ac_Hword> ac_memory;

#define ac_behavior(instr) AC_BEHAVIOR_##instr ()

#define AC_BEHAVIOR_instruction() m6502_parms::m6502_isa::_behavior_instruction(uint8_t op)

#define AC_BEHAVIOR_begin() m6502_parms::m6502_isa::_behavior_begin()
#define AC_BEHAVIOR_end() m6502_parms::m6502_isa::_behavior_end()

#define AC_BEHAVIOR_Type_Imp() m6502_parms::m6502_isa::_behavior_m6502_Type_Imp(uint8_t op)
#define AC_BEHAVIOR_Type_Imm() m6502_parms::m6502_isa::_behavior_m6502_Type_Imm(uint8_t op, uint8_t imm)
#define AC_BEHAVIOR_Type_ZP() m6502_parms::m6502_isa::_behavior_m6502_Type_ZP(uint8_t op, uint8_t zp)
#define AC_BEHAVIOR_Type_Abs() m6502_parms::m6502_isa::_behavior_m6502_Type_Abs(uint8_t op, uint16_t addr)
#define AC_BEHAVIOR_Type_Branch() m6502_parms::m6502_isa::_behavior_m6502_Type_Branch(uint8_t op, int8_t bdisp)

#define AC_BEHAVIOR_tax() m6502_parms::m6502_isa::behavior_tax(uint8_t op)
#define AC_BEHAVIOR_txa() m6502_parms::m6502_isa::behavior_txa(uint8_t op)
#define AC_BEHAVIOR_tay() m6502_parms::m6502_isa::behavior_tay(uint8_t op)
#define AC_BEHAVIOR_tya() m6502_parms::m6502_isa::behavior_tya(uint8_t op)
#define AC_BEHAVIOR_inx() m6502_parms::m6502_isa::behavior_inx(uint8_t op)
#define AC_BEHAVIOR_dex() m6502_parms::m6502_isa::behavior_dex(uint8_t op)
#define AC_BEHAVIOR_iny() m6502_parms::m6502_isa::behavior_iny(uint8_t op)
#define AC_BEHAVIOR_dey() m6502_parms::m6502_isa::behavior_dey(uint8_t op)
#define AC_BEHAVIOR_clc() m6502_parms::m6502_isa::behavior_clc(uint8_t op)
#define AC_BEHAVIOR_sec() m6502_parms::m6502_isa::behavior_sec(uint8_t op)
#define AC_BEHAVIOR_nop_op() m6502_parms::m6502_isa::behavior_nop_op(uint8_t op)
#define AC_BEHAVIOR_brk_op() m6502_parms::m6502_isa::behavior_brk_op(uint8_t op)
#define AC_BEHAVIOR_lda_imm() m6502_parms::m6502_isa::behavior_lda_imm(uint8_t op, uint8_t imm)
#define AC_BEHAVIOR_ldx_imm() m6502_parms::m6502_isa::behavior_ldx_imm(uint8_t op, uint8_t imm)
#define AC_BEHAVIOR_ldy_imm() m6502_parms::m6502_isa::behavior_ldy_imm(uint8_t op, uint8_t imm)
#define AC_BEHAVIOR_adc_imm() m6502_parms::m6502_isa::behavior_adc_imm(uint8_t op, uint8_t imm)
#define AC_BEHAVIOR_sbc_imm() m6502_parms::m6502_isa::behavior_sbc_imm(uint8_t op, uint8_t imm)
#define AC_BEHAVIOR_and_imm() m6502_parms::m6502_isa::behavior_and_imm(uint8_t op, uint8_t imm)
#define AC_BEHAVIOR_ora_imm() m6502_parms::m6502_isa::behavior_ora_imm(uint8_t op, uint8_t imm)
#define AC_BEHAVIOR_eor_imm() m6502_parms::m6502_isa::behavior_eor_imm(uint8_t op, uint8_t imm)
#define AC_BEHAVIOR_cmp_imm() m6502_parms::m6502_isa::behavior_cmp_imm(uint8_t op, uint8_t imm)
#define AC_BEHAVIOR_cpx_imm() m6502_parms::m6502_isa::behavior_cpx_imm(uint8_t op, uint8_t imm)
#define AC_BEHAVIOR_cpy_imm() m6502_parms::m6502_isa::behavior_cpy_imm(uint8_t op, uint8_t imm)
#define AC_BEHAVIOR_lda_zp() m6502_parms::m6502_isa::behavior_lda_zp(uint8_t op, uint8_t zp)
#define AC_BEHAVIOR_sta_zp() m6502_parms::m6502_isa::behavior_sta_zp(uint8_t op, uint8_t zp)
#define AC_BEHAVIOR_ldx_zp() m6502_parms::m6502_isa::behavior_ldx_zp(uint8_t op, uint8_t zp)
#define AC_BEHAVIOR_stx_zp() m6502_parms::m6502_isa::behavior_stx_zp(uint8_t op, uint8_t zp)
#define AC_BEHAVIOR_ldy_zp() m6502_parms::m6502_isa::behavior_ldy_zp(uint8_t op, uint8_t zp)
#define AC_BEHAVIOR_sty_zp() m6502_parms::m6502_isa::behavior_sty_zp(uint8_t op, uint8_t zp)
#define AC_BEHAVIOR_lda_abs() m6502_parms::m6502_isa::behavior_lda_abs(uint8_t op, uint16_t addr)
#define AC_BEHAVIOR_sta_abs() m6502_parms::m6502_isa::behavior_sta_abs(uint8_t op, uint16_t addr)
#define AC_BEHAVIOR_jmp_abs() m6502_parms::m6502_isa::behavior_jmp_abs(uint8_t op, uint16_t addr)
#define AC_BEHAVIOR_bne() m6502_parms::m6502_isa::behavior_bne(uint8_t op, int8_t bdisp)
#define AC_BEHAVIOR_beq() m6502_parms::m6502_isa::behavior_beq(uint8_t op, int8_t bdisp)
#define AC_BEHAVIOR_bcs() m6502_parms::m6502_isa::behavior_bcs(uint8_t op, int8_t bdisp)
#define AC_BEHAVIOR_bcc() m6502_parms::m6502_isa::behavior_bcc(uint8_t op, int8_t bdisp)
#define AC_BEHAVIOR_bmi() m6502_parms::m6502_isa::behavior_bmi(uint8_t op, int8_t bdisp)
#define AC_BEHAVIOR_bpl() m6502_parms::m6502_isa::behavior_bpl(uint8_t op, int8_t bdisp)


#endif //_M6502_BHV_MACROS_H

