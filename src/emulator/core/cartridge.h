#pragma once
#include <emulator/common/types.h>
#include <string>

class Emulator;

class Cartridge {
public:
    Cartridge(Emulator *emulator);
    ~Cartridge();
    
    void load_cartridge(std::string rom_path);

    // transfer data from the rom to main memory
    void direct_boot();

    std::string nds_rom_name;

    struct cartridge_header {
        char game_title[12];
        u32 gamecode; // this is 0 on homebrew
        u16 makercode; // this is 0 on homebrew and 01 on nintendo

        // trying to understand this lol
        u32 arm9_rom_offset; // specifies the offset in rom that data will start to be transferred to main ram
        u32 arm9_entry_address; // not too sure about this one
        u32 arm9_ram_address; // specifies the address in the arm9 bus that data will start to be transferred to
        u32 arm9_size; // specifies the size of data being transferred to main ram
        u32 arm7_rom_offset;
        u32 arm7_entry_address;
        u32 arm7_ram_address;
        u32 arm7_size;
    } header;

private:
    Emulator *emulator;

    // allocate dynamically using malloc() to not use 4gb :joy:
    u8 *rom;


    

    // load data from rom into header struct
    void load_header_data();

    

};