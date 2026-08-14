/**
 * @file      ac_arch_dec_if.H
 * @author    Thiago Massariolli Sigrist
 *            The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 *
 * @version   1.0
 * @date      Fri Nov 18 19:53:16 2005
 * 
 * @brief     Defines an ac_arch specialization that contains GetBits()
 * 
 * @attention Copyright (C) 2002-2006 --- The ArchC Team
 * 
 * This program is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; either version 2 of the License, or 
 * (at your option) any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

//////////////////////////////////////////////////////////////////////////////

// Standard includes

// SystemC includes

// ArchC includes
#include "ac_arch.H"
#include "ac_decoder_rt.H"
//#include "ac_memport.H"

//////////////////////////////////////////////////////////////////////////////

#ifndef _AC_ARCH_DEC_IF_H_
#define _AC_ARCH_DEC_IF_H_

//////////////////////////////////////////////////////////////////////////////

// Forward class declarations, needed to compile

//////////////////////////////////////////////////////////////////////////////

template <typename ac_word, typename ac_Hword> class ac_arch_dec_if:
  public ac_arch<ac_word, ac_Hword>, public ac_dec_prog_source {
public:
  explicit ac_arch_dec_if(int max_buffer) :
    ac_arch<ac_word, ac_Hword>(max_buffer) {}

  int ExpandInstrBuffer(int index) {
    //Expand the instruction buffer word by word, the number necessary to read position index
    int read = (index + 1) - this->quant;
    for(int i=0; i<read; i++){
      this->buffer[this->quant + i] = (this->INST_PORT)->read(this->decode_pc + (this->quant + i) * sizeof(ac_word));
    }
    this->quant += read;
    return this->quant;
  }

  unsigned long long GetBits(unsigned char* bu, int* quant, int last,
                             int quantity, int sign) {

    ac_word* buffer = (ac_word*) bu;

    //! Read the buffer using this macro
#define BUFFER(index) ((index<*quant) ? (buffer[index]) : (*quant=ExpandInstrBuffer(index),buffer[index]))

    int first = last - (quantity-1);
    const int word_bits = sizeof(ac_word) * 8;

    int index_first = first / word_bits;
    int index_last = last / word_bits;

    unsigned long long value = 0;

    // Single-word fast path (most common case in RISC ISAs)
    if (index_first == index_last) {
      ac_word w = BUFFER(index_first);
      if (!this->ac_mt_endian) {
        int shift = word_bits - (last % word_bits + 1);
        value = (shift >= 0 && shift < word_bits) ? (w >> shift) : 0;
      } else {
        int shift = first % word_bits;
        value = (shift >= 0 && shift < word_bits) ? (w >> shift) : 0;
      }
    } else {
      if (!this->ac_mt_endian) {
        for (int i=index_first; i<=index_last; i++) {
          value <<= word_bits;
          value |= BUFFER(i);
        }
        value >>= word_bits - (last % word_bits + 1);
      } else {
        for (int i=index_last; i>=index_first; i--) {
          value <<= word_bits;
          value |= BUFFER(i);
        }
        value >>= (first % word_bits);
      }
    }

    // Mask to the size of the field
    if (quantity > 0 && quantity < 64) {
      value &= (1ULL << quantity) - 1ULL;
    }

    // Sign extend if requested
    if (sign && quantity > 0 && quantity < 64) {
      unsigned long long sign_bit = 1ULL << (quantity - 1);
      if (value & sign_bit) {
        value |= (~0ULL) << quantity;
      }
    }

    return value;
#undef BUFFER
  }

};

//////////////////////////////////////////////////////////////////////////////

#endif // _AC_ARCH_DEC_IF_H_
