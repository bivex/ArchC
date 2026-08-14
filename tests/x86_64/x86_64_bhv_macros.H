#ifndef _X86_64_BHV_MACROS_H
#define _X86_64_BHV_MACROS_H

typedef ac_memport<x86_64_parms::ac_word, x86_64_parms::ac_Hword> ac_memory;

#define ac_behavior(instr) AC_BEHAVIOR_##instr ()

#define AC_BEHAVIOR_instruction() x86_64_parms::x86_64_isa::_behavior_instruction(uint8_t opcode)

#define AC_BEHAVIOR_begin() x86_64_parms::x86_64_isa::_behavior_begin()
#define AC_BEHAVIOR_end() x86_64_parms::x86_64_isa::_behavior_end()

#define AC_BEHAVIOR_Type_RR() x86_64_parms::x86_64_isa::_behavior_x86_64_Type_RR(uint8_t opcode, uint8_t rex, uint8_t reg, uint8_t rm, uint16_t pad)
#define AC_BEHAVIOR_Type_RI() x86_64_parms::x86_64_isa::_behavior_x86_64_Type_RI(uint8_t opcode, uint8_t reg, uint8_t pad, int16_t imm)
#define AC_BEHAVIOR_Type_RM() x86_64_parms::x86_64_isa::_behavior_x86_64_Type_RM(uint8_t opcode, uint8_t reg, uint8_t rm, int16_t disp)
#define AC_BEHAVIOR_Type_J() x86_64_parms::x86_64_isa::_behavior_x86_64_Type_J(uint8_t opcode, int32_t offset)
#define AC_BEHAVIOR_Type_HLT() x86_64_parms::x86_64_isa::_behavior_x86_64_Type_HLT(uint8_t opcode, uint32_t pad)

#define AC_BEHAVIOR_add_rr() x86_64_parms::x86_64_isa::behavior_add_rr(uint8_t opcode, uint8_t rex, uint8_t reg, uint8_t rm, uint16_t pad)
#define AC_BEHAVIOR_sub_rr() x86_64_parms::x86_64_isa::behavior_sub_rr(uint8_t opcode, uint8_t rex, uint8_t reg, uint8_t rm, uint16_t pad)
#define AC_BEHAVIOR_and_rr() x86_64_parms::x86_64_isa::behavior_and_rr(uint8_t opcode, uint8_t rex, uint8_t reg, uint8_t rm, uint16_t pad)
#define AC_BEHAVIOR_or_rr() x86_64_parms::x86_64_isa::behavior_or_rr(uint8_t opcode, uint8_t rex, uint8_t reg, uint8_t rm, uint16_t pad)
#define AC_BEHAVIOR_xor_rr() x86_64_parms::x86_64_isa::behavior_xor_rr(uint8_t opcode, uint8_t rex, uint8_t reg, uint8_t rm, uint16_t pad)
#define AC_BEHAVIOR_mov_rr() x86_64_parms::x86_64_isa::behavior_mov_rr(uint8_t opcode, uint8_t rex, uint8_t reg, uint8_t rm, uint16_t pad)
#define AC_BEHAVIOR_imul_rr() x86_64_parms::x86_64_isa::behavior_imul_rr(uint8_t opcode, uint8_t rex, uint8_t reg, uint8_t rm, uint16_t pad)
#define AC_BEHAVIOR_mov_ri() x86_64_parms::x86_64_isa::behavior_mov_ri(uint8_t opcode, uint8_t reg, uint8_t pad, int16_t imm)
#define AC_BEHAVIOR_add_ri() x86_64_parms::x86_64_isa::behavior_add_ri(uint8_t opcode, uint8_t reg, uint8_t pad, int16_t imm)
#define AC_BEHAVIOR_sub_ri() x86_64_parms::x86_64_isa::behavior_sub_ri(uint8_t opcode, uint8_t reg, uint8_t pad, int16_t imm)
#define AC_BEHAVIOR_cmp_ri() x86_64_parms::x86_64_isa::behavior_cmp_ri(uint8_t opcode, uint8_t reg, uint8_t pad, int16_t imm)
#define AC_BEHAVIOR_mov_rm() x86_64_parms::x86_64_isa::behavior_mov_rm(uint8_t opcode, uint8_t reg, uint8_t rm, int16_t disp)
#define AC_BEHAVIOR_mov_mr() x86_64_parms::x86_64_isa::behavior_mov_mr(uint8_t opcode, uint8_t reg, uint8_t rm, int16_t disp)
#define AC_BEHAVIOR_jmp() x86_64_parms::x86_64_isa::behavior_jmp(uint8_t opcode, int32_t offset)
#define AC_BEHAVIOR_jne() x86_64_parms::x86_64_isa::behavior_jne(uint8_t opcode, int32_t offset)
#define AC_BEHAVIOR_je() x86_64_parms::x86_64_isa::behavior_je(uint8_t opcode, int32_t offset)
#define AC_BEHAVIOR_hlt() x86_64_parms::x86_64_isa::behavior_hlt(uint8_t opcode, uint32_t pad)


#endif //_X86_64_BHV_MACROS_H

