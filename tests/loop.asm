PUSH 1000

loop:
DUP
PUSH 0
SWAP
CMP
JZ end

PUSH 1
SUB
JMP loop

end:
HALT
