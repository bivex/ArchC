# ArchC RISC-V (RV32I / RV64I) Architecture Model & Verification Suite

This directory contains a complete RISC-V (RV32I / RV64I) processor description, simulator, and automated test suite modeled in ArchC and SystemC.

## Architecture Model Files
- `riscv.ac`: Processor definition with 32 integer registers (`x0`..`x31`), 512MB memory space, little-endian.
- `riscv.isa`: Complete instruction formats (R-type, I-type, Shift-type, S-type, B-type, U-type, J-type).
- `riscv_isa.cpp`: Standard RISC-V execution semantics with `x0` hardwired zero invariant.
- `riscv_test_gen.py`: Automated test generator producing both HEX and native ELF64 binaries (`EM_RISCV = 243`).

## Benchmarks & Performance
- **ALU Intensive (`rv_alu`)**: ~500 MIPS (40M instructions in 0.08s)
- **Memory Read/Write (`rv_mem`)**: ~400 MIPS (24M instructions in 0.06s)
- **Branch & Control Flow (`rv_branch`)**: ~366 MIPS (22M instructions in 0.06s)

## Building and Running

```bash
# 1. Generate simulator
../../install_local/bin/acsim riscv.ac -nw -nci

# 2. Build simulator
export PKG_CONFIG_PATH=/Volumes/External/Code/ArchC/install_local/lib/pkgconfig:$PKG_CONFIG_PATH
make -f Makefile.archc -j$(sysctl -n hw.ncpu)

# 3. Run test suite
python3 riscv_test_gen.py
```
