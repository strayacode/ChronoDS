#include <emulator/emulator.h>
#include <stdio.h>
#include <string>

Emulator::Emulator() : arm9(this, 1), arm7(this, 0), memory(this), cartridge(this) {
    
}

void Emulator::reset() {
    running = true;
    memory.load_arm9_bios();
    memory.load_arm7_bios();
    memory.load_firmware();
    arm9.reset();
    printf("arm9 reset\n");
    arm7.reset();
    printf("arm7 reset\n");
}

void Emulator::run(std::string rom_path) {
    cartridge.load_cartridge(rom_path);
    reset();

    while (running) {
        arm9.step();
        arm7.step();
    }
}