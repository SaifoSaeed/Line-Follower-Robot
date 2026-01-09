#include <stdio.h>
#include <stdint.h>  // Required for uint16_t

int main(){
    uint16_t var = 0x80;

    printf("var = ");
    // %hx is the format specifier for 'short' (16-bit) hex
    printf("0x%hx", var); 
    printf("\n");

    printf("~var = ");
    // CASTING/MASKING IS CRITICAL HERE:
    // Without the cast or mask, integer promotion makes this a 32-bit int
    printf("0x%hx", (uint16_t)~var); 
    printf("\n");

    return 0;
}