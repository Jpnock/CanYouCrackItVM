#pragma once

#include <iostream>
#include <array>

#define R1 0x00 
#define R2 0x01
#define R3 0x02 
#define R4 0x03

#define CS 0x04
#define DS 0x05

class CPU
{
public:
	int ip = 0x0;
	int i = 0x0;
	
	std::array<int, 6> r; // r0, r1, r2, r3, CS, DS

	int fl = 0x0;

	bool bChangedAddr = false;
	bool bShouldHalt = false;

	void Exec(unsigned char* bytes, int dwLen);

	void HLT_CPU()
	{
		bShouldHalt = true;
		printf("\n\nCPU will now halt! Press enter to end.\n\n");
	}

	enum VM_OPCODES
	{
		JMP = 0x0,
		MOVR,
		MOVM,
		ADD,
		XOR,
		CMP,
		JMPE,
		HLT
	};

	CPU() :
		r ({ { 0, 0, 0, 0, 0, 0x10 } }) // Order: r0, r1, r2, r3, cs, ds	// ds -> set to 0x10
	{
		
	}

};