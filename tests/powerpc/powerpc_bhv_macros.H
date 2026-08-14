#ifndef _POWERPC_BHV_MACROS_H
#define _POWERPC_BHV_MACROS_H

typedef ac_memport<powerpc_parms::ac_word, powerpc_parms::ac_Hword> ac_memory;

#define ac_behavior(instr) AC_BEHAVIOR_##instr ()

#define AC_BEHAVIOR_instruction() powerpc_parms::powerpc_isa::_behavior_instruction(uint8_t op)

#define AC_BEHAVIOR_begin() powerpc_parms::powerpc_isa::_behavior_begin()
#define AC_BEHAVIOR_end() powerpc_parms::powerpc_isa::_behavior_end()

#define AC_BEHAVIOR_Type_XO() powerpc_parms::powerpc_isa::_behavior_powerpc_Type_XO(uint8_t op, uint8_t rt, uint8_t ra, uint8_t rb, uint8_t oe, uint16_t xo)
#define AC_BEHAVIOR_Type_D() powerpc_parms::powerpc_isa::_behavior_powerpc_Type_D(uint8_t op, uint8_t rt, uint8_t ra, int16_t d)
#define AC_BEHAVIOR_Type_B() powerpc_parms::powerpc_isa::_behavior_powerpc_Type_B(uint8_t op, uint8_t bo, uint8_t bi, int16_t bd, uint8_t aa, uint8_t lk)
#define AC_BEHAVIOR_Type_I() powerpc_parms::powerpc_isa::_behavior_powerpc_Type_I(uint8_t op, int32_t li, uint8_t aa, uint8_t lk)
#define AC_BEHAVIOR_Type_HLT() powerpc_parms::powerpc_isa::_behavior_powerpc_Type_HLT(uint8_t op, uint32_t imm)

#define AC_BEHAVIOR_add() powerpc_parms::powerpc_isa::behavior_add(uint8_t op, uint8_t rt, uint8_t ra, uint8_t rb, uint8_t oe, uint16_t xo)
#define AC_BEHAVIOR_subf() powerpc_parms::powerpc_isa::behavior_subf(uint8_t op, uint8_t rt, uint8_t ra, uint8_t rb, uint8_t oe, uint16_t xo)
#define AC_BEHAVIOR_and_op() powerpc_parms::powerpc_isa::behavior_and_op(uint8_t op, uint8_t rt, uint8_t ra, uint8_t rb, uint8_t oe, uint16_t xo)
#define AC_BEHAVIOR_or_op() powerpc_parms::powerpc_isa::behavior_or_op(uint8_t op, uint8_t rt, uint8_t ra, uint8_t rb, uint8_t oe, uint16_t xo)
#define AC_BEHAVIOR_xor_op() powerpc_parms::powerpc_isa::behavior_xor_op(uint8_t op, uint8_t rt, uint8_t ra, uint8_t rb, uint8_t oe, uint16_t xo)
#define AC_BEHAVIOR_slw() powerpc_parms::powerpc_isa::behavior_slw(uint8_t op, uint8_t rt, uint8_t ra, uint8_t rb, uint8_t oe, uint16_t xo)
#define AC_BEHAVIOR_srw() powerpc_parms::powerpc_isa::behavior_srw(uint8_t op, uint8_t rt, uint8_t ra, uint8_t rb, uint8_t oe, uint16_t xo)
#define AC_BEHAVIOR_sraw() powerpc_parms::powerpc_isa::behavior_sraw(uint8_t op, uint8_t rt, uint8_t ra, uint8_t rb, uint8_t oe, uint16_t xo)
#define AC_BEHAVIOR_addi() powerpc_parms::powerpc_isa::behavior_addi(uint8_t op, uint8_t rt, uint8_t ra, int16_t d)
#define AC_BEHAVIOR_addic_dot() powerpc_parms::powerpc_isa::behavior_addic_dot(uint8_t op, uint8_t rt, uint8_t ra, int16_t d)
#define AC_BEHAVIOR_addis() powerpc_parms::powerpc_isa::behavior_addis(uint8_t op, uint8_t rt, uint8_t ra, int16_t d)
#define AC_BEHAVIOR_ori() powerpc_parms::powerpc_isa::behavior_ori(uint8_t op, uint8_t rt, uint8_t ra, int16_t d)
#define AC_BEHAVIOR_andi_dot() powerpc_parms::powerpc_isa::behavior_andi_dot(uint8_t op, uint8_t rt, uint8_t ra, int16_t d)
#define AC_BEHAVIOR_lwz() powerpc_parms::powerpc_isa::behavior_lwz(uint8_t op, uint8_t rt, uint8_t ra, int16_t d)
#define AC_BEHAVIOR_stw() powerpc_parms::powerpc_isa::behavior_stw(uint8_t op, uint8_t rt, uint8_t ra, int16_t d)
#define AC_BEHAVIOR_bc() powerpc_parms::powerpc_isa::behavior_bc(uint8_t op, uint8_t bo, uint8_t bi, int16_t bd, uint8_t aa, uint8_t lk)
#define AC_BEHAVIOR_b() powerpc_parms::powerpc_isa::behavior_b(uint8_t op, int32_t li, uint8_t aa, uint8_t lk)
#define AC_BEHAVIOR_halt() powerpc_parms::powerpc_isa::behavior_halt(uint8_t op, uint32_t imm)


#endif //_POWERPC_BHV_MACROS_H

