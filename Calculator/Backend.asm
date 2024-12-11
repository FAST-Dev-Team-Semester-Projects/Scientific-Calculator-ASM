; Program Description: A scientific calculator implemented in assembly language for floating-point and integer arithmetic
; The backend handles mathematical and scientific operations, while the frontend interacts with it for user input and output
; Authors: Syed Haider Murtaza, Mujtaba Kamran, Muhammad Rayyan

.686                            ; Target processor: Use instructions for Pentium-class machines
.MODEL FLAT, C                  ; Use the flat memory model with C calling conventions
.STACK 8192                     ; Define a stack segment of 8KB

.DATA
.CODE

    ; ===================================================================================================================================================
    ;                                               Explanation of FPU Stack Usage in this Program
    ; ===================================================================================================================================================
    ;
    ; This program uses the Floating-Point Unit (FPU) stack for floating-point operations instead of the regular CPU stack
    ; The FPU stack (st(0) to st(7)) is optimized for high-performance floating-point calculations
    ;
    ; Why the FPU Stack?
    ; - Efficiency: FPU registers are designed for fast floating-point operations
    ; - Precision: FPU registers offer higher precision for scientific calculations
    ; - FPU-specific instructions: Operations like faddp, fsubp, fsin, fcos are optimized for the FPU stack
    ;
    ; Trigonometric Functions:
    ; Built-in trigonometric functions (sin, cos, tan) are available only through the FPU stack, ensuring efficient and precise calculation
    ;
    ; Why Not the Regular Stack?
    ; The CPU stack isn't optimized for floating-point operations and lacks FPU-specific instructions, making it unsuitable for scientific calculations
    ; ==================================================================================================================================================


    ; =====================================================================================================
    ;                               Basic Floating-Point Arithmetic Operations
    ; =====================================================================================================

    ;------------------------------------------------------------------------------------------------------
    addition PROC
    ;
    ; Calculates and returns the sum of two 32-bit floating-point numbers
    ; Receives: 
    ;   - First parameter (a) at [esp+4] (32-bit floating-point number)
    ;   - Second parameter (b) at [esp+8] (32-bit floating-point number)
    ; Returns: 
    ;   - The sum (a + b) stored in EAX as a 32-bit floating-point number
    ; Requires: 
    ;   - Inputs should be valid 32-bit floating-point numbers
    ;------------------------------------------------------------------------------------------------------
        fld dword ptr [esp+4]    ; Load first parameter (a) onto the FPU stack (st(0))
        fld dword ptr [esp+8]    ; Load second parameter (b) onto the FPU stack (st(0)), push (a) to st(1)
        faddp st(1), st(0)       ; Add st(0) (b) and st(1) (a), store result in st(1), pop st(0)
        fst dword ptr [esp-4]    ; Store the result from st(0) into memory at [esp-4]
        mov eax, [esp-4]         ; Move the result from memory into EAX register for return
        ret                      ; Return with the result in EAX
    addition ENDP

    ;------------------------------------------------------------------------------------------------------
    subtraction PROC
    ;
    ; Calculates and returns the difference of two 32-bit floating-point numbers
    ; Receives:
    ;   - First parameter (a) at [esp+4] (32-bit floating-point number)
    ;   - Second parameter (b) at [esp+8] (32-bit floating-point number)
    ; Returns:
    ;   - The difference (b - a) stored in EAX as a 32-bit floating-point number
    ; Requires:
    ;   - Inputs should be valid 32-bit floating-point numbers
    ;------------------------------------------------------------------------------------------------------
        fld dword ptr [esp+8]    ; Load second parameter (b) onto the FPU stack (st(0))
        fld dword ptr [esp+4]    ; Load first parameter (a) onto the FPU stack (st(0)), push (b) to st(1)
        fsubp st(1), st(0)       ; Subtract st(0) (a) from st(1) (b), store result in st(1), pop st(0), result in st(0)
        fst dword ptr [esp-4]    ; Store the result from st(0) into memory at [esp-4]
        mov eax, [esp-4]         ; Move the result from memory into EAX register for return
        ret                      ; Return with the result in EAX
    subtraction ENDP

    ;------------------------------------------------------------------------------------------------------
    multiplication PROC
    ;
    ; Calculates and returns the product of two 32-bit floating-point numbers
    ; Receives:
    ;   - First parameter (a) at [esp+4] (32-bit floating-point number)
    ;   - Second parameter (b) at [esp+8] (32-bit floating-point number)
    ; Returns:
    ;   - The product (a * b) stored in EAX as a 32-bit floating-point number
    ; Requires:
    ;   - Inputs should be valid 32-bit floating-point numbers
    ;------------------------------------------------------------------------------------------------------
        fld dword ptr [esp+4]    ; Load first parameter (a) onto the FPU stack (st(0))
        fld dword ptr [esp+8]    ; Load second parameter (b) onto the FPU stack (st(0)), push (a) to st(1)
        fmulp st(1), st(0)       ; Multiply st(0) (b) by st(1) (a), store result in st(1), pop st(0), result in st(0)
        fst dword ptr [esp-4]    ; Store the result from st(0) into memory at [esp-4]
        mov eax, [esp-4]         ; Move the result from memory into EAX register for return
        ret                      ; Return with the result in EAX
    multiplication ENDP

    ;------------------------------------------------------------------------------------------------------
    division PROC
    ;
    ; Calculates and returns the quotient of two 32-bit floating-point numbers
    ; Receives:
    ;   - Dividend (a) at [esp+8] (32-bit floating-point number)
    ;   - Divisor (b) at [esp+4] (32-bit floating-point number)
    ; Returns:
    ;   - The quotient (a / b) stored in EAX as a 32-bit floating-point number
    ; Requires:
    ;   - Inputs should be valid 32-bit floating-point numbers
    ;   - Divisor must not be zero; division by zero is undefined
    ;------------------------------------------------------------------------------------------------------
        fld dword ptr [esp+8]    ; Load dividend (a) onto the FPU stack (st(0))
        fld dword ptr [esp+4]    ; Load divisor (b) onto the FPU stack (st(0)), push (a) to st(1)
        fdivp st(1), st(0)       ; Divide st(1) (a) by st(0) (b), store result in st(1), pop st(0), result in st(0)
        fst dword ptr [esp-4]    ; Store the result from st(0) into memory at [esp-4]
        mov eax, [esp-4]         ; Move the result from memory into EAX register for return
        ret                      ; Return with the result in EAX
    division ENDP

    ; =====================================================================================================
    ;                                      Advanced Scientific Operations
    ; =====================================================================================================

    ;------------------------------------------------------------------------------------------------------
    trigSin PROC
    ;
    ; Calculates and returns the sine of an angle in radians
    ; Receives: 
    ;   - Angle in radians (floating-point) passed at [ESP+4]
    ; Returns: 
    ;   - The sine of the angle in radians, stored in EAX as a 32-bit floating-point number
    ; Requires: 
    ;   - Input angle should be in radians
    ;------------------------------------------------------------------------------------------------------
        fld dword ptr [esp+4]    ; Load angle (in radians) onto the FPU stack (st(0))
        fsin                     ; Compute sine of the angle in radians, result in st(0)
        fst dword ptr [esp-4]    ; Store the result from st(0) into memory at [esp-4]
        mov eax, [esp-4]         ; Move the result from memory into EAX register for return
        ret                      ; Return with result in EAX
    trigSin ENDP

    ;------------------------------------------------------------------------------------------------------
    trigCos PROC
    ;
    ; Calculates and returns the cosine of an angle in radians
    ; Receives: 
    ;   - Angle in radians (floating-point) passed at [ESP+4]
    ; Returns: 
    ;   - The cosine of the angle in radians, stored in EAX as a 32-bit floating-point number
    ; Requires: 
    ;   - Input angle should be in radians
    ;------------------------------------------------------------------------------------------------------
        fld dword ptr [esp+4]    ; Load angle (in radians) onto the FPU stack (st(0))
        fcos                     ; Compute cosine of the angle in radians, result in st(0)
        fst dword ptr [esp-4]    ; Store the result from st(0) into memory at [esp-4]
        mov eax, [esp-4]         ; Move the result from memory into EAX register for return
        ret                      ; Return with result in EAX
    trigCos ENDP

    ;------------------------------------------------------------------------------------------------------
    trigTan PROC
    ;
    ; Calculates and returns the tangent of an angle in radians
    ; Receives: 
    ;   - Angle in radians (floating-point) passed at [ESP+4]
    ; Returns: 
    ;   - The tangent of the angle in radians, stored in EAX as a 32-bit floating-point number
    ; Requires: 
    ;   - Input angle should be in radians
    ;   - Undefined results for angles where cosine = 0; ensure input validation to avoid division by zero
    ;------------------------------------------------------------------------------------------------------
        fld dword ptr [esp+4]    ; Load angle (in radians) onto the FPU stack (st(0))
        fptan                    ; Compute tangent of the angle in radians, result in st(0)
        fstp st(0)               ; Discard top of stack (1.0 that fptan pushes), leaving result in st(0)
        fst dword ptr [esp-4]    ; Store the result from st(0) into memory at [esp-4]
        mov eax, [esp-4]         ; Move the result from memory into EAX register for return
        ret                      ; Return with result in EAX
    trigTan ENDP

    ;------------------------------------------------------------------------------------------------------
    exponentiation PROC
    ;
    ; Calculates the exponential function e^x using the formula: e^x = 2^(x * log2(e))
    ; Receives: 
    ;   - Exponent (x) at [esp+4] (32-bit floating-point number)
    ; Returns: 
    ;   - Result of e^x in EAX as a 32-bit floating-point number
    ;------------------------------------------------------------------------------------------------------
        fld dword ptr [esp+4]     ; Load exponent (x) onto the FPU stack
        fldl2e                    ; Load log2(e) constant onto the FPU stack
        fmulp st(1), st(0)        ; Multiply x by log2(e), store result in st(1), pop st(0)
        fld st(0)                 ; Duplicate the value (x * log2(e))
        frndint                   ; Round the value to the nearest integer
        fsub st(1), st(0)         ; Subtract the rounded integer part from the original value (get fractional part)
        fxch st(1)                ; Swap the top two stack values (integer part and fractional part)
        f2xm1                     ; Compute 2^(fractional part) - 1
        fld1                      ; Load 1.0 onto the stack
        faddp st(1), st(0)        ; Add 1.0 to the result of 2^(fractional part) - 1
        fscale                    ; Scale the result by 2^(integer part)
        fstp dword ptr [esp-4]    ; Store the final result (e^x) in memory at [esp-4]
        mov eax, [esp-4]          ; Move the result from memory into EAX register for return
        ret                       ; Return with the result in EAX
    exponentiation ENDP

    ;------------------------------------------------------------------------------------------------------
    performLn PROC
    ;
    ; Calculates the natural logarithm (ln) of a positive number using the formula: ln(x) = log2(x) * ln(2)
    ; Receives: 
    ;   - Input number at [esp+4] (32-bit floating-point number)
    ; Returns: 
    ;   - The natural logarithm of the input number in ST(0)
    ; Requirements: 
    ;   - Input must be a positive 32-bit floating-point number
    ;------------------------------------------------------------------------------------------------------
        fld dword ptr [esp+4]    ; Load the input number (x) onto the FPU stack
        fldln2                   ; Load the constant ln(2) onto the FPU stack
        fxch st(1)               ; Swap ST(0) (ln(2)) and ST(1) (x) to align the operands for fyl2x
        fyl2x                    ; Compute ln(x) as log2(x) * ln(2), result stored in ST(0)
        ret                      ; Return with the result in ST(0)
    performLn ENDP

    ;------------------------------------------------------------------------------------------------------
    squareRoot PROC
    ;
    ; Calculates the square root of a non-negative number using the FPU instruction fsqrt
    ; Receives: 
    ;   - Input number in ST(0) (32-bit floating-point number)
    ; Returns: 
    ;   - The square root of the input number in ST(0)
    ; Requirements: 
    ;   - Input must be a non-negative 32-bit floating-point number
    ;   - Behaviour is undefined for negative inputs
    ;------------------------------------------------------------------------------------------------------
        fsqrt              ; Compute the square root of the number in ST(0), result replaces ST(0)
        ret                ; Return with the result in ST(0)
    squareRoot ENDP

    ;------------------------------------------------------------------------------------------------------
    power PROC
    ;
    ; Raises a base to the power of an exponent using a simple loop
    ; Receives: 
    ;   - Base in EAX (32-bit integer)
    ;   - Exponent in EDX (32-bit integer)
    ; Returns: 
    ;   - Base raised to the power of Exponent in EAX (integer result)
    ; Requires: 
    ;   - Both Base and Exponent must be valid integers
    ;   - Exponent must be non-negative; results are undefined for negative exponents
    ;------------------------------------------------------------------------------------------------------
        mov ecx, eax          ; Save the base in ECX
        mov eax, 1            ; Start with result = 1

        test edx, edx         ; Check if exponent is zero
        jz PowerDone          ; If exponent is zero, return 1

        PowerLoop:
            imul eax, ecx         ; Multiply EAX (result) by ECX (base)

            dec edx               ; Decrease exponent (EDX)
            jnz PowerLoop         ; Repeat the loop until exponent is zero

        PowerDone:
            ret                   ; Return the result in EAX

    power ENDP

    ;------------------------------------------------------------------------------------------------------
    factorial PROC
    ;
    ; Calculates the factorial of a non-negative integer using a loop
    ; Receives: 
    ;   - Integer in EAX (input number)
    ; Returns: 
    ;   - EAX = factorial of the input number
    ; Requires: 
    ;   - If the input is zero, the result is 1 (since 0! = 1)
    ;------------------------------------------------------------------------------------------------------
        mov ecx, eax                ; Copy the input number to ECX (loop counter)
        mov eax, 1                  ; Initialize EAX to 1 (result)

        FactorialLoop:
            test ecx, ecx               ; Check if ECX (counter) is zero
            jz FactorialDone            ; If ECX is zero, exit the loop

            imul eax, ecx               ; Multiply EAX (current result) by ECX (current counter)
            dec ecx                     ; Decrement the counter (ECX)
            jmp FactorialLoop           ; Repeat the loop until ECX reaches zero

        FactorialDone:
            ret                         ; Return the result in EAX

    factorial ENDP

END
