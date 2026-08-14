#ifndef _ESP32S3_BHV_MACROS_H
#define _ESP32S3_BHV_MACROS_H

typedef ac_memport<esp32s3_parms::ac_word, esp32s3_parms::ac_Hword> ac_memory;

#define ac_behavior(instr) AC_BEHAVIOR_##instr ()

#define AC_BEHAVIOR_instruction() esp32s3_parms::esp32s3_isa::_behavior_instruction(uint8_t op)

#define AC_BEHAVIOR_begin() esp32s3_parms::esp32s3_isa::_behavior_begin()
#define AC_BEHAVIOR_end() esp32s3_parms::esp32s3_isa::_behavior_end()

#define AC_BEHAVIOR_Type_RRR() esp32s3_parms::esp32s3_isa::_behavior_esp32s3_Type_RRR(uint8_t op, uint8_t op1, uint8_t r, uint8_t s, uint8_t t, uint8_t op2)
#define AC_BEHAVIOR_Type_RRI8() esp32s3_parms::esp32s3_isa::_behavior_esp32s3_Type_RRI8(uint8_t op, int8_t imm8, uint8_t s, uint8_t t, uint8_t pad1)
#define AC_BEHAVIOR_Type_RI16() esp32s3_parms::esp32s3_isa::_behavior_esp32s3_Type_RI16(uint8_t op, uint8_t t, int16_t imm16, uint8_t pad2)
#define AC_BEHAVIOR_Type_L32I() esp32s3_parms::esp32s3_isa::_behavior_esp32s3_Type_L32I(uint8_t op, uint8_t disp, uint8_t s, uint8_t t, uint8_t pad3)
#define AC_BEHAVIOR_Type_Branch() esp32s3_parms::esp32s3_isa::_behavior_esp32s3_Type_Branch(uint8_t op, int16_t bdisp, uint8_t s, uint8_t t, uint8_t pad4)
#define AC_BEHAVIOR_Type_Jump() esp32s3_parms::esp32s3_isa::_behavior_esp32s3_Type_Jump(uint8_t op, int32_t target, uint8_t pad5)
#define AC_BEHAVIOR_Type_Loop() esp32s3_parms::esp32s3_isa::_behavior_esp32s3_Type_Loop(uint8_t op, uint8_t ldisp, uint8_t s, uint16_t pad6)
#define AC_BEHAVIOR_Type_PIE() esp32s3_parms::esp32s3_isa::_behavior_esp32s3_Type_PIE(uint8_t op, uint8_t qr, uint8_t qs, uint8_t qt, uint8_t mode, uint8_t pad8)
#define AC_BEHAVIOR_Type_Halt() esp32s3_parms::esp32s3_isa::_behavior_esp32s3_Type_Halt(uint8_t op, uint32_t pad7)

