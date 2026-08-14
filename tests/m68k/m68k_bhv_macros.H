#ifndef _M68K_BHV_MACROS_H
#define _M68K_BHV_MACROS_H

typedef ac_memport<m68k_parms::ac_word, m68k_parms::ac_Hword> ac_memory;

#define ac_behavior(instr) AC_BEHAVIOR_##instr ()

#define AC_BEHAVIOR_instruction() m68k_parms::m68k_isa::_behavior_instruction(uint8_t op)

#define AC_BEHAVIOR_begin() m68k_parms::m68k_isa::_behavior_begin()
#define AC_BEHAVIOR_end() m68k_parms::m68k_isa::_behavior_end()

#define AC_BEHAVIOR_Type_MoveQ() m68k_parms::m68k_isa::_behavior_m68k_Type_MoveQ(uint8_t op, uint8_t rd, uint8_t zero, int8_t data)
#define AC_BEHAVIOR_Type_ALU_Reg() m68k_parms::m68k_isa::_behavior_m68k_Type_ALU_Reg(uint8_t op, uint8_t rd, uint8_t opmode, uint8_t mode, uint8_t reg)
#define AC_BEHAVIOR_Type_Quick() m68k_parms::m68k_isa::_behavior_m68k_Type_Quick(uint8_t op, uint8_t qdata, uint8_t s, uint8_t size, uint8_t mode, uint8_t reg)
#define AC_BEHAVIOR_Type_Branch() m68k_parms::m68k_isa::_behavior_m68k_Type_Branch(uint8_t op, uint8_t cond, int8_t bdisp)
#define AC_BEHAVIOR_Type_Move_Mem() m68k_parms::m68k_isa::_behavior_m68k_Type_Move_Mem(uint8_t op, uint8_t size, uint8_t dst_reg, uint8_t dst_mode, uint8_t src_mode, uint8_t src_reg)
#define AC_BEHAVIOR_Type_Halt() m68k_parms::m68k_isa::_behavior_m68k_Type_Halt(uint16_t op)

#define AC_BEHAVIOR_moveq() m68k_parms::m68k_isa::behavior_moveq(uint8_t op, uint8_t rd, uint8_t zero, int8_t data)
#define AC_BEHAVIOR_add_l() m68k_parms::m68k_isa::behavior_add_l(uint8_t op, uint8_t rd, uint8_t opmode, uint8_t mode, uint8_t reg)
#define AC_BEHAVIOR_sub_l() m68k_parms::m68k_isa::behavior_sub_l(uint8_t op, uint8_t rd, uint8_t opmode, uint8_t mode, uint8_t reg)
#define AC_BEHAVIOR_mul_l() m68k_parms::m68k_isa::behavior_mul_l(uint8_t op, uint8_t rd, uint8_t opmode, uint8_t mode, uint8_t reg)
#define AC_BEHAVIOR_and_l() m68k_parms::m68k_isa::behavior_and_l(uint8_t op, uint8_t rd, uint8_t opmode, uint8_t mode, uint8_t reg)
#define AC_BEHAVIOR_or_l() m68k_parms::m68k_isa::behavior_or_l(uint8_t op, uint8_t rd, uint8_t opmode, uint8_t mode, uint8_t reg)
#define AC_BEHAVIOR_eor_l() m68k_parms::m68k_isa::behavior_eor_l(uint8_t op, uint8_t rd, uint8_t opmode, uint8_t mode, uint8_t reg)
#define AC_BEHAVIOR_addq_l() m68k_parms::m68k_isa::behavior_addq_l(uint8_t op, uint8_t qdata, uint8_t s, uint8_t size, uint8_t mode, uint8_t reg)
#define AC_BEHAVIOR_subq_l() m68k_parms::m68k_isa::behavior_subq_l(uint8_t op, uint8_t qdata, uint8_t s, uint8_t size, uint8_t mode, uint8_t reg)
#define AC_BEHAVIOR_bra() m68k_parms::m68k_isa::behavior_bra(uint8_t op, uint8_t cond, int8_t bdisp)
#define AC_BEHAVIOR_bne() m68k_parms::m68k_isa::behavior_bne(uint8_t op, uint8_t cond, int8_t bdisp)
#define AC_BEHAVIOR_beq() m68k_parms::m68k_isa::behavior_beq(uint8_t op, uint8_t cond, int8_t bdisp)
#define AC_BEHAVIOR_blt() m68k_parms::m68k_isa::behavior_blt(uint8_t op, uint8_t cond, int8_t bdisp)
#define AC_BEHAVIOR_bge() m68k_parms::m68k_isa::behavior_bge(uint8_t op, uint8_t cond, int8_t bdisp)
#define AC_BEHAVIOR_move_to_mem() m68k_parms::m68k_isa::behavior_move_to_mem(uint8_t op, uint8_t size, uint8_t dst_reg, uint8_t dst_mode, uint8_t src_mode, uint8_t src_reg)
#define AC_BEHAVIOR_move_from_mem() m68k_parms::m68k_isa::behavior_move_from_mem(uint8_t op, uint8_t size, uint8_t dst_reg, uint8_t dst_mode, uint8_t src_mode, uint8_t src_reg)
#define AC_BEHAVIOR_halt() m68k_parms::m68k_isa::behavior_halt(uint16_t op)


#endif //_M68K_BHV_MACROS_H

