#include "CPU.h"

void CPU::Exec(unsigned char* bytesToExec, int dwLen)
{
	bool bFirstRun = true; // Used so we do not increment ip/p on first time around

	for (this->ip = 0; this->ip < dwLen;)
	{
		if (this->bShouldHalt)
			return;

		this->i = (this->r[CS] * 16) + this->ip;

		if (!this->bChangedAddr && !bFirstRun)
		{
			this->i++;
			this->ip++;
		}
		else
			this->bChangedAddr = false;



		int dwOpcode = bytesToExec[this->i] >> 5; // Grabs the opcode from the bits 7, 6 & 5
		int dwMod = (bytesToExec[this->i] >> 4) & 1; // Gets whether the mod option is set from bit 4
		int dwOp1 = (bytesToExec[this->i] & 15); // Grabs operand 1 from bits 3, 2, 1 & 0
		int dwOp2Optional = bytesToExec[this->i + 1]; // Reads the next byte in memory which could be the 2nd operand if used.

		if (dwMod == 0)
		{
			switch (dwOpcode)
			{
			case CPU::JMP: // 0x0
				printf("jmp r%d\n", dwOp1);
				this->ip = this->r[dwOp1];
				this->bChangedAddr = true;

				break;
			case CPU::MOVR:
				printf("movr r%d, r%d\n", dwOp1, dwOp2Optional);
				this->r[dwOp1] = this->r[dwOp2Optional];

				this->ip += 1; // Uses 2nd operand
				break;
			case CPU::MOVM:
				printf("movm r%d, [ds:r%d]\n", dwOp1, dwOp2Optional);
				this->r[dwOp1] = bytesToExec[((this->r[DS]) * 16) + this->r[dwOp2Optional]];

				this->ip += 1; // Uses 2nd operand
				break;
			case CPU::ADD:
				printf("add r%d, r%d\n", dwOp1, dwOp2Optional);
				this->r[dwOp1] += this->r[dwOp2Optional];

				this->ip += 1; // Uses 2nd operand
				break;
			case CPU::XOR:
				printf("xor r%d, r%d\n", dwOp1, dwOp2Optional);
				this->r[dwOp1] ^= this->r[dwOp2Optional];

				this->ip += 1; // Uses 2nd operand
				break;
			case CPU::CMP:
				printf("cmp r%d, r%d\n", dwOp1, dwOp2Optional);

				if (this->r[dwOp1] == this->r[dwOp2Optional])
					this->fl = 0x0;
				else if (this->r[dwOp1] < this->r[dwOp2Optional])
					this->fl = 0xFF;
				else if (this->r[dwOp1] > this->r[dwOp2Optional])
					this->fl = 0x1;

				this->ip += 1; // Uses 2nd operand
				break;
			case CPU::JMPE:
				printf("jmpe r%d\n", dwOp1);

				if (this->fl == 0x0)
				{
					this->ip = this->r[dwOp1];
					this->bChangedAddr = true;
				}

				break;
			case CPU::HLT:
				printf("HLT\n");
				this->HLT_CPU();

				break;
			}
		}
		else if (dwMod == 1)
		{
			switch (dwOpcode)
			{
			case CPU::JMP: // 0x0
				printf("jmp 0x%x:r%d\n", dwOp2Optional, dwOp1);
				this->r[CS] = dwOp2Optional;
				this->ip = this->r[dwOp1];
				this->bChangedAddr = true;

				break;
			case CPU::MOVR:
				printf("movr r%d, #0x%x\n", dwOp1, dwOp2Optional);
				this->r[dwOp1] = dwOp2Optional;

				this->ip += 1; // Uses 2nd operand
				break;
			case CPU::MOVM:
				printf("movm [ds:r%d], r%d\n", dwOp1, dwOp2Optional);
				bytesToExec[((this->r[DS]) * 16) + this->r[dwOp1]] = this->r[dwOp2Optional];

				this->ip += 1; // Uses 2nd operand
				break;
			case CPU::ADD:
				printf("add r%d, #0x%x\n", dwOp1, dwOp2Optional);
				this->r[dwOp1] += dwOp2Optional;

				this->ip += 1; // Uses 2nd operand
				break;
			case CPU::XOR:
				printf("xor r%d, #0x%x\n", dwOp1, dwOp2Optional);
				this->r[dwOp1] ^= dwOp2Optional;

				this->ip += 1; // Uses 2nd operand
				break;
			case CPU::CMP:
				printf("cmp r%d, #0x%x\n", dwOp1, dwOp2Optional);
				printf("%x\n", this->r[dwOp1]);

				if (this->r[dwOp1] == dwOp2Optional)
					this->fl = 0x0;
				else if (this->r[dwOp1] < dwOp2Optional)
					this->fl = 0xFF;
				else if (this->r[dwOp1] > dwOp2Optional)
					this->fl = 0x1;

				this->ip += 1; // Uses 2nd operand
				break;
			case CPU::JMPE:
				printf("jmpe r%d:r%d\n", dwOp2Optional, dwOp1);

				if (this->fl == 0x0)
				{
					this->r[CS] = dwOp2Optional;
					this->ip = (dwOp2Optional * 16) + this->r[dwOp1];
					this->bChangedAddr = true;
				}
				else
					this->ip += 1;

				break;
			case CPU::HLT:
				printf("HLT\n");
				this->HLT_CPU();

				break;
			}
		}

		bFirstRun = false;
	}
}