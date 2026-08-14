#ifndef _ARM_BHV_MACROS_H
#define _ARM_BHV_MACROS_H

typedef ac_memport<arm_parms::ac_word, arm_parms::ac_Hword> ac_memory;

#define ac_behavior(instr) AC_BEHAVIOR_##instr ()

#define AC_BEHAVIOR_instruction() arm_parms::arm_isa::_behavior_instruction(uint8_t cond, uint8_t op)

#define AC_BEHAVIOR_begin() arm_parms::arm_isa::_behavior_begin()
#define AC_BEHAVIOR_end() arm_parms::arm_isa::_behavior_end()

#define AC_BEHAVIOR_Type_DP() arm_parms::arm_isa::_behavior_arm_Type_DP(uint8_t cond, uint8_t op, uint8_t cmd, uint8_t s, uint8_t rn, uint8_t rd, uint16_t op2)
#define AC_BEHAVIOR_Type_MEM() arm_parms::arm_isa::_behavior_arm_Type_MEM(uint8_t cond, uint8_t op, uint8_t i, uint8_t p, uint8_t u, uint8_t b, uint8_t w, uint8_t l, uint8_t rn, uint8_t rd, uint16_t offset)
#define AC_BEHAVIOR_Type_BR() arm_parms::arm_isa::_behavior_arm_Type_BR(uint8_t cond, uint8_t op, uint8_t l, int32_t offset)
#define AC_BEHAVIOR_Type_HLT() arm_parms::arm_isa::_behavior_arm_Type_HLT(uint8_t cond, uint8_t op, uint32_t imm)

#define AC_BEHAVIOR_add() arm_parms::arm_isa::behavior_add(uint8_t cond, uint8_t op, uint8_t cmd, uint8_t s, uint8_t rn, uint8_t rd, uint16_t op2)
#define AC_BEHAVIOR_sub() arm_parms::arm_isa::behavior_sub(uint8_t cond, uint8_t op, uint8_t cmd, uint8_t s, uint8_t rn, uint8_t rd, uint16_t op2)
#define AC_BEHAVIOR_mov() arm_parms::arm_isa::behavior_mov(uint8_t cond, uint8_t op, uint8_t cmd, uint8_t s, uint8_t rn, uint8_t rd, uint16_t op2)
#define AC_BEHAVIOR_cmp_op() arm_parms::arm_isa::behavior_cmp_op(uint8_t cond, uint8_t op, uint8_t cmd, uint8_t s, uint8_t rn, uint8_t rd, uint16_t op2)
#define AC_BEHAVIOR_and_op() arm_parms::arm_isa::behavior_and_op(uint8_t cond, uint8_t op, uint8_t cmd, uint8_t s, uint8_t rn, uint8_t rd, uint16_t op2)
#define AC_BEHAVIOR_orr() arm_parms::arm_isa::behavior_orr(uint8_t cond, uint8_t op, uint8_t cmd, uint8_t s, uint8_t rn, uint8_t rd, uint16_t op2)
#define AC_BEHAVIOR_eor() arm_parms::arm_isa::behavior_eor(uint8_t cond, uint8_t op, uint8_t cmd, uint8_t s, uint8_t rn, uint8_t rd, uint16_t op2)
#define AC_BEHAVIOR_ldr() arm_parms::arm_isa::behavior_ldr(uint8_t cond, uint8_t op, uint8_t i, uint8_t p, uint8_t u, uint8_t b, uint8_t w, uint8_t l, uint8_t rn, uint8_t rd, uint16_t offset)
#define AC_BEHAVIOR_str() arm_parms::arm_isa::behavior_str(uint8_t cond, uint8_t op, uint8_t i, uint8_t p, uint8_t u, uint8_t b, uint8_t w, uint8_t l, uint8_t rn, uint8_t rd, uint16_t offset)
#define AC_BEHAVIOR_b() arm_parms::arm_isa::behavior_b(uint8_t cond, uint8_t op, uint8_t l, int32_t offset)
#define AC_BEHAVIOR_bl() arm_parms::arm_isa::behavior_bl(uint8_t cond, uint8_t op, uint8_t l, int32_t offset)
#define AC_BEHAVIOR_hlt() arm_parms::arm_isa::behavior_hlt(uint8_t cond, uint8_t op, uint32_t imm)


#endif //_ARM_BHV_MACROS_H

