.model tiny
.code
org 100h
locals @@
GRANTED equ '1'
Start:
	mov dx, offset HelloStr		; dx = &MyStr
	mov ah, 09h			        ; ah = 09h
	int 21h				        ; DOS Fn 09h = puts(dx) puts(HelloStr)

    call INPUT_PASSWORD

	mov ax, 4c00h			    ; ax = 4c00h
	int 21h				        ; DOS Fn 4ch = exit(al)

HelloStr    : db "Hello, please enter password. Be careful. Wrong password will delete all your data...:$"
AccessStr   : db "YEESESESESESES$"
DenyStr     : db "Plaki-plaki$"
PWD         : db "qwerty"

INPUT_PASSWORD proc
    push bp
    sub sp, 17
    mov bp, sp
    mov di, bp
    mov ah, 01
    mov byte ptr [bp + 16], '$'
@@GET_CHAR:
    int 21h
    cmp al, 0Dh
    je @@END_GETS
    stosb
    jmp @@GET_CHAR

@@END_GETS:
    cmp byte ptr [bp + 16], GRANTED     ; only noobs place check before strcmp. Easy error
    je @@YES

    mov di, bp
    mov si, offset PWD
    mov cx, 6
    rep cmpsb


    jne @@DENY_ACCESS
    mov byte ptr [bp + 16], GRANTED
    jmp @@END_1
    @@DENY_ACCESS:
    mov byte ptr [bp + 16], 0
    @@END_1:

    cmp byte ptr [bp + 16], GRANTED
    jne @@NO
    @@YES:
    call ACCESS_GRANTED
    jmp @@END
    @@NO:
    call ACCESS_DENIED                      ; here not safe stack - return address not protected
    @@END:

    add sp, 17
    pop bp
    ret
    endp

ACCESS_GRANTED proc
    mov dx, offset AccessStr
	mov ah, 09h			        ; ah = 09h
	int 21h				        ; DOS Fn 09h = puts(dx)
    ret
    endp

ACCESS_DENIED proc
    mov dx, offset DenyStr
	mov ah, 09h			        ; ah = 09h
	int 21h				        ; DOS Fn 09h = puts(dx)
    ret
    endp

buffer:
end Start
