#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>



/**
 * Library-level functions.
 * You should use them in the main sections.
 */

uint64_t convertToUint64 (double number) {
    return *((uint64_t *)(&number));
}

bool getBit (const uint64_t number, const uint8_t index) {
    uint64_t mask = 1ull << index;
    return (mask & number) == mask;
}


/**
 * Checkers here:
 */

const uint64_t exponent_mask = 0x7FF0000000000000; // max exponent value
// 0 11111111111 00000000 ... 00000000  (0b11111111111 = 2047)
const uint64_t fraction_mask = 0x000FFFFFFFFFFFFF; // max fraction value
// 0 00000000000 11111111 ... 11111111

bool checkForNormal (uint64_t number) {
    uint64_t exponent = number & exponent_mask;
    return (exponent != 0) && (exponent != exponent_mask);
}

bool checkForDenormal (uint64_t number) {
    uint64_t exponent = number & exponent_mask;
    uint64_t fraction = number & fraction_mask;
    return (exponent == 0) && (fraction != 0);
}

bool checkForPlus (uint64_t number) {
    return getBit(number, 63);
}

bool checkForPlusZero (uint64_t number) {
    return number == 0x0000000000000000;
}

bool checkForMinusZero (uint64_t number) {
    return number == 0x8000000000000000;
    // 1 00000000000 00000000 ... 00000000
}

bool checkForPlusInf (uint64_t number) {
    return number == 0x7FF0000000000000;
    // 0 11111111111 00000000 ... 00000000 (exponent_mask)
}

bool checkForMinusInf (uint64_t number) {
    return number == 0xFFF0000000000000;
    // 1 11111111111 00000000 ... 00000000
}

bool checkForPlusNormal (uint64_t number) {
    return checkForNormal(number) && checkForPlus(number);
}

bool checkForMinusNormal (uint64_t number) {
    return checkForNormal(number) && !checkForPlus(number);
}

bool checkForPlusDenormal (uint64_t number) {
    return checkForDenormal(number) && checkForPlus(number);
}

bool checkForMinusDenormal (uint64_t number) {
    return checkForDenormal(number) && !checkForPlus(number);
}

bool checkForSignalingNan (uint64_t number) {
    uint64_t expbit = number & 0x7FF8000000000000; 
    // exponent + most significant bit of the significand
    // 0 11111111111 10000000 ... 00000000
    uint64_t signbits = number & 0x0007FFFFFFFFFFFF;
    // all bits of the significand except most significant bit
    // 0 00000000000 01111111 ... 11111111
    return (expbit == exponent_mask) && (signbits > 0);
    // s 11111111111 0[0101110 ... 0110011] sNaN
}

bool checkForQuietNan (uint64_t number) {
    return (number & 0x7FF8000000000000) == 0x7FF8000000000000;
    // s 11111111111 1[xxxxxxx ... xxxxxxxx] qNaN
}


void classify (double number) {
    if (checkForPlusZero(convertToUint64(number))) {
        printf("Plus Zero\n");
    }

    else if (checkForMinusZero(convertToUint64(number))) {
        printf("Minus Zero\n");
    }

    else if (checkForPlusInf(convertToUint64(number))) {
        printf("Plus Inf\n");
    }

    else if (checkForMinusInf(convertToUint64(number))) {
        printf("Minus Inf\n");
    }

    else if (checkForPlusNormal(convertToUint64(number))) {
        printf("Plus Normal\n");
    }

    else if (checkForMinusNormal(convertToUint64(number))) {
        printf("Minus Normal\n");
    }

    else if (checkForPlusDenormal(convertToUint64(number))) {
        printf("Plus Denormal\n");
    }

    else if (checkForMinusDenormal(convertToUint64(number))) {
        printf("Minus Denormal\n");
    }

    else if (checkForSignalingNan(convertToUint64(number))) {
        printf("Signaling NaN\n");
    }

    else if (checkForQuietNan(convertToUint64(number))) {
        printf("Quiet NaN\n");
    }

    else {
        printf("Error.\n");
    }
}