// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

@1
D = A;

(SETWHITE)
    @KBD
    D = A;
    @R0
    M = D;
    @SCREEN
    D = A;
    @R0
    M = M - D; // R0 = &KBD - &SCREEN
    (WHITESCREEN) //while (R0 >= 0)
        @R0
        D = M;
        @ENDWHITESCREEN
        D;JLT //if (R0 < 0) break;

        @SCREEN
        A = A + D;
        M = 0; //RAM[&SCREEN + R0] = 0
        @R0
        M = M - 1;  //R0--;

        @WHITESCREEN
        0;JMP
    (ENDWHITESCREEN)
    (WAITFORKEYBOARD)
        @KBD
        D = M;
        @WAITFORKEYBOARD
        D;JEQ
    (ENDWAITFORKEYBOARD)
    @SETBLACK
    0;JMP
(ENDSETWHITE)

(SETBLACK)
    @KBD    
    D = A;
    @R0
    M = D;
    @SCREEN
    D = A;
    @R0
    M = M - D; // R0 = &KBD - &SCREEN
    (BLACKSCREEN) //while (R0 >= 0)
        @R0
        D = M;
        @ENDBLACKSCREEN
        D;JLT //if (R0 < 0) break;

        @SCREEN
        A = A + D;
        M = -1; //RAM[&SCREEN + R0] = -1
        @R0
        M = M - 1;  //R0--;

        @BLACKSCREEN
        0;JMP
    (ENDBLACKSCREEN)
    (WAITFORKEYBOARD2)
        @KBD
        D = M;
        @WAITFORKEYBOARD2
        D;JNE
    (ENDWAITFORKEYBOARD2)
    @SETWHITE
    0;JMP
(ENDSETBLACK)

(END)
@END
0;JMP
