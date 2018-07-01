
#include "chip-8/chip.h"

#include <cassert>
#include <memory>
#include <iterator>
#include <random>
#include <iostream>

namespace chip8
{

struct Instruction
{
	union
	{
		struct
		{
			uint8_t l8;
			uint8_t r8;
		};
		uint16_t opcode;
	};
};

Chip::Chip()
	:
	V_          (                    ),
	PC_         (kProgramMemoryOffset),
	I_          (0u                  ),
	STACK_      (                    ),
	MEM_        (                    ),
	GFX_        (                    ),
	KEY_        (0u                  ),
	DELAY_TIMER_(0u                  ),
	SOUND_TIMER_(0u                  )
{
	wipe_up_resources();
	load_fontset();
	load_program();
}

Chip::~Chip()
{
}

void Chip::instruction_cycle()
{
	const auto opcode = fetch();
	decode_and_execute(opcode);

	if (DELAY_TIMER_ > 0)
		DELAY_TIMER_--;
}

void Chip::wipe_up_resources()
{
	  V_.fill(0u);
	MEM_.fill(0u);
	GFX_.fill(0u);
}

void Chip::load_fontset()
{
	constexpr auto kFontset = std::array<uint8_t, 80>
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, //0
		0x20, 0x60, 0x20, 0x20, 0x70, //1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
		0x90, 0x90, 0xF0, 0x10, 0x10, //4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
		0xF0, 0x10, 0x20, 0x40, 0x40, //7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
		0xF0, 0x90, 0xF0, 0x90, 0x90, //A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
		0xF0, 0x80, 0x80, 0x80, 0xF0, //C
		0xE0, 0x90, 0x90, 0x90, 0xE0, //D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
		0xF0, 0x80, 0xF0, 0x80, 0x80  //F
	};

	std::uninitialized_copy(std::begin(kFontset),
		                    std::end  (kFontset),
		                    std::begin(MEM_    ) + kFontsetMemoryOffset);
}

