# ArchC Performance Benchmarking & Bottleneck Analysis

This directory contains a complete 32-bit RISC processor model (`bench32`), synthetic test generators, and an automated performance profiling suite designed to measure simulation throughput (MIPS) and detect architectural bottlenecks in ArchC simulator engines (`.x` binaries).

---

## 🚀 Quick Start

To run the automated benchmark across all optimization configurations:

```bash
cd tests/bench32
python3 run_profiler.py
```

To run individual workloads manually:

```bash
# Generate workload hex files
python3 bench_gen.py

# Run on the simulator
./bench32.x --load=workload_alu.hex
./bench32.x --load=workload_mem.hex
./bench32.x --load=workload_branch.hex
./bench32.x --load=workload_mixed.hex
```

---

## 🧪 Workload Suites

The test suite evaluates 4 representative execution patterns:

| Workload | Dynamic Instructions | Characteristics & Focus |
|---|---|---|
| **ALU Intensive** | ~40,000,000 | Arithmetic & bitwise logic (`add`, `sub`, `and`, `or`, `xor`, `slt`, `sll`). Measures raw pipeline execution and register bank access. |
| **Memory Read/Write** | ~24,000,000 | Continuous store (`sw`) and load (`lw`) cycles. Evaluates `ac_mem` storage subsystem, address decoding, and memory port overhead. |
| **Branch & Control Flow** | ~22,000,000 | Alternating taken/not-taken branches (`beq`, `bne`). Tests PC update mechanism, target recalculation, and branch prediction impacts. |
| **Mixed Kernel** | ~24,000,000 | Realistic algorithmic kernel (Fibonacci/Hash) combining arithmetic, shifts, conditional branches, and memory buffers. |

---

## 📊 Benchmark Results

Measured on Apple Silicon (ARM64 macOS, SystemC 3.0.2, Clang C++17 `-O3`):

```
================================================================================
Configuration                                 | Workload                 | MIPS       | Time (s)
================================================================================
Optimized (--no-wait --no-curr-instr-id)      | ALU Intensive            |   507.86   |  0.079s
Optimized (--no-wait --no-curr-instr-id)      | Memory Read/Write        |   341.67   |  0.070s
Optimized (--no-wait --no-curr-instr-id)      | Branch & Control Flow    |   361.44   |  0.061s
Optimized (--no-wait --no-curr-instr-id)      | Mixed Compute/Mem Kernel |   450.22   |  0.053s
--------------------------------------------------------------------------------
Default (Direct Threading + Decode Cache + O3) | ALU Intensive            |   154.27   |  0.259s
Default (Direct Threading + Decode Cache + O3) | Memory Read/Write        |   119.72   |  0.200s
Default (Direct Threading + Decode Cache + O3) | Branch & Control Flow    |   142.36   |  0.155s
Default (Direct Threading + Decode Cache + O3) | Mixed Compute/Mem Kernel |   136.63   |  0.176s
--------------------------------------------------------------------------------
No Direct Threading (-nt)                     | ALU Intensive            |   144.67   |  0.276s
No Direct Threading (-nt)                     | Memory Read/Write        |   113.39   |  0.212s
No Direct Threading (-nt)                     | Branch & Control Flow    |   132.13   |  0.167s
No Direct Threading (-nt)                     | Mixed Compute/Mem Kernel |   129.00   |  0.186s
--------------------------------------------------------------------------------
Full Decode Optimization (-fdc)               | ALU Intensive            |   147.90   |  0.270s
Full Decode Optimization (-fdc)               | Memory Read/Write        |   114.16   |  0.210s
Full Decode Optimization (-fdc)               | Branch & Control Flow    |   133.93   |  0.164s
Full Decode Optimization (-fdc)               | Mixed Compute/Mem Kernel |   133.03   |  0.180s
--------------------------------------------------------------------------------
No Decode Cache (-ndc)                        | ALU Intensive            |    27.85   |  1.436s
No Decode Cache (-ndc)                        | Memory Read/Write        |    30.57   |  0.785s
No Decode Cache (-ndc)                        | Branch & Control Flow    |    32.68   |  0.673s
No Decode Cache (-ndc)                        | Mixed Compute/Mem Kernel |    28.79   |  0.833s
================================================================================
```

---

## 🔍 Bottleneck Analysis & Optimization Insights

### 1. Decode Cache (`DEC_CACHE`) vs On-The-Fly Decoding
* **Impact**: **5.5× speedup** with decode cache enabled (~150 MIPS vs ~27 MIPS).
* **Cause**: Without decode cache, `(ISA.decoder)->Decode(...)` executes bit-shifting, mask checks, and dynamic format selection on every single instruction fetch.
* **Recommendation**: Keep `DEC_CACHE` enabled (default in `acsim`).

### 2. Direct Threading (`goto *dispatch()`) vs Switch-Case Dispatch
* **Impact**: Direct Threading yields **7–10% higher MIPS** over `switch-case` dispatch.
* **Cause**: Computed gotos minimize loop overhead and allow better branch prediction on the host CPU compared to centralized switch statements.

### 3. Memory Subsystem Indirection (`ac_mem` / `ac_memport`)
* **Impact**: **~21% lower throughput** in memory workloads (119 MIPS vs 150 MIPS ALU).
* **Cause**: Each `DM.read()` or `DM.write()` traverses `ac_memport` wrapper methods, performs page index lookups, verifies alignment, and checks endianness match (`ac_match_endian`).
* **Recommendation**: For performance-critical blocks, utilize direct pointer memory access or compiled simulation (`accsim`).

### 4. Per-Instruction Dispatch Hot-Path Overhead
In `acsim`, the `dispatch()` loop executes several checks per cycle:
```cpp
if (ac_qk.need_sync()) { ac_qk.sync(); }   // SystemC quantum keeper check
if (ac_pc >= dec_cache_size) { ... }       // PC bounds checking
ac_instr_counter++;                        // Global instruction counter
instr_dec = (DEC_CACHE + (ac_pc));         // Array indexing
ISA.cur_instr_id = ins_id;                 // Global state update
```
Flags such as `--no-wait` (`-nw`) and `--no-pc-addr-ver` (`-npv`) prune unnecessary guards on the hot loop.

---

## 📁 Files in this Directory

- [`bench32.ac`](bench32.ac): Architecture description (registers, memory, endianness).
- [`bench32.isa`](bench32.isa): Instruction Set Architecture (opcodes, formats, assembly syntax).
- [`bench32_isa.cpp`](bench32_isa.cpp): Behavior implementations for ALU, memory, and branch instructions.
- [`bench_gen.py`](bench_gen.py): Python generator for custom synthetic benchmark programs.
- [`run_profiler.py`](run_profiler.py): Automated benchmark runner that rebuilds and tests multiple simulator configurations.
- [`Makefile`](Makefile): Simulator build script generated by ArchC.
