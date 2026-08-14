#ifndef _M8051_BHV_MACROS_H
#define _M8051_BHV_MACROS_H

typedef ac_memport<m8051_parms::ac_word, m8051_parms::ac_Hword> ac_memory;

#define ac_behavior(instr) AC_BEHAVIOR_##instr ()

#define AC_BEHAVIOR_instruction() m8051_parms::m8051_isa::_behavior_instruction()

#define AC_BEHAVIOR_begin() m8051_parms::m8051_isa::_behavior_begin()
#define AC_BEHAVIOR_end() m8051_parms::m8051_isa::_behavior_end()

#define AC_BEHAVIOR_Type_1B() m8051_parms::m8051_isa::_behavior_m8051_Type_1B(uint8_t op1, uint8_t rn1)
#define AC_BEHAVIOR_Type_2B() m8051_parms::m8051_isa::_behavior_m8051_Type_2B(uint8_t op2, uint8_t imm2)
#define AC_BEHAVIOR_Type_2BR() m8051_parms::m8051_isa::_behavior_m8051_Type_2BR(uint8_t op3, uint8_t rn3, uint8_t imm3)
#define AC_BEHAVIOR_Type_REL() m8051_parms::m8051_isa::_behavior_m8051_Type_REL(uint8_t op4, int8_t offset4)
#define AC_BEHAVIOR_Type_DJNZ() m8051_parms::m8051_isa::_behavior_m8051_Type_DJNZ(uint8_t op5, uint8_t rn5, int8_t offset5)
#define AC_BEHAVIOR_Type_HLT() m8051_parms::m8051_isa::_behavior_m8051_Type_HLT(uint8_t op6)

#define AC_BEHAVIOR_add_a_rn() m8051_parms::m8051_isa::behavior_add_a_rn(uint8_t op1, uint8_t rn1)
#define AC_BEHAVIOR_sub_a_rn() m8051_parms::m8051_isa::behavior_sub_a_rn(uint8_t op1, uint8_t rn1)
#define AC_BEHAVIOR_anl_a_rn() m8051_parms::m8051_isa::behavior_anl_a_rn(uint8_t op1, uint8_t rn1)
#define AC_BEHAVIOR_orl_a_rn() m8051_parms::m8051_isa::behavior_orl_a_rn(uint8_t op1, uint8_t rn1)
#define AC_BEHAVIOR_xrl_a_rn() m8051_parms::m8051_isa::behavior_xrl_a_rn(uint8_t op1, uint8_t rn1)
#define AC_BEHAVIOR_mov_a_rn() m8051_parms::m8051_isa::behavior_mov_a_rn(uint8_t op1, uint8_t rn1)
#define AC_BEHAVIOR_mov_rn_a() m8051_parms::m8051_isa::behavior_mov_rn_a(uint8_t op1, uint8_t rn1)
#define AC_BEHAVIOR_inc_rn() m8051_parms::m8051_isa::behavior_inc_rn(uint8_t op1, uint8_t rn1)
#define AC_BEHAVIOR_dec_rn() m8051_parms::m8051_isa::behavior_dec_rn(uint8_t op1, uint8_t rn1)
#define AC_BEHAVIOR_mov_a_imm() m8051_parms::m8051_isa::behavior_mov_a_imm(uint8_t op2, uint8_t imm2)
#define AC_BEHAVIOR_add_a_imm() m8051_parms::m8051_isa::behavior_add_a_imm(uint8_t op2, uint8_t imm2)
#define AC_BEHAVIOR_mov_rn_imm() m8051_parms::m8051_isa::behavior_mov_rn_imm(uint8_t op3, uint8_t rn3, uint8_t imm3)
#define AC_BEHAVIOR_sjmp() m8051_parms::m8051_isa::behavior_sjmp(uint8_t op4, int8_t offset4)
#define AC_BEHAVIOR_jz() m8051_parms::m8051_isa::behavior_jz(uint8_t op4, int8_t offset4)
#define AC_BEHAVIOR_jnz() m8051_parms::m8051_isa::behavior_jnz(uint8_t op4, int8_t offset4)
#define AC_BEHAVIOR_djnz_rn() m8051_parms::m8051_isa::behavior_djnz_rn(uint8_t op5, uint8_t rn5, int8_t offset5)
#define AC_BEHAVIOR_halt() m8051_parms::m8051_isa::behavior_halt(uint8_t op6)


#endif //_M8051_BHV_MACROS_H

