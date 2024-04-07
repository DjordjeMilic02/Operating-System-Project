.extern _ZN5riscv20handleSupervisorTrapEv

.align 4
.global _ZN5riscv14supervisorTrapEv
.type _ZN5riscv14supervisorTrapEv, @function
_ZN5riscv14supervisorTrapEv:

    j _ZN5riscv15supervisorTrapxEv
    j _ZN5riscv15supervisorTrapyEv
    sret
    sret
    sret
    sret
    sret
    sret
    sret
    j _ZN5riscv15supervisorTrapzEv