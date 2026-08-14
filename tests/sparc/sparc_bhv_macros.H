#ifndef _SPARC_BHV_MACROS_H
#define _SPARC_BHV_MACROS_H

typedef ac_memport<sparc_parms::ac_word, sparc_parms::ac_Hword> ac_memory;

#define ac_behavior(instr) AC_BEHAVIOR_##instr ()

#define AC_BEHAVIOR_instruction() sparc_parms::sparc_isa::_behavior_instruction(uint8_t op)

#define AC_BEHAVIOR_begin() sparc_parms::sparc_isa::_behavior_begin()
#define AC_BEHAVIOR_end() sparc_parms::sparc_isa::_behavior_end()

#define AC_BEHAVIOR_Type_F2() sparc_parms::sparc_isa::_behavior_sparc_Type_F2(uint8_t op, uint8_t rd, uint8_t op2, uint32_t imm22)
#define AC_BEHAVIOR_Type_F3R() sparc_parms::sparc_isa::_behavior_sparc_Type_F3R(uint8_t op, uint8_t rd, uint8_t op3, uint8_t rs1, uint8_t i, uint8_t asi, uint8_t rs2)
#define AC_BEHAVIOR_Type_F3I() sparc_parms::sparc_isa::_behavior_sparc_Type_F3I(uint8_t op, uint8_t rd, uint8_t op3, uint8_t rs1, uint8_t i, int16_t simm13)
#define AC_BEHAVIOR_Type_BR() sparc_parms::sparc_isa::_behavior_sparc_Type_BR(uint8_t op, uint8_t a, uint8_t cond, uint8_t op2, int32_t disp22)
#define AC_BEHAVIOR_Type_HLT() sparc_parms::sparc_isa::_behavior_sparc_Type_HLT(uint8_t op, uint8_t rd, uint8_t op3, uint32_t imm)

#define AC_BEHAVIOR_add_r() sparc_parms::sparc_isa::behavior_add_r(uint8_t op, uint8_t rd, uint8_t op3, uint8_t rs1, uint8_t i, uint8_t asi, uint8_t rs2)
#define AC_BEHAVIOR_sub_r() sparc_parms::sparc_isa::behavior_sub_r(uint8_t op, uint8_t rd, uint8_t op3, uint8_t rs1, uint8_t i, uint8_t asi, uint8_t rs2)
#define AC_BEHAVIOR_and_r() sparc_parms::sparc_isa::behavior_and_r(uint8_t op, uint8_t rd, uint8_t op3, uint8_t rs1, uint8_t i, uint8_t asi, uint8_t rs2)
#define AC_BEHAVIOR_or_r() sparc_parms::sparc_isa::behavior_or_r(uint8_t op, uint8_t rd, uint8_t op3, uint8_t rs1, uint8_t i, uint8_t asi, uint8_t rs2)
#define AC_BEHAVIOR_xor_r() sparc_parms::sparc_isa::behavior_xor_r(uint8_t op, uint8_t rd, uint8_t op3, uint8_t rs1, uint8_t i, uint8_t asi, uint8_t rs2)
#define AC_BEHAVIOR_sll_r() sparc_parms::sparc_isa::behavior_sll_r(uint8_t op, uint8_t rd, uint8_t op3, uint8_t rs1, uint8_t i, uint8_t asi, uint8_t rs2)
#define AC_BEHAVIOR_srl_r() sparc_parms::sparc_isa::behavior_srl_r(uint8_t op, uint8_t rd, uint8_t op3, uint8_t rs1, uint8_t i, uint8_t asi, uint8_t rs2)
#define AC_BEHAVIOR_sra_r() sparc_parms::sparc_isa::behavior_sra_r(uint8_t op, uint8_t rd, uint8_t op3, uint8_t rs1, uint8_t i, uint8_t asi, uint8_t rs2)
#define AC_BEHAVIOR_add_i() sparc_parms::sparc_isa::behavior_add_i(uint8_t op, uint8_t rd, uint8_t op3, uint8_t rs1, uint8_t i, int16_t simm13)
#define AC_BEHAVIOR_sub_i() sparc_parms::sparc_isa::behavior_sub_i(uint8_t op, uint8_t rd, uint8_t op3, uint8_t rs1, uint8_t i, int16_t simm13)
#define AC_BEHAVIOR_and_i() sparc_parms::sparc_isa::behavior_and_i(uint8_t op, uint8_t rd, uint8_t op3, uint8_t rs1, uint8_t i, int16_t simm13)
#define AC_BEHAVIOR_or_i() sparc_parms::sparc_isa::behavior_or_i(uint8_t op, uint8_t rd, uint8_t op3, uint8_t rs1, uint8_t i, int16_t simm13)
#define AC_BEHAVIOR_xor_i() sparc_parms::sparc_isa::behavior_xor_i(uint8_t op, uint8_t rd, uint8_t op3, uint8_t rs1, uint8_t i, int16_t simm13)
#define AC_BEHAVIOR_sll_i() sparc_parms::sparc_isa::behavior_sll_i(uint8_t op, uint8_t rd, uint8_t op3, uint8_t rs1, uint8_t i, int16_t simm13)
#define AC_BEHAVIOR_srl_i() sparc_parms::sparc_isa::behavior_srl_i(uint8_t op, uint8_t rd, uint8_t op3, uint8_t rs1, uint8_t i, int16_t simm13)
#define AC_BEHAVIOR_sra_i() sparc_parms::sparc_isa::behavior_sra_i(uint8_t op, uint8_t rd, uint8_t op3, uint8_t rs1, uint8_t i, int16_t simm13)
#define AC_BEHAVIOR_ld_i() sparc_parms::sparc_isa::behavior_ld_i(uint8_t op, uint8_t rd, uint8_t op3, uint8_t rs1, uint8_t i, int16_t simm13)
#define AC_BEHAVIOR_st_i() sparc_parms::sparc_isa::behavior_st_i(uint8_t op, uint8_t rd, uint8_t op3, uint8_t rs1, uint8_t i, int16_t simm13)
#define AC_BEHAVIOR_sethi() sparc_parms::sparc_isa::behavior_sethi(uint8_t op, uint8_t rd, uint8_t op2, uint32_t imm22)
#define AC_BEHAVIOR_be() sparc_parms::sparc_isa::behavior_be(uint8_t op, uint8_t a, uint8_t cond, uint8_t op2, int32_t disp22)
#define AC_BEHAVIOR_bne() sparc_parms::sparc_isa::behavior_bne(uint8_t op, uint8_t a, uint8_t cond, uint8_t op2, int32_t disp22)
#define AC_BEHAVIOR_ba() sparc_parms::sparc_isa::behavior_ba(uint8_t op, uint8_t a, uint8_t cond, uint8_t op2, int32_t disp22)
#define AC_BEHAVIOR_halt() sparc_parms::sparc_isa::behavior_halt(uint8_t op, uint8_t rd, uint8_t op3, uint32_t imm)


#endif //_SPARC_BHV_MACROS_H

