// Author: Kevin Mitra OpenAL
// CMPS 3350
// created on: 9/20/2019

#include "fonts.h"

/* Steps for Debugging code
 * 1. Reproduce the bug before changing code
 * 2. Understand stack traces
 * 3. Write a test case that Reproduces the bug
 * 4. Know your error codes
 * 5. Pair your program
 */
    

extern void postScores(char*);
void creditsKevin(Rect r)
{
    ggprint8b(&r, 16, 0x00004C00, "Kevin Mitra");
}
