#pragma once
#include <stdint.h>
#include "memory_mapper.h"
#include <map>
#include <string>

#define GET_BIT(val, n)         (((val) >> (n)) & 1)
#define SET_BIT(var, n)         var |= (1 << (n))
#define CLEAR_BIT(var, n)       var &= ~(1 << (n))
#define SET_BIT_TO(var, n, b)   if (b) { SET_BIT(var, n); } else { CLEAR_BIT(var, n); }
#define DOWN_TO(val, a, b)      (((val) >> (b)) & ((1 << ((a-b) + 1)) - 1))

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
        R7,
        R8,
        R9,
        R10,
        R11,
        R12,
        RXT,
        RFL,
        RSP,
        _REG_COUNT
    };

    enum State {
        FETCH, 
        IDLE,
        EXEC,
        CONTD, 
        CONTD2
    };

    void halt(bool halted);
    void interrupt(bool set);

    void run(int cycles);
    void cycle();
    void memCycle();

    uint16_t getRegister(Reg reg);

private:
    MemoryMapper* _mapper;

    uint16_t MEM_ADDR = 0;
    uint16_t MEM_IN = 0;
    uint16_t MEM_OUT = 0;
    bool MEM_WE = 0;
    bool HALT = 0;
    bool INTERRUPT = 0;

    uint16_t DEBUG_OUT = 0;

    State state = State::FETCH;
    State state_after_idle = State::FETCH;
    
    uint8_t opcode_contd = 0;
    uint8_t dst_contd = 0;
    uint8_t src_contd = 0;
    
    uint16_t dst_content_contd = 0;
    uint16_t src_content_contd = 0;

    uint16_t regs[Reg::_REG_COUNT];

    uint16_t ip = 0;

    bool last_interrupt = 0;
    
};

inline std::map<uint8_t, std::string> mnemonics = {
    { 0x00, "ldi"  },
    { 0x01, "mov"  },
    { 0x02, "xch"  },
    { 0x03, "ld"   },
    { 0x17, "ld"   },
    { 0x05, "st"   },
    { 0x04, "st"   },
    { 0x06, "add"  },
    { 0x07, "sub"  },
    { 0x08, "cmp"  },
    { 0x09, "mul"  },
    { 0x0A, "inc"  },
    { 0x0B, "dec"  },
    { 0x0C, "xor"  },
    { 0x0D, "and"  },
    { 0x0E, "or"   },
    { 0x0F, "shl"  },
    { 0x10, "shr"  },
    { 0x11, "jmp"  },
    { 0x11, "jmp"  },
    { 0x13, "push" },
    { 0x12, "push" },
    { 0x14, "pop"  },
    { 0x15, "hlt"  },
    { 0x16, "ret"  },
    { 0x18, "call" },
    { 0x18, "call" },
};