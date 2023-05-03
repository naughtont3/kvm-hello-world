#include <stddef.h>
#include <stdint.h>

#if 1
/*
 *  //long long tmp = 777;
 *  //long long tmp = 925277023586666;
 *  // FIXME: fix reverse in itoa() and truncation for large number
 *  long long tmp   = 1234567898;
 */
/* QUICK copy/paste itoa() - https://www.geeksforgeeks.org/implement-itoa/ */
/* A utility function to reverse a string  */
void reverse(char *str, int length)
{
    int start = 0;
    int end = length -1;

    while (start < end)
    {
	    char tmp1 = *(str+start);
        char tmp2 = *(str+end);

        *(str+end) = tmp1;
        *(str+start) = tmp2;

        start++;
        end--;
    }
    return;
}

/* QUICK copy/paste itoa() - https://www.geeksforgeeks.org/implement-itoa/ */
char* itoa(long long num, char* str, int base)
{
    int i = 0;
    int isNegative = 0;

    /* Handle 0 explicitly, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = 1;
        num = -num;
    }

    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}
#endif

static void outb(uint16_t port, uint8_t value) {
	asm("outb %0,%1"    /* src (port), dst (value) */
        : /* empty */
        : "a" (value), "Nd" (port)
        : "memory");
}

void msg(char *msg)
{
    const char *p = NULL;
	for (p = msg; *p; ++p)
		outb(0xE9, *p);
}

void
__attribute__((noreturn))
__attribute__((section(".start")))
_start(void) {
    char str[128];
    register long long TSC;
#if 0
    /* Test the itoa() with a static value */
    char str2[128];
    register long long FOO;
    FOO = 94065830478992;
    itoa(FOO, str2, 10);
    msg("FOO: \n---\n");
    msg(str2);
    msg("\n---\n");
#endif

    /* RDTSC #1 */
    msg("Guest: Calling rdtsc\n");

    asm volatile("rdtsc" : "=r"(TSC) : /* no input */ : "eax", "edx");

    itoa(TSC, str, 10);

    msg("Guest: Result for rdtsc\n---\n");
    msg(str);
    msg("\n---\n");

#if 0
    /* RDTSC #2 */
    msg("Guest: Calling rdtsc #2\n");

    asm volatile("rdtsc" : "=r"(TSC) : /* no input */ : "eax", "edx");

    itoa(TSC, str, 10);

    msg("Guest: Result for rdtsc #2\n---\n");
    msg(str);
    msg("\n---\n");

#endif

#if 0
    /* RDTSC #3 */
    msg("Guest: Calling rdtsc #3\n");

    asm volatile("rdtsc" : "=r"(TSC) : /* no input */ : "eax", "edx");

    itoa(TSC, str, 10);

    msg("Guest: Result for rdtsc #3\n---\n");
    msg(str);
    msg("\n---\n");

#endif

    msg("Guest: Done halting\n");

	*(long *) 0x400 = 42;
	for (;;)
		asm("hlt" : /* empty */ : "a" (42) : "memory");
}
