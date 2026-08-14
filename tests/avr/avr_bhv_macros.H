#ifndef _AVR_BHV_MACROS_H
#define _AVR_BHV_MACROS_H

typedef ac_memport<avr_parms::ac_word, avr_parms::ac_Hword> ac_memory;

#define ac_behavior(instr) AC_BEHAVIOR_##instr ()

#define AC_BEHAVIOR_instruction() avr_parms::avr_isa::_behavior_instruction(uint8_t op)

#define AC_BEHAVIOR_begin() avr_parms::avr_isa::_behavior_begin()
#define AC_BEHAVIOR_end() avr_parms::avr_isa::_behavior_end()

#define AC_BEHAVIOR_Type_2R() avr_parms::avr_isa::_behavior_avr_Type_2R(uint8_t op, uint8_t rd, uint8_t rr)
#define AC_BEHAVIOR_Type_1R() avr_parms::avr_isa::_behavior_avr_Type_1R(uint8_t op, uint8_t rd, uint8_t func)
#define AC_BEHAVIOR_Type_Imm() avr_parms::avr_isa::_behavior_avr_Type_Imm(uint8_t op, uint8_t k_hi, uint8_t rd_hi, uint8_t k_lo)
#define AC_BEHAVIOR_Type_Branch() avr_parms::avr_isa::_behavior_avr_Type_Branch(uint8_t op, int8_t k_br, uint8_t cond)
#define AC_BEHAVIOR_Type_RJMP() avr_parms::avr_isa::_behavior_avr_Type_RJMP(uint8_t op, int16_t k_jmp)
#define AC_BEHAVIOR_Type_Ptr() avr_parms::avr_isa::_behavior_avr_Type_Ptr(uint8_t op, uint8_t rd, uint8_t ptr)
#define AC_BEHAVIOR_Type_Halt() avr_parms::avr_isa::_behavior_avr_Type_Halt(uint8_t op, uint8_t pad)

#define AC_BEHAVIOR_add_r() avr_parms::avr_isa::behavior_add_r(uint8_t op, uint8_t rd, uint8_t rr)
#define AC_BEHAVIOR_adc_r() avr_parms::avr_isa::behavior_adc_r(uint8_t op, uint8_t rd, uint8_t rr)
#define AC_BEHAVIOR_sub_r() avr_parms::avr_isa::behavior_sub_r(uint8_t op, uint8_t rd, uint8_t rr)
#define AC_BEHAVIOR_sbc_r() avr_parms::avr_isa::behavior_sbc_r(uint8_t op, uint8_t rd, uint8_t rr)
#define AC_BEHAVIOR_and_r() avr_parms::avr_isa::behavior_and_r(uint8_t op, uint8_t rd, uint8_t rr)
#define AC_BEHAVIOR_or_r() avr_parms::avr_isa::behavior_or_r(uint8_t op, uint8_t rd, uint8_t rr)
#define AC_BEHAVIOR_eor_r() avr_parms::avr_isa::behavior_eor_r(uint8_t op, uint8_t rd, uint8_t rr)
#define AC_BEHAVIOR_cp_r() avr_parms::avr_isa::behavior_cp_r(uint8_t op, uint8_t rd, uint8_t rr)
#define AC_BEHAVIOR_mov_r() avr_parms::avr_isa::behavior_mov_r(uint8_t op, uint8_t rd, uint8_t rr)
#define AC_BEHAVIOR_inc_r() avr_parms::avr_isa::behavior_inc_r(uint8_t op, uint8_t rd, uint8_t func)
#define AC_BEHAVIOR_dec_r() avr_parms::avr_isa::behavior_dec_r(uint8_t op, uint8_t rd, uint8_t func)
#define AC_BEHAVIOR_clr_r() avr_parms::avr_isa::behavior_clr_r(uint8_t op, uint8_t rd, uint8_t func)
#define AC_BEHAVIOR_ldi_i() avr_parms::avr_isa::behavior_ldi_i(uint8_t op, uint8_t k_hi, uint8_t rd_hi, uint8_t k_lo)
#define AC_BEHAVIOR_subi_i() avr_parms::avr_isa::behavior_subi_i(uint8_t op, uint8_t k_hi, uint8_t rd_hi, uint8_t k_lo)
#define AC_BEHAVIOR_andi_i() avr_parms::avr_isa::behavior_andi_i(uint8_t op, uint8_t k_hi, uint8_t rd_hi, uint8_t k_lo)
#define AC_BEHAVIOR_ori_i() avr_parms::avr_isa::behavior_ori_i(uint8_t op, uint8_t k_hi, uint8_t rd_hi, uint8_t k_lo)
#define AC_BEHAVIOR_cpi_i() avr_parms::avr_isa::behavior_cpi_i(uint8_t op, uint8_t k_hi, uint8_t rd_hi, uint8_t k_lo)
#define AC_BEHAVIOR_brne() avr_parms::avr_isa::behavior_brne(uint8_t op, int8_t k_br, uint8_t cond)
#define AC_BEHAVIOR_breq() avr_parms::avr_isa::behavior_breq(uint8_t op, int8_t k_br, uint8_t cond)
#define AC_BEHAVIOR_brcs() avr_parms::avr_isa::behavior_brcs(uint8_t op, int8_t k_br, uint8_t cond)
#define AC_BEHAVIOR_brcc() avr_parms::avr_isa::behavior_brcc(uint8_t op, int8_t k_br, uint8_t cond)
#define AC_BEHAVIOR_rjmp() avr_parms::avr_isa::behavior_rjmp(uint8_t op, int16_t k_jmp)
#define AC_BEHAVIOR_ld_x() avr_parms::avr_isa::behavior_ld_x(uint8_t op, uint8_t rd, uint8_t ptr)
#define AC_BEHAVIOR_st_x() avr_parms::avr_isa::behavior_st_x(uint8_t op, uint8_t rd, uint8_t ptr)
#define AC_BEHAVIOR_ld_z() avr_parms::avr_isa::behavior_ld_z(uint8_t op, uint8_t rd, uint8_t ptr)
#define AC_BEHAVIOR_st_z() avr_parms::avr_isa::behavior_st_z(uint8_t op, uint8_t rd, uint8_t ptr)
#define AC_BEHAVIOR_halt() avr_parms::avr_isa::behavior_halt(uint8_t op, uint8_t pad)


#endif //_AVR_BHV_MACROS_H

