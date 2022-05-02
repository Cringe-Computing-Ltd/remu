#pragma once
#include <stdint.h>
#include "memory_mapper.h"

class RaccoonCore {
public:
    RaccoonCore(MemoryMapper* mapper);

    enum Reg {
        R0,
        R1,
        R2,
        R3,
        R4,
        R5,
        R6,
        RSP,
        _REG_COUNT
    };

    enum State {
        FETCH, 
        IDLE,
        EXEC,
        CONTD
    };

    void run(int cycles);
    void cycle();
    void memCycle();

private:
    MemoryMapper* _mapper;

    uint16_t MEM_ADDR = 0;
    uint16_t MEM_IN = 0;
    uint16_t MEM_OUT = 0;
    bool MEM_WE = 0;

    State state = State::FETCH;
    State state_after_idle = State::FETCH;
    
    uint8_t opcode_contd = 0;
    uint8_t dst_contd = 0;
    uint8_t src_contd = 0;
    
    uint16_t dst_content_contd = 0;
    uint16_t src_content_contd = 0;

    uint16_t regs[Reg::_REG_COUNT];

    uint8_t flags = 0;

    uint16_t ip = 0;
    
};