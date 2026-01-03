# Lab 4 – Instruction Set & Bytecode Specification

This document freezes the instruction set, opcode mapping, bytecode format, and execution semantics for the Lab 4 Bytecode Virtual Machine. **No changes should be made without agreement from both partners.**

---

## 1. Core Design Decisions (Frozen)

- **VM type**: Stack-based virtual machine
- **Operand size**: 32-bit signed integer (`int32_t`)
- **Endianness**: Big-endian for operands in bytecode
- **Execution model**: Single-threaded, deterministic
- **Stacks**:

  - Operand stack (for computation)
  - Return stack (for function calls)

---

## 2. Bytecode Layout

Each instruction in bytecode has the following format:

```
[ OPCODE : 1 byte ] [ OPERAND : 4 bytes (optional) ]
```

- Instructions **without operands** consist of only the opcode byte.
- Instructions **with operands** are followed by a 4-byte signed integer.

### Examples

| Assembly | Bytecode (hex)   |
| -------- | ---------------- |
| `PUSH 5` | `01 00 00 00 05` |
| `ADD`    | `10`             |
| `JMP 12` | `20 00 00 00 0C` |

---

## 3. Instruction Set

### 3.1 Stack & Data Instructions

| Mnemonic | Opcode | Operand | Stack Effect   | Description              |
| -------- | ------ | ------- | -------------- | ------------------------ |
| PUSH val | `0x01` | int32   | `[ ] → [val]`  | Push constant onto stack |
| POP      | `0x02` | —       | `[val] → [ ]`  | Remove top of stack      |
| DUP      | `0x03` | —       | `[a] → [a, a]` | Duplicate top element    |
| HALT     | `0xFF` | —       | —              | Stop VM execution        |

---

### 3.2 Arithmetic & Logical Instructions

All arithmetic instructions:

- Pop **b** first, then **a**
- Push the result

| Mnemonic | Opcode | Stack Effect       | Description               |
| -------- | ------ | ------------------ | ------------------------- |
| ADD      | `0x10` | `[a, b] → [a + b]` | Addition                  |
| SUB      | `0x11` | `[a, b] → [a − b]` | Subtraction               |
| MUL      | `0x12` | `[a, b] → [a × b]` | Multiplication            |
| DIV      | `0x13` | `[a, b] → [a / b]` | Integer division          |
| CMP      | `0x14` | `[a, b] → [0/1]`   | Push 1 if `a < b`, else 0 |

---

### 3.3 Control Flow Instructions

- All jump addresses are **absolute bytecode addresses**
- `JZ` and `JNZ` **consume** the top of the operand stack

| Mnemonic | Opcode | Operand | Description               |
| -------- | ------ | ------- | ------------------------- |
| JMP addr | `0x20` | int32   | Unconditional jump        |
| JZ addr  | `0x21` | int32   | Jump if top of stack == 0 |
| JNZ addr | `0x22` | int32   | Jump if top of stack != 0 |

---

### 3.4 Memory Instructions

- Memory is a global integer array
- Index is provided as immediate operand

| Mnemonic  | Opcode | Operand | Stack Effect  | Description                 |
| --------- | ------ | ------- | ------------- | --------------------------- |
| STORE idx | `0x30` | int32   | `[val] → [ ]` | Store value in memory[idx]  |
| LOAD idx  | `0x31` | int32   | `[ ] → [val]` | Load memory[idx] onto stack |

---

### 3.5 Function Call Instructions

#### CALL

- Push `(PC after CALL)` onto **return stack**
- Set `PC = target address`

#### RET

- Pop return address from return stack
- Set `PC = return address`

| Mnemonic  | Opcode | Operand | Description              |
| --------- | ------ | ------- | ------------------------ |
| CALL addr | `0x40` | int32   | Call function at address |
| RET       | `0x41` | —       | Return from function     |

---

## 4. Error Handling Rules

- Invalid opcode → terminate VM with error message
- Stack underflow/overflow → terminate VM
- Division by zero → terminate VM
- Invalid memory index → terminate VM
- RET with empty return stack → terminate VM

---

## 5. Responsibilities Agreement

- **VM implementation** must follow this spec exactly
- **Assembler** must emit bytecode strictly in this format
- Any change requires updating this document and informing both partners

---

## 6. Next Step

After freezing this document:

1. VM skeleton implementation may begin
2. Assembler implementation may begin in parallel
3. This document should be included in the final submission/report
