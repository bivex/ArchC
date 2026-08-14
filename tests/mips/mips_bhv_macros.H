#ifndef _MIPS_BHV_MACROS_H
#define _MIPS_BHV_MACROS_H

typedef ac_memport<mips_parms::ac_word, mips_parms::ac_Hword> ac_memory;

#define ac_behavior(instr) AC_BEHAVIOR_##instr ()

#define AC_BEHAVIOR_instruction() mips_parms::mips_isa::_behavior_instruction(uint8_t op)

#define AC_BEHAVIOR_begin() mips_parms::mips_isa::_behavior_begin()
#define AC_BEHAVIOR_end() mips_parms::mips_isa::_behavior_end()

#define AC_BEHAVIOR_Type_R() mips_parms::mips_isa::_behavior_mips_Type_R(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_Type_I() mips_parms::mips_isa::_behavior_mips_Type_I(uint8_t op, uint8_t rs, uint8_t rt, int16_t imm)
#define AC_BEHAVIOR_Type_J() mips_parms::mips_isa::_behavior_mips_Type_J(uint8_t op, uint32_t addr)

#define AC_BEHAVIOR_add() mips_parms::mips_isa::behavior_add(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_sub() mips_parms::mips_isa::behavior_sub(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_and_op() mips_parms::mips_isa::behavior_and_op(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_or_op() mips_parms::mips_isa::behavior_or_op(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_xor_op() mips_parms::mips_isa::behavior_xor_op(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_nor_op() mips_parms::mips_isa::behavior_nor_op(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_slt() mips_parms::mips_isa::behavior_slt(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_sltu() mips_parms::mips_isa::behavior_sltu(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_sll() mips_parms::mips_isa::behavior_sll(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_srl() mips_parms::mips_isa::behavior_srl(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_sra() mips_parms::mips_isa::behavior_sra(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_mult() mips_parms::mips_isa::behavior_mult(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_div_op() mips_parms::mips_isa::behavior_div_op(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_mfhi() mips_parms::mips_isa::behavior_mfhi(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_mflo() mips_parms::mips_isa::behavior_mflo(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_jr() mips_parms::mips_isa::behavior_jr(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_nop() mips_parms::mips_isa::behavior_nop(uint8_t op, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t func)
#define AC_BEHAVIOR_addi() mips_parms::mips_isa::behavior_addi(uint8_t op, uint8_t rs, uint8_t rt, int16_t imm)
#define AC_BEHAVIOR_addiu() mips_parms::mips_isa::behavior_addiu(uint8_t op, uint8_t rs, uint8_t rt, int16_t imm)
#define AC_BEHAVIOR_andi() mips_parms::mips_isa::behavior_andi(uint8_t op, uint8_t rs, uint8_t rt, int16_t imm)
#define AC_BEHAVIOR_ori() mips_parms::mips_isa::behavior_ori(uint8_t op, uint8_t rs, uint8_t rt, int16_t imm)
#define AC_BEHAVIOR_xori() mips_parms::mips_isa::behavior_xori(uint8_t op, uint8_t rs, uint8_t rt, int16_t imm)
#define AC_BEHAVIOR_lui() mips_parms::mips_isa::behavior_lui(uint8_t op, uint8_t rs, uint8_t rt, int16_t imm)
#define AC_BEHAVIOR_lw() mips_parms::mips_isa::behavior_lw(uint8_t op, uint8_t rs, uint8_t rt, int16_t imm)
#define AC_BEHAVIOR_sw() mips_parms::mips_isa::behavior_sw(uint8_t op, uint8_t rs, uint8_t rt, int16_t imm)
#define AC_BEHAVIOR_beq() mips_parms::mips_isa::behavior_beq(uint8_t op, uint8_t rs, uint8_t rt, int16_t imm)
#define AC_BEHAVIOR_bne() mips_parms::mips_isa::behavior_bne(uint8_t op, uint8_t rs, uint8_t rt, int16_t imm)
#define AC_BEHAVIOR_blez() mips_parms::mips_isa::behavior_blez(uint8_t op, uint8_t rs, uint8_t rt, int16_t imm)
#define AC_BEHAVIOR_bgtz() mips_parms::mips_isa::behavior_bgtz(uint8_t op, uint8_t rs, uint8_t rt, int16_t imm)
#define AC_BEHAVIOR_j() mips_parms::mips_isa::behavior_j(uint8_t op, uint32_t addr)
#define AC_BEHAVIOR_jal() mips_parms::mips_isa::behavior_jal(uint8_t op, uint32_t addr)
#define AC_BEHAVIOR_halt() mips_parms::mips_isa::behavior_halt(uint8_t op, uint32_t addr)


#endif //_MIPS_BHV_MACROS_H

