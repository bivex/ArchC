#ifndef _C6X_BHV_MACROS_H
#define _C6X_BHV_MACROS_H

typedef ac_memport<c6x_parms::ac_word, c6x_parms::ac_Hword> ac_memory;

#define ac_behavior(instr) AC_BEHAVIOR_##instr ()

#define AC_BEHAVIOR_instruction() c6x_parms::c6x_isa::_behavior_instruction(uint8_t op)

#define AC_BEHAVIOR_begin() c6x_parms::c6x_isa::_behavior_begin()
#define AC_BEHAVIOR_end() c6x_parms::c6x_isa::_behavior_end()

#define AC_BEHAVIOR_Type_VLIW_Reg() c6x_parms::c6x_isa::_behavior_c6x_Type_VLIW_Reg(uint8_t creg, uint8_t z, uint8_t op, uint8_t src2, uint8_t src1, uint8_t dst, uint8_t pad, uint8_t s, uint8_t p)
#define AC_BEHAVIOR_Type_VLIW_Imm() c6x_parms::c6x_isa::_behavior_c6x_Type_VLIW_Imm(uint8_t creg, uint8_t z, uint8_t op, int16_t cst, uint8_t dst)
#define AC_BEHAVIOR_Type_Branch() c6x_parms::c6x_isa::_behavior_c6x_Type_Branch(uint8_t creg, uint8_t z, uint8_t op, int32_t disp)
#define AC_BEHAVIOR_Type_Halt() c6x_parms::c6x_isa::_behavior_c6x_Type_Halt(uint8_t op, uint32_t pad)

#define AC_BEHAVIOR_add_reg() c6x_parms::c6x_isa::behavior_add_reg(uint8_t creg, uint8_t z, uint8_t op, uint8_t src2, uint8_t src1, uint8_t dst, uint8_t pad, uint8_t s, uint8_t p)
#define AC_BEHAVIOR_sub_reg() c6x_parms::c6x_isa::behavior_sub_reg(uint8_t creg, uint8_t z, uint8_t op, uint8_t src2, uint8_t src1, uint8_t dst, uint8_t pad, uint8_t s, uint8_t p)
#define AC_BEHAVIOR_and_reg() c6x_parms::c6x_isa::behavior_and_reg(uint8_t creg, uint8_t z, uint8_t op, uint8_t src2, uint8_t src1, uint8_t dst, uint8_t pad, uint8_t s, uint8_t p)
#define AC_BEHAVIOR_or_reg() c6x_parms::c6x_isa::behavior_or_reg(uint8_t creg, uint8_t z, uint8_t op, uint8_t src2, uint8_t src1, uint8_t dst, uint8_t pad, uint8_t s, uint8_t p)
#define AC_BEHAVIOR_xor_reg() c6x_parms::c6x_isa::behavior_xor_reg(uint8_t creg, uint8_t z, uint8_t op, uint8_t src2, uint8_t src1, uint8_t dst, uint8_t pad, uint8_t s, uint8_t p)
#define AC_BEHAVIOR_sadd_reg() c6x_parms::c6x_isa::behavior_sadd_reg(uint8_t creg, uint8_t z, uint8_t op, uint8_t src2, uint8_t src1, uint8_t dst, uint8_t pad, uint8_t s, uint8_t p)
#define AC_BEHAVIOR_ssub_reg() c6x_parms::c6x_isa::behavior_ssub_reg(uint8_t creg, uint8_t z, uint8_t op, uint8_t src2, uint8_t src1, uint8_t dst, uint8_t pad, uint8_t s, uint8_t p)
#define AC_BEHAVIOR_mpy_reg() c6x_parms::c6x_isa::behavior_mpy_reg(uint8_t creg, uint8_t z, uint8_t op, uint8_t src2, uint8_t src1, uint8_t dst, uint8_t pad, uint8_t s, uint8_t p)
#define AC_BEHAVIOR_smpy_reg() c6x_parms::c6x_isa::behavior_smpy_reg(uint8_t creg, uint8_t z, uint8_t op, uint8_t src2, uint8_t src1, uint8_t dst, uint8_t pad, uint8_t s, uint8_t p)
#define AC_BEHAVIOR_ldw() c6x_parms::c6x_isa::behavior_ldw(uint8_t creg, uint8_t z, uint8_t op, uint8_t src2, uint8_t src1, uint8_t dst, uint8_t pad, uint8_t s, uint8_t p)
#define AC_BEHAVIOR_stw() c6x_parms::c6x_isa::behavior_stw(uint8_t creg, uint8_t z, uint8_t op, uint8_t src2, uint8_t src1, uint8_t dst, uint8_t pad, uint8_t s, uint8_t p)
#define AC_BEHAVIOR_mvkl() c6x_parms::c6x_isa::behavior_mvkl(uint8_t creg, uint8_t z, uint8_t op, int16_t cst, uint8_t dst)
#define AC_BEHAVIOR_mvkh() c6x_parms::c6x_isa::behavior_mvkh(uint8_t creg, uint8_t z, uint8_t op, int16_t cst, uint8_t dst)
#define AC_BEHAVIOR_add_imm() c6x_parms::c6x_isa::behavior_add_imm(uint8_t creg, uint8_t z, uint8_t op, int16_t cst, uint8_t dst)
#define AC_BEHAVIOR_sub_imm() c6x_parms::c6x_isa::behavior_sub_imm(uint8_t creg, uint8_t z, uint8_t op, int16_t cst, uint8_t dst)
#define AC_BEHAVIOR_b_disp() c6x_parms::c6x_isa::behavior_b_disp(uint8_t creg, uint8_t z, uint8_t op, int32_t disp)
#define AC_BEHAVIOR_halt() c6x_parms::c6x_isa::behavior_halt(uint8_t op, uint32_t pad)


#endif //_C6X_BHV_MACROS_H

