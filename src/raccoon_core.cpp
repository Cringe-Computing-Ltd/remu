#include "raccoon_core.h"
#include <stdio.h>

RaccoonCore::RaccoonCore(MemoryMapper* mapper) {
    _mapper = mapper;
}

void RaccoonCore::run(int cycles) {
    for (int i = 0; i < cycles; i++) { cycle(); }
}

void RaccoonCore::cycle() {
    // Run a memory cycle
    memCycle();

    uint32_t alu_op_out;

    uint32_t mult_tmp;
    
    uint16_t tmp;

    uint8_t opcode;
    uint8_t src;
    uint8_t dst;

    uint16_t dst_content;
    uint16_t src_content;

    uint16_t tmp_content;
    bool jmp_cond_ok;


    switch (state) {
        case State::FETCH:
            MEM_ADDR = ip;
            MEM_WE = 0;

            state = IDLE;
            state_after_idle = EXEC;
            break;

        case State::IDLE:
            state = state_after_idle;
            break;

        case State::EXEC:
            opcode = MEM_OUT & 0b111111;
            dst = (MEM_OUT >> 6) & 0b11111;
            src = (MEM_OUT >> 11) & 0b11111;

            dst_content = regs[dst];
            src_content = regs[src];

            opcode_contd = opcode;
            dst_contd = dst;
            src_contd = src;
            
            dst_content_contd = dst_content;
            src_content_contd = src_content;
            
            switch (opcode) {
                case 0b000000:
                    MEM_ADDR = ip + 1;

                    state = IDLE;
                    state_after_idle = CONTD;
                    break;
                    
                case 0b000001:
                    regs[dst] = src_content;

                    ip = ip + 1;
                    state = FETCH;
                    break;
                
                case 0b000010:
                    regs[src] = dst_content;
                    regs[dst] = src_content;

                    ip = ip + 1;
                    state = FETCH;
                    break;
                    
                case 0b000011:
                    MEM_ADDR = src_content;
                    
                    state = IDLE;
                    state_after_idle = CONTD;
                    break;

                case 0b000100:
                    MEM_ADDR = ip + 1;

                    state = IDLE;
                    state_after_idle = CONTD;
                    break;
                    
                case 0b000101:
                    MEM_ADDR = dst_content;
                    MEM_IN = src_content;
                    MEM_WE = 1;
                
                    ip = ip + 1;
                    state = IDLE;
                    state_after_idle = FETCH;
                    break;
                
                case 0b000110:
                    alu_op_out = (uint32_t)dst_content + (uint32_t)src_content;

                    regs[dst] = alu_op_out & 0xFFFF;

                    if ((alu_op_out & 0xFFFF) == 0) {
                        flags |= 0b0001;
                    }
                    else {
                        flags &= 0b1110;
                    }
                    if (alu_op_out & 0b01000000000000000) {
                        flags |= 0b0010;
                    }
                    else {
                        flags &= 0b1101;
                    }
                    if (alu_op_out & 0b10000000000000000) {
                        flags |= 0b0100;
                    }
                    else {
                        flags &= 0b1011;
                    }

                    ip = ip + 1;
                    state = FETCH;
                    break;
                
                case 0b000111:
                    // TODO: May need to sign extend?
                    alu_op_out = (uint32_t)dst_content - (uint32_t)src_content;

                    regs[dst] = alu_op_out & 0xFFFF;

                    if ((alu_op_out & 0xFFFF) == 0) {
                        flags |= 0b0001;
                    }
                    else {
                        flags &= 0b1110;
                    }
                    if (alu_op_out & 0b01000000000000000) {
                        flags |= 0b0010;
                    }
                    else {
                        flags &= 0b1101;
                    }
                    if (alu_op_out & 0b10000000000000000) {
                        flags |= 0b0100;
                    }
                    else {
                        flags &= 0b1011;
                    }

                    ip = ip + 1;
                    state = FETCH;
                    break;
                
                case 0b001000:
                    // TODO: May need to sign extend?
                    alu_op_out = (uint32_t)dst_content - (uint32_t)src_content;

                    if ((alu_op_out & 0xFFFF) == 0) {
                        flags |= 0b0001;
                    }
                    else {
                        flags &= 0b1110;
                    }
                    if (alu_op_out & 0b01000000000000000) {
                        flags |= 0b0010;
                    }
                    else {
                        flags &= 0b1101;
                    }
                    if (alu_op_out & 0b10000000000000000) {
                        flags |= 0b0100;
                    }
                    else {
                        flags &= 0b1011;
                    }

                    ip = ip + 1;
                    state = FETCH;
                    break;
                    
                case 0b001001:
                    mult_tmp = (uint32_t)dst_content * (uint32_t)src_content;

                    regs[dst] = mult_tmp & 0xFFFF;
                    regs[Reg::R3] = (mult_tmp >> 16) & 0xFFFF;

                    flags = 0;

                    ip = ip + 1;
                    state = FETCH;
                    break;

                case 0b001010:
                    tmp = dst_content + 1;

                    regs[dst] = tmp;

                    ip = ip + 1;
                    state = FETCH;
                    break;
                
                case 0b001011:
                    tmp = dst_content - 1;

                    regs[dst] = tmp;

                    if ((tmp & 0xFFFF) == 0) {
                        flags |= 0b0001;
                    }
                    else {
                        flags &= 0b1110;
                    }

                    ip = ip + 1;
                    state = FETCH;
                    break;

                case 0b001100:
                    tmp = dst_content ^ src_content;
                    regs[dst] = tmp;

                    if ((tmp & 0xFFFF) == 0) {
                        flags |= 0b0001;
                    }
                    else {
                        flags &= 0b1110;
                    }
                    if (tmp & 0b01000000000000000) {
                        flags |= 0b0010;
                    }
                    else {
                        flags &= 0b1101;
                    }
                    flags &= 0b1011;

                    ip = ip + 1;
                    state = FETCH;
                    break;
                
                case 0b001101:
                    tmp = dst_content & src_content;
                    regs[dst] = tmp;

                    if ((tmp & 0xFFFF) == 0) {
                        flags |= 0b0001;
                    }
                    else {
                        flags &= 0b1110;
                    }
                    if (tmp & 0b01000000000000000) {
                        flags |= 0b0010;
                    }
                    else {
                        flags &= 0b1101;
                    }
                    flags &= 0b1011;

                    ip = ip + 1;
                    state = FETCH;
                    break;

                case 0b001110:
                    tmp = dst_content | src_content;
                    regs[dst] = tmp;

                    if ((tmp & 0xFFFF) == 0) {
                        flags |= 0b0001;
                    }
                    else {
                        flags &= 0b1110;
                    }
                    if (tmp & 0b01000000000000000) {
                        flags |= 0b0010;
                    }
                    else {
                        flags &= 0b1101;
                    }
                    flags &= 0b1011;

                    ip = ip + 1;
                    state = FETCH;
                    break;
                
                case 0b001111:
                    regs[dst] = dst_content << src;

                    ip = ip + 1;
                    state = FETCH;
                    break;

                case 0b010000:
                    regs[dst] = dst_content >> src;

                    ip = ip + 1;
                    state = FETCH;
                    break;

                case 0b010001:
                    switch (src & 0b1111) {
                        case 0b0000:
                            jmp_cond_ok = 1;
                            break;
                        
                        case 0b1110:
                            jmp_cond_ok = ((flags & 1) != 0);
                            break;
                        
                        case 0b1111:
                            jmp_cond_ok = !((flags & 1) != 0);
                            break;
                        
                        case 0b1000:
                            jmp_cond_ok = !((flags & 0b10) != 0) && !((flags & 1) != 0);
                            break;
                        
                        case 0b1001:
                            jmp_cond_ok = !((flags & 0b10) != 0);
                            break;
                        
                        case 0b0100:
                            jmp_cond_ok = ((flags & 0b10) != 0);
                            break;
                        
                        case 0b0101:
                            jmp_cond_ok = ((flags & 0b10) != 0) || ((flags & 1) != 0);
                            break;
                        
                        case 0b0001:
                            jmp_cond_ok = ((flags & 0b100) != 0);
                            break;
                    }

                    if (jmp_cond_ok == 1) {
                        if (src & 0b10000) {
                            MEM_ADDR = ip + 1;

                            state_after_idle = CONTD;
                            state = IDLE;
                        }
                        else {
                            ip = dst_content;
                            state = FETCH;
                        }
                    }
                    else if (src & 0b10000) {
                        ip = ip + 2;
                        state = FETCH;
                    }
                    else {
                        ip = ip + 1;
                        state = FETCH;
                    }
                    break;
                    
                case 0b010010:
                    MEM_ADDR = ip + 1;
                    regs[Reg::RSP] = regs[Reg::RSP] - 1;

                    state_after_idle = CONTD;
                    state = IDLE;
                    break;

                case 0b010011:
                    MEM_ADDR = regs[Reg::RSP] - 1;
                    MEM_IN = dst_content;
                    MEM_WE = 1;

                    regs[Reg::RSP] = regs[Reg::RSP] - 1;

                    ip = ip + 1;
                    state_after_idle = FETCH;
                    state = IDLE;
                    break;

                case 0b010100:
                    MEM_ADDR = regs[Reg::RSP];

                    regs[Reg::RSP] = regs[Reg::RSP] + 1;
                    
                    state_after_idle = CONTD;
                    state = IDLE;
                    break;
                
                case 0b010101:
                    state = FETCH;
                    break;

                case 0b010110:
                    MEM_ADDR = regs[Reg::RSP];

                    regs[Reg::RSP] = regs[Reg::RSP] + 1;
                    
                    state_after_idle = CONTD;
                    state = IDLE;
                    break;
            }
            break;

        case State::CONTD:
            opcode = opcode_contd;
            dst = dst_contd;
            src = src_contd;
            dst_content = dst_content_contd;
            src_content = src_content_contd;

            switch (opcode) {
                case 0b000000:
                    regs[dst] = MEM_OUT;

                    ip = ip + 2;
                    state = FETCH;
                    break;
                    
                case 0b000011:
                    regs[dst] = MEM_OUT;

                    ip = ip + 1;
                    state = FETCH;
                    break;
                    
                case 0b000100:
                    MEM_ADDR = MEM_OUT;
                    MEM_IN = src_content;
                    MEM_WE = 1;

                    ip = ip + 2;
                    state_after_idle = FETCH;
                    state = IDLE;
                    break;
                
                case 0b010001:
                    ip = MEM_OUT;
                    state = FETCH;
                    break;            

                case 0b010010:
                    MEM_ADDR = regs[Reg::RSP];
                    MEM_IN = MEM_OUT;
                    MEM_WE = 1;

                    ip = ip + 2;
                    state_after_idle = FETCH;
                    state = IDLE;
                    break;

                case 0b010100:
                    regs[dst] = MEM_OUT;

                    ip = ip + 1;
                    state = FETCH;
                    break;

                case 0b010110:
                    ip = MEM_OUT;
                    state = FETCH;
            }
            break;
    }

}

void RaccoonCore::memCycle() {
    if (MEM_WE) {
        _mapper->write(MEM_ADDR, MEM_IN);
    }
    else {
        MEM_OUT = _mapper->read(MEM_ADDR);
    }
}