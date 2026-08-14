ArchC
=====

**Architecture Description Language**

ArchC is a powerful and modern open-source architecture description language designed at University of Campinas by the ArchC team in the Computer Systems Laboratory, Institute of Computing.

License
-------
 - ArchC tools are provided under the GNU GPL license.
   See [Copying](COPYING) file for details on this license.

 - ArchC utility library, i.e. all files stored in the `src/aclib`
   directory of the ArchC source tree, are provided under the GNU LGPL
   license. See the [Copying Lib](COPYING.LIB) file for details on this license.


Requirements & Dependencies
---------------------------

### Prerequisites

1. **GNU Autotools & Build Utilities**:
   - `autoconf` (>= 2.59)
   - `automake` (>= 1.14)
   - `libtool` / `glibtoolize`
   - `pkg-config`
   - `flex` and `bison`
   - `m4`
   - `make`
   - C/C++ compiler supporting C++17 (`clang++` or `g++`)

2. **SystemC (Required for Simulator Generation Tools)**:
   - SystemC >= 2.3.0 (including 3.0.x) with TLM and TLM2 support.
   - **macOS (Homebrew)**:
     ```bash
     brew install systemc pkg-config autoconf automake libtool flex bison
     ```
   - **Linux (Ubuntu/Debian)**:
     ```bash
     sudo apt-get install build-essential autoconf automake libtool pkg-config flex bison libsystemc-dev
     ```
     Or compile and install SystemC from [Accellera SystemC](https://www.accellera.org/downloads/standards/systemc).

3. **Optional Libraries**:
   - `elfutils` (`libelf` and `libdw`) for High Level Trace (HLT) feature.
   - GNU `binutils` source for generating binary utilities (`acasm`, `acld`).
   - GNU `gdb` source for generating debugger support.


Configuration
-------------

1. Generate the build configuration scripts:
   ```bash
   ./autogen.sh
   ```

2. Run `./configure` to detect dependencies and set build options:
   ```bash
   ./configure
   ```

   **Configuration Flags & Options**:
   * `--prefix=<install-dir>`: Target directory for installation (default: `/usr/local`).
   * `--with-systemc=<systemc-install-path>`: Path to custom SystemC installation (if not detected via `pkg-config`).
   * `--with-binutils=<binutils-src-path>`: Directory where Binutils source files are stored (to generate `acasm`/`acld`).
   * `--with-gdb=<gdb-src-path>`: Directory where GDB source files are stored (to generate GDB integration).
   * `--disable-hlt`: Disable High Level Trace feature (if `libelf`/`libdw` are not present).


Building
--------

Compile all tools and libraries:
```bash
# macOS
make -j$(sysctl -n hw.ncpu)

# Linux
make -j$(nproc)
```

The build process produces the following core tools in `src/`:
- `src/acsim/acsim`: Interpreted Simulator Generator
- `src/accsim/accsim`: Compiled Simulator Generator
- `src/actsim/actsim`: Timed Simulator Generator
- `src/acbinutils/bmdsfg`: Binary Utilities Generator
- `src/acbinutils/acrelconvert`: Relocation Conversion Utility
- `src/aclib/libarchc.la`: ArchC Runtime Library
- `src/powersc/libpowersc.a`: PowerSC Library


Testing & Verification
----------------------

1. **Verify Build Targets**:
   Run the test target:
   ```bash
   make check
   ```

2. **Verify Tool Binaries**:
   Check that all generated simulator generators run properly:
   ```bash
   src/acsim/acsim --help
   src/accsim/accsim --help
   src/actsim/actsim --help
   ```

3. **Verify Environment Setup**:
   Source the environment configuration script:
   ```bash
   source env.sh
   ```
   This script configures `PATH`, `LD_LIBRARY_PATH` (or `DYLD_LIBRARY_PATH`), `PKG_CONFIG_PATH`, and `ARCHC_PREFIX`.


Installation
------------

To install ArchC tools and headers into the configured prefix (default `/usr/local`):
```bash
make install
```


Running Simulations
-------------------

To generate and run a simulator for an ArchC architecture model (e.g. `mips.ac`):
```bash
# Using the helper runner
./bin/ac_run <arch_name> <target_program>

# Or generating a simulator manually with acsim:
acsim <arch_name>.ac
make -f Makefile.archc
./<arch_name>.x --load=<target_program>
```


Contributing
------------

See [Contributing](CONTRIBUTING.md)


More Information
----------------

- Documentation and guides are available in the [`doc/`](doc/) directory:
  - [`doc/whats_new.txt`](doc/whats_new.txt)
  - [`doc/tlm_howto.txt`](doc/tlm_howto.txt)
  - [`doc/porting_to_2.txt`](doc/porting_to_2.txt)
- Language overview, models, and tutorials: http://www.archc.org

---
The ArchC Team  
Computer Systems Laboratory (LSC)  
IC-UNICAMP  
http://www.lsc.ic.unicamp.br
