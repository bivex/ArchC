#ifndef _ALPHA_BHV_MACROS_H
#define _ALPHA_BHV_MACROS_H

typedef ac_memport<alpha_parms::ac_word, alpha_parms::ac_Hword> ac_memory;

#define ac_behavior(instr) AC_BEHAVIOR_##instr ()

#define AC_BEHAVIOR_instruction() alpha_parms::alpha_isa::_behavior_instruction(uint8_t op)

#define AC_BEHAVIOR_begin() alpha_parms::alpha_isa::_behavior_begin()
#define AC_BEHAVIOR_end() alpha_parms::alpha_isa::_behavior_end()

#define AC_BEHAVIOR_Type_OpReg() alpha_parms::alpha_isa::_behavior_alpha_Type_OpReg(uint8_t op, uint8_t ra, uint8_t rb, uint8_t sbz, uint8_t func, uint8_t rc)
#define AC_BEHAVIOR_Type_OpLit() alpha_parms::alpha_isa::_behavior_alpha_Type_OpLit(uint8_t op, uint8_t ra, uint8_t lit, uint8_t flag, uint8_t func, uint8_t rc)
#define AC_BEHAVIOR_Type_Memory() alpha_parms::alpha_isa::_behavior_alpha_Type_Memory(uint8_t op, uint8_t ra, uint8_t rb, int16_t disp)
#define AC_BEHAVIOR_Type_Branch() alpha_parms::alpha_isa::_behavior_alpha_Type_Branch(uint8_t op, uint8_t ra, int32_t bdisp)
#define AC_BEHAVIOR_Type_Halt() alpha_parms::alpha_isa::_behavior_alpha_Type_Halt(uint8_t op, uint32_t pad)

#define AC_BEHAVIOR_addq() alpha_parms::alpha_isa::behavior_addq(uint8_t op, uint8_t ra, uint8_t rb, uint8_t sbz, uint8_t func, uint8_t rc)
#define AC_BEHAVIOR_subq() alpha_parms::alpha_isa::behavior_subq(uint8_t op, uint8_t ra, uint8_t rb, uint8_t sbz, uint8_t func, uint8_t rc)
#define AC_BEHAVIOR_mulq() alpha_parms::alpha_isa::behavior_mulq(uint8_t op, uint8_t ra, uint8_t rb, uint8_t sbz, uint8_t func, uint8_t rc)
#define AC_BEHAVIOR_cmpeq() alpha_parms::alpha_isa::behavior_cmpeq(uint8_t op, uint8_t ra, uint8_t rb, uint8_t sbz, uint8_t func, uint8_t rc)
#define AC_BEHAVIOR_cmplt() alpha_parms::alpha_isa::behavior_cmplt(uint8_t op, uint8_t ra, uint8_t rb, uint8_t sbz, uint8_t func, uint8_t rc)
#define AC_BEHAVIOR_cmple() alpha_parms::alpha_isa::behavior_cmple(uint8_t op, uint8_t ra, uint8_t rb, uint8_t sbz, uint8_t func, uint8_t rc)
#define AC_BEHAVIOR_cmpult() alpha_parms::alpha_isa::behavior_cmpult(uint8_t op, uint8_t ra, uint8_t rb, uint8_t sbz, uint8_t func, uint8_t rc)
#define AC_BEHAVIOR_cmpule() alpha_parms::alpha_isa::behavior_cmpule(uint8_t op, uint8_t ra, uint8_t rb, uint8_t sbz, uint8_t func, uint8_t rc)
#define AC_BEHAVIOR_and_op() alpha_parms::alpha_isa::behavior_and_op(uint8_t op, uint8_t ra, uint8_t rb, uint8_t sbz, uint8_t func, uint8_t rc)
#define AC_BEHAVIOR_bis_op() alpha_parms::alpha_isa::behavior_bis_op(uint8_t op, uint8_t ra, uint8_t rb, uint8_t sbz, uint8_t func, uint8_t rc)
#define AC_BEHAVIOR_xor_op() alpha_parms::alpha_isa::behavior_xor_op(uint8_t op, uint8_t ra, uint8_t rb, uint8_t sbz, uint8_t func, uint8_t rc)
#define AC_BEHAVIOR_bic_op() alpha_parms::alpha_isa::behavior_bic_op(uint8_t op, uint8_t ra, uint8_t rb, uint8_t sbz, uint8_t func, uint8_t rc)
#define AC_BEHAVIOR_cmoveq() alpha_parms::alpha_isa::behavior_cmoveq(uint8_t op, uint8_t ra, uint8_t rb, uint8_t sbz, uint8_t func, uint8_t rc)
#define AC_BEHAVIOR_cmovne() alpha_parms::alpha_isa::behavior_cmovne(uint8_t op, uint8_t ra, uint8_t rb, uint8_t sbz, uint8_t func, uint8_t rc)
#define AC_BEHAVIOR_addq_i() alpha_parms::alpha_isa::behavior_addq_i(uint8_t op, uint8_t ra, uint8_t lit, uint8_t flag, uint8_t func, uint8_t rc)
#define AC_BEHAVIOR_subq_i() alpha_parms::alpha_isa::behavior_subq_i(uint8_t op, uint8_t ra, uint8_t lit, uint8_t flag, uint8_t func, uint8_t rc)
#define AC_BEHAVIOR_lda() alpha_parms::alpha_isa::behavior_lda(uint8_t op, uint8_t ra, uint8_t rb, int16_t disp)
#define AC_BEHAVIOR_ldah() alpha_parms::alpha_isa::behavior_ldah(uint8_t op, uint8_t ra, uint8_t rb, int16_t disp)
#define AC_BEHAVIOR_ldq() alpha_parms::alpha_isa::behavior_ldq(uint8_t op, uint8_t ra, uint8_t rb, int16_t disp)
#define AC_BEHAVIOR_stq() alpha_parms::alpha_isa::behavior_stq(uint8_t op, uint8_t ra, uint8_t rb, int16_t disp)
#define AC_BEHAVIOR_ldl() alpha_parms::alpha_isa::behavior_ldl(uint8_t op, uint8_t ra, uint8_t rb, int16_t disp)
#define AC_BEHAVIOR_stl() alpha_parms::alpha_isa::behavior_stl(uint8_t op, uint8_t ra, uint8_t rb, int16_t disp)
#define AC_BEHAVIOR_br() alpha_parms::alpha_isa::behavior_br(uint8_t op, uint8_t ra, int32_t bdisp)
#define AC_BEHAVIOR_bne() alpha_parms::alpha_isa::behavior_bne(uint8_t op, uint8_t ra, int32_t bdisp)
#define AC_BEHAVIOR_beq() alpha_parms::alpha_isa::behavior_beq(uint8_t op, uint8_t ra, int32_t bdisp)
#define AC_BEHAVIOR_blt() alpha_parms::alpha_isa::behavior_blt(uint8_t op, uint8_t ra, int32_t bdisp)
#define AC_BEHAVIOR_bge() alpha_parms::alpha_isa::behavior_bge(uint8_t op, uint8_t ra, int32_t bdisp)
#define AC_BEHAVIOR_halt() alpha_parms::alpha_isa::behavior_halt(uint8_t op, uint32_t pad)


#endif //_ALPHA_BHV_MACROS_H

