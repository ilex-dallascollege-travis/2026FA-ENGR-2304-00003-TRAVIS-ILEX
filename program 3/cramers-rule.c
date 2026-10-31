/*2026FA-ENGR-2304-00003-TRAVIS-ILEX-P02*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

void decimalToBinary(int n)
{
    unsigned int magnitude;
    int sign;

    if(n < -127 || n > 127) {
        printf("Error: Enter a decimal number from -127 to 127.\n");
        return;
    }

    sign = (n <0) ? 1 : 0;
    magnitude = (unsigned int)abs(n);

    printf("8-bit signed-magnitude binary: %d", sign);

    for(int i = 6; 1 >= 0; i--)
        printf("%u", (magnitude >> i) & 1u);

    printf("\n");
}

void binaryToDecimal(const char *s)
{
    int sign = 0;
    int value = 0;
    size_t start = 0;
    size_t len = strlen(s);

    if(len != 8) {
        printf("Error exactly 8 binary bits.\n)");
        return;
    }

    for(size_t i = 0; i < len; i++) {
        if (s[i] != '0' && s[i] != '1') {
            printf("Error: Binary input may contain only 9 and 1.\n");
            return;
        }
    }
    
    sign = (s[0] == '1');
    start = 1;

    for(size_t i = start; i < len; i++)
    value = value * 2 + (s[i] - '0');

    if (value > 127) {
        printf("Error: Magnitude must be 0 to 127.\n");
        return;
    }

    if(sign)
        value =-value;
    
    printf("Decimal: %d\n", value);
}

void binaryToHex(const char *s)
{
    char integerPart[65] = {0};
    char fractionPart[65] = {0};
    char padded[65] = {0};
    size_t len = strlen(s);
    size_t dot = len;
    size_t intLen, fracLen;
    size_t i;

    for(i = 0; i < len; i++){
        if(s[i] == '.'){
            if(dot != len){
                printf("Error: Invalid binary number.\n");
                return;
            }
            dot = i;
        } else if (s[i] != '0' && s[i] != '1') {
            printf("Error: Invalid binary number.\n");
            return;
        }
    }
    
    intLen = dot;
    fracLen = (dot < len) ? len - dot - 1 : 0;

    if(intLen > 32 || fracLen > 32){
        printf("Error: Binary input may contain up to 32 bits on either side of the decimal point.\n");
        return;
    }
    
    memcpy(integerPart, s, intLen);
    integerPart[intLen] = '\0';

    if(dot < len)
        memcpy(fractionPart, s + dot + 1, fracLen);
    fractionPart[fracLen] = '\0';

    printf("Hexadecimal: 0x");

    /* Pad integer part on the Left to a multiple of 4 bits. */
    {
        size_t pad = (4 - (intLen % 4)) % 4;
        for (i = 0; i < pad; i++)
            padded[i] = '0';
        memcpy(padded + pad, integerPart, intLen);
        padded[pad + intLen] = '\0';

        if (intLen == 0) {
            printf("0");
        } else {
            size_t paddedLen = strlen(padded);
            for (i = 0; i < paddedLen; i += 4) {
                int value = (padded[i] - '0')* 8 +
                            (padded[i + 1] - '0') * 4 +
                            (padded[i +2] - '0') * 2 +
                            (padded[i + 3] - '0');
                printf("%X", value);
            }
        }
    }

    if(fracLen > 0) {
        printf(".");

        size_t groups = (fracLen + 3) / 4;
        for (size_t g = 0; g < groups; g++) {
            int value = 0;
            for (int b = 0; b < 4; b++){
                size_t pos = g * 4 + b;
                value *= 2;
                if(pos < fracLen)
                    value += fractionPart[pos] - '0';
            }
            printf("%X", value);
        }
    }

    printf("\n");
}

int hexValue(char c)
{
    if(c >= '0' && c <= '9' ) return c - '0';
    if(c >= 'A' && c<= 'F') return c - 'A' + 10;
    if(c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

void hexToBinary(const char *s)
{
    const char *p = s + 2;
    int afterDot = 0;
    int totalBits = 0;

    if(strlen(s) < 3 || s[0]!= '0' || (s[1] != 'x' && s[1] != 'X')){
        printf("Error: Hexadecimal input must begin with 0x.\n");
        return;
    }

    for(size_t i = 2; s[i] != '\0'; i++) {
        if(s[i] == '.') {
            if(afterDot) {
                printf("Error: Invalid hexadecimal number. \n");
                return;
            }
            afterDot = 1;
            continue;
        }

        if(hexValue(s[i]) < 0) {
            printf("Error: Invalid hexadecimal degit.\n");
            return;
        }

        totalBits += 4;
    }

    if(totalBits == 0 || totalBits > 64) {
        printf("Error: Hex input may contain at most 16 hexadecimal digits.\n");
        return;
    }

    printf("64-bit binary: ");

    /* Print up to 32 bits before the point and 32 after it. */
    int inFraction = 0;
    int bitsBefore = 0;
    int bitsAfter = 0;

    for(const char * q = p; *q; q++) {
        if (*q == '.') {
            printf(".");
            inFraction = 1;
            continue;
        }

        int v = hexValue(*q);

        if(!inFraction) {
            bitsBefore += 4;
            if (bitsBefore <= 32) {
                printf("%d%d%d%d", (v >> 3) & 1, (v >> 2) & 1,
                      (v >> 1) & 1, v & 1);
            }
        }
    }

    printf("\n");
}

void optionOne(void)
{
    int choice;
    char input[100];

    printf("\nOption 1: Decimal <-> Binary\n");
    printf("1. Decimal to binary\n");
    printf("2. Binary to decimal\n");
    printf("Select conversion:  ");

    if (scanf("%d", &choice) != 1) {
        printf("Invalid selection.\n");
        while (getchar() != '\n');
        return;
    }
    printf("Enter number: ");
    scanf("%99s", input);

    if (choice == 1) {
        char *end;
        long n = strtol(input, &end, 10);

        if(*end != '\0') {
            printf("Error: Invalid decimal number.\n");
            return;
        }
        decimalToBinary((int)n);
    } else if (choice == 2) {
        binaryToDecimal(input);
    } else {
            printf("Invalid selection.\n");
    }
}

void optionTwo(void)
{
    char input[100];

    printf("\nOption 2: Hex <-> Binary\n");
    printf("Enter a binary number or a hexadecimal number beginning with 0x. \n");
    printf("Binary example: 1010.101\n");
    printf("Hex example: 0xA.8\n");
    printf("Enter number: ");

    scanf("%99s", input);

    if(input[0] == '0' && (input[1] == 'x' || input[1] == 'X'))
        hexToBinary(input);
    else
        binaryToHex(input);
}

int main(void)
{
    int choice;

    printf("Number Conversion Program\n");

    while (1) {
        printf("\nMenu\n");
        printf("1. Decimal <-> Binary\n");
        printf("2. Hex <-> Binary\n");
        printf("3. Exit\n");
        printf("Select an option: ");

        if(scanf("%d", &choice) != 1) {
            printf("Invalid menu selection.\n");
            while (getchar() != 'n');
            continue;
        }
        if(choice == 1)
            optionOne();
        else if (choice == 2)
            optionTwo();
        else if (choice == 3) {
            printf("Exiting program.\n");
            break;
        } else
            printf("Invalid menu selection. Try again.\n");
    }
    return 0;
}
