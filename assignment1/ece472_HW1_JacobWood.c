// Jacob Wood
// ECE 472
// 10/11/2016
// assignment 1

#include <stdio.h>
#include <string.h>

#define cpuid(a, b, c, d) __asm__("cpuid" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "a"(a), "b"(b), "c"(c), "d"(d))


// ----------------------------------------------------------------------------------------
// -------------------------------   bin_data   -------------------------------------------
//
// This function is responsible for extracting the desired CPU information from a given
// register
// 
// Parameters:
// val references the register holding the saught after information
// base corresponds to the low end of the bit pattern range
// end corresponds to the high end of the bit pattern range
//

#define bin_data(val, base, end) ((val << (__WORDSIZE-end-1)) >> (__WORDSIZE-end+base-1))

// ----------------------------------------------------------------------------------------



int main(int argc, char **argv) {

	// Initialize register variables
	unsigned long eax;
	unsigned long ebx;
	unsigned long ecx;
	unsigned long edx;
	unsigned long val;


	// GenuineIntel
	cpuid(0, 0, 0, 0);
	printf("identification: \"%.4s%.4s%.4s\"\n", (char *)&ebx, (char *)&edx, (char *)&ecx);

	// L1 data cache size
	// Set ecx = 1 to return L1 data cache information
	cpuid(0x04, 0, 1, 0);
	// Necessary arithmetic for retrieving cache size
	val = (bin_data(ebx, 22, 31) + 1) * (bin_data(ebx, 12, 21) + 1) * (bin_data(ebx, 0, 11) + 1) * (bin_data(ecx, 0, 31) + 1);
	printf("L1 Data Cache Size: %ld\n\n", val);


	// L1 instruction cache size
	// Set ecx = 2 to retrieve instruction cache size
	cpuid(0x04, 0, 2, 0);
	// Necessary arithmetic for retrieving cache size
	val = (bin_data(ebx, 22, 31) + 1) * (bin_data(ebx, 12, 21) + 1) * (bin_data(ebx, 0, 11) + 1) * (bin_data(ecx, 0, 31) + 1);
	printf("L1 Instruction Cache Size: %ld\n\n", val); 


	// TLB information
	cpuid(0x02, 0, 0, 0);
	printf("TLB Information:\n");
	printf("EAX:\t%x\t%x\t%x\t%x\n", bin_data(eax, 24, 31), bin_data(eax, 16, 23), bin_data(eax, 8, 15), bin_data(eax, 0, 7));
	printf("EBX:\t%x\t%x\t%x\t%x\n", bin_data(ebx, 24, 31), bin_data(ebx, 16, 23), bin_data(ebx, 8, 15), bin_data(ebx, 0, 7));
	printf("ECX:\t%x\t%x\t%x\t%x\n", bin_data(ecx, 24, 31), bin_data(ecx, 16, 23), bin_data(ecx, 8, 15), bin_data(ecx, 0, 7));
	printf("EDX:\t%x\t%x\t%x\t%x\n", bin_data(edx, 24, 31), bin_data(edx, 16, 23), bin_data(edx, 8, 15), bin_data(edx, 0, 7));
	printf("\n");

	// Memory hierarchy information
	cpuid(0, 0, 0, 0);


	// Physical address width
	cpuid(0x80000008, 0, 0, 0);
	printf("Physical Address Width: %ldbits\n\n", bin_data(eax, 0,  7));

	// Logical address width
	cpuid(0x80000008, 0, 0, 0);
	printf("Logical Address Width: %ldbits\n\n", bin_data(eax, 8, 15));

	// Number of logical CPUs
	cpuid(0x0B, 0, 0, 0);
	printf("Number of Logical CPUs: %ld\n\n", bin_data(ebx, 0, 15));

	// CPU family
	cpuid(0x01, 0, 0, 0);
	printf("CPU Family ID: %ld\n\n", bin_data(eax, 8, 11));

	// CPU model
	cpuid(0x01, 0, 0, 0);
	printf("CPU Model ID: %ld\n\n", (bin_data(eax, 16, 19) << 4) + bin_data(eax, 4, 7));

	// CPU frequency
	cpuid(0x16, 0, 0, 0);
	printf("Processor Base Frequency: %ldMhz\n", bin_data(eax, 0, 15));
	printf("Maximum Frequency: %ldMhz\n", bin_data(ebx, 0, 15));
	printf("Bus Frequency: %ldMhz\n\n", bin_data(ecx, 0, 15)); 

	// CPU features available
	printf("CPU Features; Available if result is '1', Unavailable otherwise\n");
	cpuid(0x80000001, 0, 0, 0);
	printf("LAHF/SAHF available in 64-bit mode: %ld\n", bin_data(ecx, 0, 0));
	printf("LZCNT: %ld\n", bin_data(ecx, 5, 5));
	printf("PREFETCHW: %ld\n", bin_data(ecx, 8, 8));
	printf("SYSCALL/SYSRET: %ld\n", bin_data(edx, 11, 11));
	printf("Execute Disable Bit Available: %ld\n", bin_data(edx, 20, 20));
	printf("1-GByte Pages Available: %ld\n", bin_data(edx, 26, 26));
	printf("RDTSCP and IA32_TSC_AUX Available: %ld\n", bin_data(edx, 27, 27));
	printf("Intel 64 Architecture Available: %ld\n", bin_data(edx, 29, 29));


}