void Chip::load_program()
{
	constexpr auto kPong = std::array<uint8_t, 295>
	{
		0x22, 0xFC, 0x6B, 0x0C, 0x6C,
		0x3F, 0x6D, 0x0C, 0xA2, 0xEA,
		0xDA, 0xB6, 0xDC, 0xD6, 0x6E,
		0x00, 0x22, 0xD4, 0x66, 0x03,
		0x68, 0x02, 0x60, 0x60, 0xF0,
		0x15, 0xF0, 0x07, 0x30, 0x00,
		0x12, 0x1A, 0xC7, 0x17, 0x77,
		0x08, 0x69, 0xFF, 0xA2, 0xF0,
		0xD6, 0x71, 0xA2, 0xEA, 0xDA,
		0xB6, 0xDC, 0xD6, 0x60, 0x01,
		0xE0, 0xA1, 0x7B, 0xFE, 0x60,
		0x04, 0xE0, 0xA1, 0x7B, 0x02,
		0x60, 0x1F, 0x8B, 0x02, 0xDA,
		0xB6, 0x60, 0x0C, 0xE0, 0xA1,
		0x7D, 0xFE, 0x60, 0x0D, 0xE0,
		0xA1, 0x7D, 0x02, 0x60, 0x1F,
		0x8D, 0x02, 0xDC, 0xD6, 0xA2,
		0xF0, 0xD6, 0x71, 0x86, 0x84,
		0x87, 0x94, 0x60, 0x3F, 0x86,
		0x02, 0x61, 0x1F, 0x87, 0x12,
		0x46, 0x00, 0x12, 0x78, 0x46,
		0x3F, 0x12, 0x82, 0x47, 0x1F,
		0x69, 0xFF, 0x47, 0x00, 0x69,
		0x01, 0xD6, 0x71, 0x12, 0x2A,
		0x68, 0x02, 0x63, 0x01, 0x80,
		0x70, 0x80, 0xB5, 0x12, 0x8A,
		0x68, 0xFE, 0x63, 0x0A, 0x80,
		0x70, 0x80, 0xD5, 0x3F, 0x01,
		0x12, 0xA2, 0x61, 0x02, 0x80,
		0x15, 0x3F, 0x01, 0x12, 0xBA,
		0x80, 0x15, 0x3F, 0x01, 0x12,
		0xC8, 0x80, 0x15, 0x3F, 0x01,
		0x12, 0xC2, 0x60, 0x20, 0xF0,
		0x18, 0x22, 0xD4, 0x8E, 0x34,
		0x22, 0xD4, 0x66, 0x3E, 0x33,
		0x01, 0x66, 0x03, 0x68, 0xFE,
		0x33, 0x01, 0x68, 0x02, 0x12,
		0x16, 0x79, 0xFF, 0x49, 0xFE,
		0x69, 0xFF, 0x12, 0xC8, 0x79,
		0x01, 0x49, 0x02, 0x69, 0x01,
		0x60, 0x04, 0xF0, 0x18, 0x76,
		0x01, 0x46, 0x40, 0x76, 0xFE,
		0x12, 0x6C, 0xA2, 0xF2, 0xFE,
		0x33, 0xF2, 0x65, 0xF1, 0x29,
		0x64, 0x14, 0x65, 0x02, 0xD4,
		0x55, 0x74, 0x15, 0xF2, 0x29,
		0xD4, 0x55, 0x00, 0xEE, 0x80,
		0x80, 0x80, 0x80, 0x80, 0x80,
		0x80, 0x00, 0x00, 0x00, 0x00,
		0x00, 0xC0, 0xC0, 0xC0, 0x00,
		0xFF, 0x00, 0x6B, 0x20, 0x6C,
		0x00, 0xA2, 0xF6, 0xDB, 0xC4,
		0x7C, 0x04, 0x3C, 0x20, 0x13,
		0x02, 0x6A, 0x00, 0x6B, 0x00,
		0x6C, 0x1F, 0xA2, 0xFA, 0xDA,
		0xB1, 0xDA, 0xC1, 0x7A, 0x08,
		0x3A, 0x40, 0x13, 0x12, 0xA2,
		0xF6, 0x6A, 0x00, 0x6B, 0x20,
		0xDB, 0xA1, 0x00, 0xEE, 0x00
	};
	
	std::uninitialized_copy(std::begin(kPong),
		                    std::end  (kPong),
		                    std::begin(MEM_ ) + kProgramMemoryOffset);
}

uint16_t Chip::fetch()
{
	return Instruction { MEM_[PC_ + 1], MEM_[PC_ + 0] }.opcode;
}

