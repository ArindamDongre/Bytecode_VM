# Lab 4 – Bytecode Virtual Machine

## Overview

Stack-based bytecode virtual machine with assembler.

## Directory Structure

- vm/: VM execution engine
- assembler/: Assembly → bytecode translator
- tests/: Sample programs
- include/: opcodes

## Build

```bash
make
```

## Assemble

```
./assembler_exec tests/arith.asm arith.bin
```

## Run

```
./vm_exec arith.bin
```
