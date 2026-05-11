print_string:
    pusha
    mov ah, 0xe
    jmp read_charecter

read_charecter:
    mov al, [bx]

    cmp al, 0
    jne read_charecter
    popa
    ret

print_charecter:
    int 0x10
    add bx, 1
    jmp read_charecter