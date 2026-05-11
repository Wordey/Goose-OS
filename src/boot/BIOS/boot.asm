; now it only reading next bytes(next file after this)

load_disk:
    pusha

    push dx
    mov ah, 0x02    ; BIOS read function
    mov al, dh      ; read dh
    mov ch, 0       ; select cylinder 0
    mov dh, 0       ; select first side
    mov cl, 2       ; start reading from the next sector(sector 2)
    int 0x13
    pop dx
    jc disk_error
    cmp al, dh
    jne disk_error
    popa
    ret

disk_error:
    mov bx, 0
    call print_string
    jmp $

MSG:
    dd "Error when reading disk sectors",0
    
times 510-($-$$) db 0
dw 0xAA55