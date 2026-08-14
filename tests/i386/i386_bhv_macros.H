#ifndef _I386_BHV_MACROS_H
#define _I386_BHV_MACROS_H

typedef ac_memport<i386_parms::ac_word, i386_parms::ac_Hword> ac_memory;

#define ac_behavior(instr) AC_BEHAVIOR_##instr ()

#define AC_BEHAVIOR_instruction() i386_parms::i386_isa::_behavior_instruction(uint8_t opcode)

#define AC_BEHAVIOR_begin() i386_parms::i386_isa::_behavior_begin()
#define AC_BEHAVIOR_end() i386_parms::i386_isa::_behavior_end()

#define AC_BEHAVIOR_Type_RR() i386_parms::i386_isa::_behavior_i386_Type_RR(uint8_t opcode, uint8_t mod, uint8_t reg, uint8_t rm, uint16_t pad)
#define AC_BEHAVIOR_Type_RI() i386_parms::i386_isa::_behavior_i386_Type_RI(uint8_t opcode, uint8_t reg, uint8_t pad, int16_t imm)
#define AC_BEHAVIOR_Type_RM() i386_parms::i386_isa::_behavior_i386_Type_RM(uint8_t opcode, uint8_t mod, uint8_t reg, uint8_t rm, int16_t disp)
#define AC_BEHAVIOR_Type_J() i386_parms::i386_isa::_behavior_i386_Type_J(uint8_t opcode, int32_t offset)
#define AC_BEHAVIOR_Type_HLT() i386_parms::i386_isa::_behavior_i386_Type_HLT(uint8_t opcode, uint32_t pad)

#define AC_BEHAVIOR_add_rr() i386_parms::i386_isa::behavior_add_rr(uint8_t opcode, uint8_t mod, uint8_t reg, uint8_t rm, uint16_t pad)
#define AC_BEHAVIOR_sub_rr() i386_parms::i386_isa::behavior_sub_rr(uint8_t opcode, uint8_t mod, uint8_t reg, uint8_t rm, uint16_t pad)
#define AC_BEHAVIOR_and_rr() i386_parms::i386_isa::behavior_and_rr(uint8_t opcode, uint8_t mod, uint8_t reg, uint8_t rm, uint16_t pad)
#define AC_BEHAVIOR_or_rr() i386_parms::i386_isa::behavior_or_rr(uint8_t opcode, uint8_t mod, uint8_t reg, uint8_t rm, uint16_t pad)
#define AC_BEHAVIOR_xor_rr() i386_parms::i386_isa::behavior_xor_rr(uint8_t opcode, uint8_t mod, uint8_t reg, uint8_t rm, uint16_t pad)
#define AC_BEHAVIOR_mov_rr() i386_parms::i386_isa::behavior_mov_rr(uint8_t opcode, uint8_t mod, uint8_t reg, uint8_t rm, uint16_t pad)
#define AC_BEHAVIOR_mov_ri() i386_parms::i386_isa::behavior_mov_ri(uint8_t opcode, uint8_t reg, uint8_t pad, int16_t imm)
#define AC_BEHAVIOR_add_ri() i386_parms::i386_isa::behavior_add_ri(uint8_t opcode, uint8_t reg, uint8_t pad, int16_t imm)
#define AC_BEHAVIOR_sub_ri() i386_parms::i386_isa::behavior_sub_ri(uint8_t opcode, uint8_t reg, uint8_t pad, int16_t imm)
#define AC_BEHAVIOR_cmp_ri() i386_parms::i386_isa::behavior_cmp_ri(uint8_t opcode, uint8_t reg, uint8_t pad, int16_t imm)
#define AC_BEHAVIOR_mov_rm() i386_parms::i386_isa::behavior_mov_rm(uint8_t opcode, uint8_t mod, uint8_t reg, uint8_t rm, int16_t disp)
#define AC_BEHAVIOR_mov_mr() i386_parms::i386_isa::behavior_mov_mr(uint8_t opcode, uint8_t mod, uint8_t reg, uint8_t rm, int16_t disp)
#define AC_BEHAVIOR_jmp() i386_parms::i386_isa::behavior_jmp(uint8_t opcode, int32_t offset)
#define AC_BEHAVIOR_jne() i386_parms::i386_isa::behavior_jne(uint8_t opcode, int32_t offset)
#define AC_BEHAVIOR_je() i386_parms::i386_isa::behavior_je(uint8_t opcode, int32_t offset)
#define AC_BEHAVIOR_hlt() i386_parms::i386_isa::behavior_hlt(uint8_t opcode, uint32_t pad)


#endif //_I386_BHV_MACROS_H

