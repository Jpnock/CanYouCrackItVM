#include <iostream>

#include "CPU.h"
#include "VMMemory.h"

int main()
{
	CPU cpu;
	cpu.Exec(cVMMemory.bytesToExec.data(), cVMMemory.bytesToExec.size());
	
	getchar(); // Allow to pause program after it halts.
	return 0;
}