#ifndef _BENCH32_BHV_MACROS_H
#define _BENCH32_BHV_MACROS_H

typedef ac_memport<bench32_parms::ac_word, bench32_parms::ac_Hword> ac_memory;

#define ac_behavior(instr) AC_BEHAVIOR_##instr ()

#define AC_BEHAVIOR_instruction() bench32_parms::bench32_isa::_behavior_instruction(uint8_t op)

#define AC_BEHAVIOR_begin() bench32_parms::bench32_isa::_behavior_begin()
#define AC_BEHAVIOR_end() bench32_parms::bench32_isa::_behavior_end()

#define AC_BEHAVIOR_Type_R() bench32_parms::bench32_isa::_behavior_bench32_Type_R(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_Type_I() bench32_parms::bench32_isa::_behavior_bench32_Type_I(uint8_t op, uint8_t rs, uint8_t rt, uint16_t imm)
#define AC_BEHAVIOR_Type_J() bench32_parms::bench32_isa::_behavior_bench32_Type_J(uint8_t op, uint32_t addr)

#define AC_BEHAVIOR_add() bench32_parms::bench32_isa::behavior_add(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_sub() bench32_parms::bench32_isa::behavior_sub(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_and_op() bench32_parms::bench32_isa::behavior_and_op(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_or_op() bench32_parms::bench32_isa::behavior_or_op(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_xor_op() bench32_parms::bench32_isa::behavior_xor_op(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_slt() bench32_parms::bench32_isa::behavior_slt(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_sll() bench32_parms::bench32_isa::behavior_sll(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_srl() bench32_parms::bench32_isa::behavior_srl(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_nop() bench32_parms::bench32_isa::behavior_nop(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_addi() bench32_parms::bench32_isa::behavior_addi(uint8_t op, uint8_t rs, uint8_t rt, uint16_t imm)
#define AC_BEHAVIOR_andi() bench32_parms::bench32_isa::behavior_andi(uint8_t op, uint8_t rs, uint8_t rt, uint16_t imm)
#define AC_BEHAVIOR_ori() bench32_parms::bench32_isa::behavior_ori(uint8_t op, uint8_t rs, uint8_t rt, uint16_t imm)
#define AC_BEHAVIOR_lw() bench32_parms::bench32_isa::behavior_lw(uint8_t op, uint8_t rs, uint8_t rt, uint16_t imm)
#define AC_BEHAVIOR_sw() bench32_parms::bench32_isa::behavior_sw(uint8_t op, uint8_t rs, uint8_t rt, uint16_t imm)
#define AC_BEHAVIOR_beq() bench32_parms::bench32_isa::behavior_beq(uint8_t op, uint8_t rs, uint8_t rt, uint16_t imm)
#define AC_BEHAVIOR_bne() bench32_parms::bench32_isa::behavior_bne(uint8_t op, uint8_t rs, uint8_t rt, uint16_t imm)
#define AC_BEHAVIOR_j() bench32_parms::bench32_isa::behavior_j(uint8_t op, uint32_t addr)
#define AC_BEHAVIOR_halt() bench32_parms::bench32_isa::behavior_halt(uint8_t op, uint32_t addr)


#endif //_BENCH32_BHV_MACROS_H

