.text

.global count_det
count_det:
    // 3x3 matrix array + offsets: 
    // [a_11, a_12, a_13, a_21,  a_22,  a_23,  a_31,  a_32,  a_33]
    // [0-3,  4-7,  8-11, 12-15, 16-19, 20-23, 24-27, 28-31, 32-35]
    ldr r1, [r0]       // a_11
    ldr r2, [r0, #16]  // a_22
    mul r1, r2, r1     // a_11*a_22
    ldr r2, [r0, #32]  // a_33
    mul r1, r2, r1     // a_11*a_22*a_33 -> r1
    ldr r2, [r0, #4]   // a_12
    ldr r3, [r0, #20]  // a_23
    mul r2, r3, r2     // a_12*a_23
    ldr r3, [r0, #24]  // a_31
    mla r1, r2, r3, r1 // a_12*a_23*a_31 + r1 -> r1
    ldr r2, [r0, #8]   // a_13
    ldr r3, [r0, #12]  // a_21
    mul r2, r3, r2     // a_13*a_21
    ldr r3, [r0, #28]  // a_32
    mla r1, r2, r3, r1 // a_13*a_21*a_32 + r1 -> r1
    ldr r2, [r0, #8]   // a_13
    ldr r3, [r0, #16]  // a_22
    mul r2, r3, r2     // a_13*a_22
    ldr r3, [r0, #24]  // a_31
    mls r1, r2, r3, r1 // r1 - a_13*a_22*a_31 -> r1
    ldr r2, [r0]       // a_11
    ldr r3, [r0, #20]  // a_23
    mul r2, r3, r2     // a_11*a_23
    ldr r3, [r0, #28]  // a_32
    mls r1, r2, r3, r1 // r1 - a_11*a_23*a_32 -> r1
    ldr r2, [r0, #4]   // a_12
    ldr r3, [r0, #12]  // a_21
    mul r2, r3, r2     // a_12*a_21
    ldr r3, [r0, #32]  // a_33
    mls r1, r2, r3, r1 // r1 - a_12*a_21*a_33 -> r1
    mov r0, r1
    bx lr