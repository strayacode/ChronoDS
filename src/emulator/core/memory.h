#pragma once
#include <emulator/common/types.h>
#include <string>

class Emulator;

class Memory {
public:
    // common between arm9 and arm7
    u8 main_ram[0x400000] = {}; // 4mb
    u8 shared_wram[0x8000] = {}; // 0kb, 16kb or 32kb can be allocated to arm9 or arm7
    // TODO: add gba slot rom and ram later

    // arm9 specific
    u8 instruction_tcm[0x8000] = {}; // 0x00000000-0x00008000 (non-movable 32kb) (mirrorable to 0x01000000)
    u8 data_tcm[0x4000] = {}; // 16kb movable
    u8 palette_ram[0x800] = {}; // 2kb for engine A obj/bg and engine B obj/bg
    u8 bg_a_vram[0x80000] = {}; // 512kb max
    u8 bg_b_vram[0x20000] = {}; // 128kb max
    u8 obj_a_vram[0x40000] = {}; // 256kb max
    u8 obj_b_vram[0x20000] = {}; // 128kb max
    u8 lcdc_vram[0xA4000] = {}; // 656kb max lcdc allocated vram
    u8 oam[0x800] = {}; // 2kb oam for both engine A and B
    
    u8 arm9_bios[0x8000] = {}; // 32kb arm9 bios only 3kb used


    // arm7 specific
    u8 arm7_bios[0x4000] = {}; // 16kb
    u8 arm7_wram[0x10000] = {}; // 64 kb
    // TODO: add wireless communications
    u8 arm7_vram[256 * 1024] = {}; // vram allocated as wram to arm7 max 256kb

    // external to the arm7/arm9 bus
    u8 firmware[256 * 1024] = {}; // built-in serial flash memory

    // memory read handlers
    template <typename T>
    T arm7_read(u32 addr);

    u8 arm7_read_byte(u32 addr);
    u16 arm7_read_halfword(u32 addr);
    u32 arm7_read_word(u32 addr);

  

    template <typename T>
    T arm9_read(u32 addr);

    u8 arm9_read_byte(u32 addr);
    u16 arm9_read_halfword(u32 addr);
    u32 arm9_read_word(u32 addr);

    template <typename T>
    T arm9_read_io(u32 addr);

    template <typename T>
    T arm7_read_io(u32 addr);

    template <typename T>
    void arm9_write_io(u32 addr, T data);

    template <typename T>
    void arm7_write_io(u32 addr, T data);

    // memory write handlers
    template <typename T>
    void arm9_write(u32 addr, T data);

    void arm9_write_byte(u32 addr, u8 data);
    void arm9_write_halfword(u32 addr, u16 data);
    void arm9_write_word(u32 addr, u32 data);


    template <typename T>
    void arm7_write(u32 addr, T data);

    void arm7_write_byte(u32 addr, u8 data);
    void arm7_write_halfword(u32 addr, u16 data);
    void arm7_write_word(u32 addr, u32 data);

    void load_arm9_bios();
    void load_arm7_bios();
    void load_firmware();
    void load_cartridge(std::string rom_path);

    Memory(Emulator *emulator);
private:
    Emulator *emulator;

    
};