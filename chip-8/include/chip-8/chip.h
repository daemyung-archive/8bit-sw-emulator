#ifndef CHIP_H
#define CHIP_H

#include <cstdint>
#include <array>
#include <stack>
#include <queue>

#include "chip-spec.h"

namespace chip8
{

struct Instruction;

using GeneralRegisters = std::array<uint8_t, kGeneralRegisterCount>;
using Stack            = std::stack<uint32_t>;
using DRam             = std::array<uint8_t, kDRamSize>;
using VRam             = std::array<uint8_t, kVRamSize>;

class Chip
{
	friend struct Instruction;

public:
	Chip ();

	~Chip ();

	void instruction_cycle ();

	auto get_pixel(uint32_t index) const noexcept
	{
		return GFX_[index];
	}

private:
	void wipe_up_resources ();

	void load_fontset ();

	void load_program ();

	uint16_t fetch ();

	void decode_and_execute (uint16_t opcode);

private:
	GeneralRegisters V_;
	uint32_t         PC_;
	uint32_t         I_;
	Stack            STACK_;
	DRam             MEM_;
	VRam             GFX_;
	uint8_t          KEY_;
	uint8_t          DELAY_TIMER_;
	uint8_t          SOUND_TIMER_;
};

}  // namespace chip8

#endif  // CHIP_H