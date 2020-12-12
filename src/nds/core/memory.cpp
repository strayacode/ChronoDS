#include <nds/core/memory.h>
#include <nds/nds.h>
#include <stdio.h>
#include <string>

static const int ARM9_BIOS_SIZE = 32 * 1024;
static const int ARM7_BIOS_SIZE = 16 * 1024;
static const int FIRMWARE_SIZE = 256 * 1024;

Memory::Memory(NDS *nds) : nds(nds) {

}

u8 Memory::arm7_read_byte(u32 addr) {
    switch (addr & 0xFF000000) {
    case 0x00000000:
        return arm7_bios[addr];
    // case 0x02000000:
    //     return main_ram[addr - 0x02000000];
    // case 0x03000000:
    //     if (addr >= 0x03800000) {
    //         return arm7_wram[addr - 0x03800000];
    //     } else {
    //         return shared_wram[addr - 0x03000000];
    //     }
    // // TODO: add wireless communications
    // case 0x04000000:
    //     arm7_read_io(addr);
    // case 0x06000000:
    //     return arm7_vram[addr - 0x06000000];
    // case 0x07000000:
    //     return arm7_vram[addr - 0x07000000];
    default:
        printf("[Memory] byte read from arm7 at address 0x%04x is unimplemented!\n", addr);
        return 0;
    }
}

u16 Memory::arm7_read_halfword(u32 addr) {
    return (arm7_read_byte(addr + 1) << 8 | arm7_read_byte(addr));
}

u32 Memory::arm7_read_word(u32 addr) {
    return (arm7_read_halfword(addr + 2) << 16 | arm7_read_halfword(addr));
}

u8 Memory::arm9_read_byte(u32 addr) {
    switch (addr & 0xFF000000) {
        case 0x00000000:
            // deal with tcm later lol
        // case 0x02000000:
        //     return main_ram[addr - 0x02000000];
        // case 0x03000000:
        //     return shared_wram[addr - 0x03000000];
        // case 0x04000000:
        //     return arm9_read_io(addr);
        // case 0x05000000:
        //     return palette_ram[addr - 0x05000000];
        // case 0x06000000:
        //     // deal with later lol
        // case 0x07000000:
        //     return oam[addr - 0x07000000];
        case 0xFF000000:
            return arm9_bios[addr - 0xFFFF0000];
        default:
            printf("[Memory] byte read from arm9 at address 0x%04x is unimplemented!\n", addr);
            return 0;

    }
}

u16 Memory::arm9_read_halfword(u32 addr) {
    return (arm9_read_byte(addr + 1) << 8 | arm9_read_byte(addr));
}

u32 Memory::arm9_read_word(u32 addr) {
    return (arm9_read_halfword(addr + 2) << 16 | arm9_read_halfword(addr));
}

u32 Memory::arm7_read_io(u32 addr) {
    switch (addr) {
    default:
        printf("[Memory] io read by arm7 at address 0x%04x is unimplemented!\n", addr);
        return 0;
    }
}

u32 Memory::arm9_read_io(u32 addr) {
    switch (addr) {
    default:
        printf("[Memory] io read by arm9 at address 0x%04x is unimplemented!\n", addr);
        return 0;
    }
}

void Memory::load_arm9_bios() {
    FILE *file_buffer = fopen("../bios/bios9.bin", "rb");
    if (file_buffer == NULL) {
        printf("[Memory] error when opening arm9 bios! make sure the file bios9.bin exists in the bios folder\n");
        nds->running = false;
    }
    fseek(file_buffer, 0, SEEK_END);
    fseek(file_buffer, 0, SEEK_SET);
    fread(arm9_bios, ARM9_BIOS_SIZE, 1, file_buffer);
    fclose(file_buffer);  
    printf("[Memory] arm9 bios loaded successfully!\n");
}

void Memory::load_arm7_bios() {
    FILE *file_buffer = fopen("../bios/bios7.bin", "rb");
    if (file_buffer == NULL) {
        printf("[Memory] error when opening arm7 bios! make sure the file bios7.bin exists in the bios folder\n");
        nds->running = false;
    }
    fseek(file_buffer, 0, SEEK_END);
    fseek(file_buffer, 0, SEEK_SET);
    fread(arm7_bios, ARM7_BIOS_SIZE, 1, file_buffer);
    fclose(file_buffer);  
    printf("[Memory] arm7 bios loaded successfully!\n");
}

void Memory::load_firmware() {
    FILE *file_buffer = fopen("../firmware/firmware.bin", "rb");
    if (file_buffer == NULL) {
        printf("[Memory] error when opening firmware! make sure the file firmware.bin exists in the firmware folder\n");
        nds->running = false;
    }
    fseek(file_buffer, 0, SEEK_END);
    fseek(file_buffer, 0, SEEK_SET);
    fread(firmware, FIRMWARE_SIZE, 1, file_buffer);
    fclose(file_buffer);  
    printf("[Memory] firmware loaded successfully!\n");
}

void Memory::load_cartridge(std::string rom_path) {
    FILE *file_buffer = fopen(rom_path.c_str(), "rb");
    if (file_buffer == NULL) {
        printf("[Memory] error while opening the selected rom! make you sure specify the path correctly\n");
        nds->running = false;
    }
    fseek(file_buffer, 0, SEEK_END);
    fseek(file_buffer, 0, SEEK_SET);
    fread(main_ram, FIRMWARE_SIZE, 1, file_buffer);
    fclose(file_buffer);  
    printf("[Memory] cartridge loaded successfully!\n");
}