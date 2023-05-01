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
void swap(char str1, char str2)
{
	char temp = str1;
	str1 = str2;
	str2 = temp;
}

void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap(*(str+start), *(str+end));
        start++;
        end--;
    }
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
//    reverse(str, i);

    return str;
}
#endif

static void outb(uint16_t port, uint8_t value) {
	asm("outb %0,%1"    /* src (port), dst (value) */
        : /* empty */
        : "a" (value), "Nd" (port)
        : "memory");
}


void
__attribute__((noreturn))
__attribute__((section(".start")))
_start(void) {
	const char *p;

    char str[128];
    register long long TSC2;

	for (p = "Guest: Calling rdtsc\n"; *p; ++p)
		outb(0xE9, *p);

    /* RDTSC #1 */
    asm volatile("rdtsc" : "=r"(TSC2) : /* no input */ : "eax", "edx");
    //memset(str, '\0', 128);
    itoa(TSC2, str, 10);

	for (p = "Guest: Result for rdtsc\n---\n"; *p; ++p)
		outb(0xE9, *p);

	for (p = str; *p; ++p)
		outb(0xE9, *p);
	outb(0xE9, '\n');

	for (p = "---\n"; *p; ++p)
		outb(0xE9, *p);

#if 1
	for (p = "Guest: Calling rdtsc #2\n"; *p; ++p)
		outb(0xE9, *p);

    /* RDTSC #2 */
    asm volatile("rdtsc" : "=r"(TSC2) : /* no input */ : "eax", "edx");
    //memset(str, '\0', 128);
    itoa(TSC2, str, 10);


	for (p = "Guest: Result for rdtsc #2\n---\n"; *p; ++p)
		outb(0xE9, *p);

	for (p = str; *p; ++p)
		outb(0xE9, *p);
	outb(0xE9, '\n');

	for (p = "---\n"; *p; ++p)
		outb(0xE9, *p);

#endif

#if 1
	for (p = "Guest: Done halting\n"; *p; ++p)
		outb(0xE9, *p);
#endif

	*(long *) 0x400 = 42;

	for (;;)
		asm("hlt" : /* empty */ : "a" (42) : "memory");
}