#define AC_BEHAVIOR_add_op() esp32s3_parms::esp32s3_isa::behavior_add_op(uint8_t op, uint8_t op1, uint8_t r, uint8_t s, uint8_t t, uint8_t op2)
#define AC_BEHAVIOR_sub_op() esp32s3_parms::esp32s3_isa::behavior_sub_op(uint8_t op, uint8_t op1, uint8_t r, uint8_t s, uint8_t t, uint8_t op2)
#define AC_BEHAVIOR_and_op() esp32s3_parms::esp32s3_isa::behavior_and_op(uint8_t op, uint8_t op1, uint8_t r, uint8_t s, uint8_t t, uint8_t op2)
#define AC_BEHAVIOR_or_op() esp32s3_parms::esp32s3_isa::behavior_or_op(uint8_t op, uint8_t op1, uint8_t r, uint8_t s, uint8_t t, uint8_t op2)
#define AC_BEHAVIOR_xor_op() esp32s3_parms::esp32s3_isa::behavior_xor_op(uint8_t op, uint8_t op1, uint8_t r, uint8_t s, uint8_t t, uint8_t op2)
#define AC_BEHAVIOR_mull_op() esp32s3_parms::esp32s3_isa::behavior_mull_op(uint8_t op, uint8_t op1, uint8_t r, uint8_t s, uint8_t t, uint8_t op2)
#define AC_BEHAVIOR_addi_op() esp32s3_parms::esp32s3_isa::behavior_addi_op(uint8_t op, int8_t imm8, uint8_t s, uint8_t t, uint8_t pad1)
#define AC_BEHAVIOR_addmi_op() esp32s3_parms::esp32s3_isa::behavior_addmi_op(uint8_t op, int8_t imm8, uint8_t s, uint8_t t, uint8_t pad1)
#define AC_BEHAVIOR_movi_op() esp32s3_parms::esp32s3_isa::behavior_movi_op(uint8_t op, uint8_t t, int16_t imm16, uint8_t pad2)
#define AC_BEHAVIOR_l32i_op() esp32s3_parms::esp32s3_isa::behavior_l32i_op(uint8_t op, uint8_t disp, uint8_t s, uint8_t t, uint8_t pad3)
#define AC_BEHAVIOR_s32i_op() esp32s3_parms::esp32s3_isa::behavior_s32i_op(uint8_t op, uint8_t disp, uint8_t s, uint8_t t, uint8_t pad3)
#define AC_BEHAVIOR_bne_op() esp32s3_parms::esp32s3_isa::behavior_bne_op(uint8_t op, int16_t bdisp, uint8_t s, uint8_t t, uint8_t pad4)
#define AC_BEHAVIOR_beq_op() esp32s3_parms::esp32s3_isa::behavior_beq_op(uint8_t op, int16_t bdisp, uint8_t s, uint8_t t, uint8_t pad4)
#define AC_BEHAVIOR_blt_op() esp32s3_parms::esp32s3_isa::behavior_blt_op(uint8_t op, int16_t bdisp, uint8_t s, uint8_t t, uint8_t pad4)
#define AC_BEHAVIOR_bge_op() esp32s3_parms::esp32s3_isa::behavior_bge_op(uint8_t op, int16_t bdisp, uint8_t s, uint8_t t, uint8_t pad4)
#define AC_BEHAVIOR_j_op() esp32s3_parms::esp32s3_isa::behavior_j_op(uint8_t op, int32_t target, uint8_t pad5)
#define AC_BEHAVIOR_call0_op() esp32s3_parms::esp32s3_isa::behavior_call0_op(uint8_t op, int32_t target, uint8_t pad5)
#define AC_BEHAVIOR_loop_op() esp32s3_parms::esp32s3_isa::behavior_loop_op(uint8_t op, uint8_t ldisp, uint8_t s, uint16_t pad6)
#define AC_BEHAVIOR_ee_vadd_s16() esp32s3_parms::esp32s3_isa::behavior_ee_vadd_s16(uint8_t op, uint8_t qr, uint8_t qs, uint8_t qt, uint8_t mode, uint8_t pad8)
#define AC_BEHAVIOR_ee_vmul_s16() esp32s3_parms::esp32s3_isa::behavior_ee_vmul_s16(uint8_t op, uint8_t qr, uint8_t qs, uint8_t qt, uint8_t mode, uint8_t pad8)
#define AC_BEHAVIOR_ee_vdot_s8() esp32s3_parms::esp32s3_isa::behavior_ee_vdot_s8(uint8_t op, uint8_t qr, uint8_t qs, uint8_t qt, uint8_t mode, uint8_t pad8)
#define AC_BEHAVIOR_ee_vld_q() esp32s3_parms::esp32s3_isa::behavior_ee_vld_q(uint8_t op, uint8_t qr, uint8_t qs, uint8_t qt, uint8_t mode, uint8_t pad8)
#define AC_BEHAVIOR_ee_vst_q() esp32s3_parms::esp32s3_isa::behavior_ee_vst_q(uint8_t op, uint8_t qr, uint8_t qs, uint8_t qt, uint8_t mode, uint8_t pad8)
#define AC_BEHAVIOR_halt_op() esp32s3_parms::esp32s3_isa::behavior_halt_op(uint8_t op, uint32_t pad7)


#endif //_ESP32S3_BHV_MACROS_H

