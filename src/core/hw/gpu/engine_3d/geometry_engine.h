#pragma once

#include <common/types.h>
#include <common/log.h>
#include <common/matrix.h>
#include <queue>
#include <functional>

struct GPU;

struct Entry {
    u8 command;
    u32 parameter;
};

// notes:
// packed commands allows 4 commands per gxfifo write
// unpacked commands only allows 1 command per gxfifo write
// after a command is sent we can read in a 32 bit parameter

// the pipe will be used when the gxfifo is empty

// queueing commands into the fifo:
// we can also write to addresses 0x04000440...0x040005FF
// to queue geometry commands into the fifo
// each write will pass a parameter for a command
// e.g. N writes for N parameters and 1 write for no parameters
// to add the entry to the fifo


// TODO: use a lut for the number of parameters for different commands

struct GeometryEngine {
    GeometryEngine(GPU* gpu);
    void Reset();
    void QueueCommand(u32 addr, u32 data);
    void InterpretCommand();
    void QueueEntry(Entry entry);
    auto DequeueEntry() -> Entry;
    void CommandSetMatrixMode();
    void CommandPopCurrentMatrix();
    void CommandLoadUnitMatrix();

    std::queue<Entry> fifo;
    std::queue<Entry> pipe;

    u32 GXSTAT;
    u32 RAM_COUNT;
    u16 DISP_1DOT_DEPTH;
    u8 POS_RESULT[0x10];
    u8 VEC_RESULT[0x06];
    u8 CLIPMTX_RESULT[0x40];
    u8 VECMTX_RESULT[0x24];

    u8 matrix_mode;


    Matrix projection_current;
    Matrix projection_stack;
    Matrix coordinate_current;
    Matrix coordinate_stack[31];
    Matrix directional_current;
    Matrix directional_stack[31];
    // not sure about this but will look into later
    Matrix texture_current;

    u8 projection_pointer;
    u8 coordinate_pointer;

    GPU* gpu;

    std::function<void()> InterpretCommandTask = std::bind(&GeometryEngine::InterpretCommand, this);
};