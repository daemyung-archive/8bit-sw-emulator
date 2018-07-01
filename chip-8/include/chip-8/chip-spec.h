#ifndef CHIP_SPEC_H
#define CHIP_SPEC_H

namespace chip8
{

constexpr auto kGeneralRegisterCount = 16u;
constexpr auto kStackSize            = 16u;
constexpr auto kDRamSize             = 4096u;
constexpr auto kFontsetMemoryOffset  = 0x50;
constexpr auto kProgramMemoryOffset  = 0x200;
constexpr auto kScreenWidth          = 64u;
constexpr auto kScreenHeight         = 32u;
constexpr auto kVRamSize             = kScreenHeight * kScreenWidth;
constexpr auto kKeyRegisterCount     = 16u;


}  // namespace chip8

#endif // CHIP_SPEC_H