void Chip::decode_and_execute(uint16_t opcode)
{

#define X   ((opcode & 0x0F00) >> 8)
#define Y   ((opcode & 0x00F0) >> 4)
#define N   ((opcode & 0x000F) >> 0)
#define NN  ((opcode & 0x00FF) >> 0)
#define NNN ((opcode & 0x0FFF) >> 0)

	switch (opcode & 0xF000)
	{
	case 0x0000:
		switch (opcode & 0x000F)
		{
		case 0x0000:
			// Clears the screen.
			GFX_.fill(0);
			V_[0xF] = 1;
			PC_ += sizeof(opcode);
			break;

		case 0x000E:
			// Returns from a subroutine.
			PC_ = STACK_.top();
			STACK_.pop();
			PC_ += sizeof(opcode);
			break;

		default:
			assert(false && "Unknown opcode!!!");
		}
		break;

	case 0x1000:
		// Jumps to address NNN.
		PC_ = NNN;
		break;

	case 0x2000:
		// Calls subroutine at NNN.
		STACK_.push(PC_);
		PC_ = NNN;
		break;

	case 0x3000:
		// Skips the next instruction if VX equals NN.
		// (Usually the next instruction is a jump to skip a code block)
		PC_ += (V_[X] == NN) ? sizeof(opcode) * 2 : sizeof(opcode);
		break;
			
	case 0x4000:
		// Skips the next instruction if VX doesn't equal NN.
		// (Usually the next instruction is a jump to skip a code block)
		PC_ += (V_[X] != NN) ? sizeof(opcode) * 2 : sizeof(opcode);
		break;

	case 0x5000:
		// Skips the next instruction if VX equals VY.
		// (Usually the next instruction is a jump to skip a code block)
		PC_ += (V_[X] == V_[Y]) ? sizeof(opcode) * 2 : sizeof(opcode);
		break;

	case 0x6000:
		// Sets VX to NN.
		V_[X] = NN;
		PC_ += sizeof(opcode);
		break;

	case 0x7000:
		// Adds NN to VX. (Carry flag is not changed)
		V_[X] += NN;
		PC_ += sizeof(opcode);
		break;

	case 0x8000:
		switch (opcode & 0x00F)
		{
		case 0x000:
			// Sets VX to the value of VY.
			V_[X] = V_[Y];
			PC_ += sizeof(opcode);
			break;

		case 0x001:
			// Sets VX to VX or VY. (Bitwise OR operation)
			V_[X] |= V_[Y];
			PC_ += sizeof(opcode);
			break;

		case 0x002:
			// Sets VX to VX and VY. (Bitwise AND operation)
			V_[X] &= V_[Y];
			PC_ += sizeof(opcode);
			break;

		case 0x003:
			// Sets VX to VX xor VY.
			V_[X] ^= V_[Y];
			PC_ += sizeof(opcode);
			break;

		case 0x004:
			// Adds VY to VX.
			// VF is set to 1 when there's a carry, and to 0 when there isn't.
			V_[0XF] = (V_[X] > 0xFF - V_[Y]) ? 1 : 0;
			V_[X] += V_[Y];
			PC_ += sizeof(opcode);
			break;

		case 0x005:
			// VY is subtracted from VX.
			// VF is set to 0 when there's a borrow, and 1 when there isn't.
			V_[0XF] = (V_[X] > V_[Y]) ? 1 : 0;
			V_[X] -= V_[Y];
			PC_ += sizeof(opcode);
			break;

		case 0x006:
			// Shifts VY right by one and stores the result to VX(VY remains unchanged).
			// VF is set to the value of the least significant bit of VY before the shift.
			V_[0xF] = V_[Y] & 0x1; // TODO CHECK
			V_[X] = V_[Y] >> 1;
			PC_ += sizeof(opcode);
			break;

		case 0x007:
			// Sets VX to VY minus VX.
			// VF is set to 0 when there's a borrow, and 1 when there isn't.
			V_[0xF] = (V_[Y] > V_[X]) ? 1 : 0;
			V_[X] = V_[Y] - V_[X];
			PC_ += sizeof(opcode);
			break;

		case 0x00E:
			// Shifts VY left by one and copies the result to VX.
			// VF is set to the value of the most significant bit of VY before the shift.
			V_[0xF] = V_[Y] & 0x1;
			V_[X] = V_[Y] = V_[Y] << 1;
			PC_ += sizeof(opcode);

		default:
			assert(false && "Unknown opcode!!!");
			break;
		}
		break;

	case 0x9000:
		// Skips the next instruction if VX doesn't equal VY.
		// (Usually the next instruction is a jump to skip a code block)
		PC_ += (V_[X] != V_[Y]) ? sizeof(opcode) + sizeof(opcode) : sizeof(opcode);
		break;

	case 0xA000:
		// Sets I to the address NNN.
		I_ = NNN;
		PC_ += sizeof(opcode);
		break;

	case 0xB000:
		// Jumps to the address NNN plus V0.
		PC_ = NNN + V_[0x0];
		break;

	case 0xC000:
		// Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
		V_[X] = (std::random_device()() % 255) & NN;
		PC_ += sizeof(opcode);
		break;

	case 0xD000:
		// Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
		// Each row of 8 pixels is read as bit-coded starting from memory location I;
		// I value doesn’t change after the execution of this instruction.
		// As described above, VF is set to 1 if any screen pixels are flipped from set to unset
		// when the sprite is drawn, and to 0 if that doesn’t happen
		V_[0xF] = 0;
		for (auto h = 0u; h != N; ++h)
		{
			for (auto shift = 0; shift != 8; ++shift)
			{
				auto x = V_[X] + 7 - shift;
				auto y = V_[Y] + h;

				auto before = GFX_[y * kScreenWidth + x];
				GFX_[y * kScreenWidth + x] ^= MEM_[I_ + h] & (0x1 << shift);

				if (0 == before && 1 == GFX_[y * kScreenWidth + x])
				{
					V_[0xF] = 1;
				}
			}
		}
		PC_ += sizeof(opcode);
		break;

	case 0xE000:
		switch (opcode & 0x00FF)
		{
		case 0x009E:
			// Skips the next instruction if the key stored in VX is pressed.
			// (Usually the next instruction is a jump to skip a code block)
			PC_ += (KEY_ == V_[X]) ? sizeof(opcode) + sizeof(opcode) : sizeof(opcode);
			break;

		case 0x00A1:
			// Skips the next instruction if the key stored in VX isn't pressed.
			// (Usually the next instruction is a jump to skip a code block)
			PC_ += (KEY_ != V_[X]) ? sizeof(opcode) + sizeof(opcode) : sizeof(opcode);
			break;

		default:
			assert(false && "Unknown opcode!!!");
		}
		break;

	case 0xF000:
		switch (opcode & 0x00FF)
		{
		case 0x0007:
			// Sets VX to the value of the delay timer.
			V_[X] = DELAY_TIMER_;
			PC_ += sizeof(opcode);
			break;

		case 0x000A:
			// A key press is awaited, and then stored in VX.
			// (Blocking Operation. All instruction halted until next key event)
			std::cin >> KEY_;
			V_[X] = KEY_;
			PC_ += sizeof(opcode);
			break;

		case 0x0015:
			// Sets the delay timer to VX.
			DELAY_TIMER_ = V_[X];
			PC_ += sizeof(opcode);
			break;

		case 0x0018:
			// Sets the sound timer to VX.
			SOUND_TIMER_ = V_[X];
			PC_ += sizeof(opcode);
			break;

		case 0x001E:
			// Adds VX to I.
			V_[X] += I_;
			PC_ += sizeof(opcode);
			break;

		case 0x0029:
			// Sets I to the location of the sprite for the character in VX.
			// Characters 0-F (in hexadecimal) are represented by a 4x5 font.
			I_ = V_[X] + kFontsetMemoryOffset;
			PC_ += sizeof(opcode);
			break;

		case 0x0033:
			// Stores the binary - coded decimal representation of VX,
			// with the most significant of three digits at the address in I,
			// the middle digit at I plus 1, and the least significant digit at I plus 2. 
			// (In other words, take the decimal representation of VX, 
			// place the hundreds digit in memory at location in I, 
			// the tens digit at location I + 1, and the ones digit at location I + 2.)
			MEM_[I_ + 0] = V_[X] / 100;
			MEM_[I_ + 1] = V_[X] / 10 % 10;
			MEM_[I_ + 2] = V_[X] % 100 % 10;
			PC_ += sizeof(opcode);
			break;

		case 0x0055:
			// Stores V0 to VX (including VX) in memory starting at address I.
			// I is increased by 1 for each value written.
			for (auto i = 0x0; i != X; ++i)
			{
				MEM_[I_++] = V_[i];
			}
			PC_ += sizeof(opcode);
			break;

		case 0x0065:
			// Fills V0 to VX (including VX) with values from memory starting at address I.
			// I is increased by 1 for each value written.
			for (auto i = 0x0; i != X; ++i)
			{
				V_[i] = MEM_[I_++];
			}
			PC_ += sizeof(opcode);
			break;

		default:
			assert(false && "Unknown opcode!!!");
		}
		break;

	default:
		assert(false && "Unknown opcode!!!");
	}

#undef NNN
#undef NN
#undef N
#undef Y
#undef X

}

}