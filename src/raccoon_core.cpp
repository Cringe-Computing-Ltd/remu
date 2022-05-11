#include "raccoon_core.h"
#include <stdio.h>

RaccoonCore::RaccoonCore(MemoryMapper* mapper) {
    _mapper = mapper;
    memset(regs, 0, sizeof(regs));
}

void RaccoonCore::halt(bool halted) {
    HALT = halted;
}

void RaccoonCore::interrupt(bool set) {
    INTERRUPT = set;
}

void RaccoonCore::run(int cycles) {
    for (int i = 0; i < cycles; i++) { cycle(); }
}

void RaccoonCore::cycle() {
    // Run a memory cycle
    memCycle();

    // alu_op_out: contains the last carry bit to set the flag
    uint32_t alu_op_out = 0;
    uint16_t alu_minus = 0;
    // mult_tmp: contains the full result of a multiplication
    uint32_t mult_tmp = 0;
    // tmp
    uint16_t tmp = 0;

    uint8_t opcode = 0;
    uint8_t src = 0;
    uint8_t dst = 0;

    // todo: get rid of these
    uint16_t dst_content = 0;
    uint16_t src_content = 0;

    uint16_t tmp_content = 0;
    bool jmp_cond_ok = 0;

    switch (state) {
        case FETCH:
            if (INTERRUPT == 1 && last_interrupt == 0 && GET_BIT(regs[Reg::RFL], 4) == 1) {
                MEM_ADDR = regs[Reg::RSP] - 1;
                MEM_WE = 1;
                MEM_IN = ip;

                regs[Reg::RSP] = regs[Reg::RSP] - 1;

                ip = 0x0002;

                state_after_idle = FETCH;
                state = IDLE;
            }
            else if (HALT == 0) {
                MEM_ADDR = ip;
                MEM_WE = 0;

                state = IDLE;
                state_after_idle = EXEC;
            }

            last_interrupt = INTERRUPT;
            break;

        case IDLE:
            state = state_after_idle;
            break;

        case EXEC:
            opcode = DOWN_TO(MEM_OUT, 5, 0);
            dst = DOWN_TO(MEM_OUT, 10, 6);
            src = DOWN_TO(MEM_OUT, 15, 11);

            dst_content = regs[dst];
            src_content = regs[src];

            opcode_contd = opcode;
            dst_contd = dst;
            src_contd = src;
            
            dst_content_contd = dst_content;
            src_content_contd = src_content;
            
            switch (opcode) {

                // ld (r): puts [src] into dst
                case 0b000000:
                    MEM_ADDR = src_content;
                    
                    state = IDLE;
                    state_after_idle = CONTD;
                    // FALLTHROUGH CONTD
                    break;

                // ld (i): puts [imm] in dst
                case 0b000001:
                    MEM_ADDR = ip + 1;

                    state_after_idle = CONTD;
                    state = IDLE;
                    break;

                // st (r): store src into [dst]
                case 0b000010:
                    MEM_ADDR = dst_content;
                    MEM_IN = src_content;
                    MEM_WE = 1;
                
                    ip = ip + 1;
                    state = IDLE;
                    state_after_idle = FETCH;
                    break;

                // st (i): store src into [imm]
                case 0b000011:
                    MEM_ADDR = ip + 1;

                    state = IDLE;
                    state_after_idle = CONTD;
                    break;

                // mov: move src into dst
                case 0b000100:
                    regs[dst] = src_content;

                    ip = ip + 1;
                    state = FETCH;
                    break;

                // ldi: put imm into dst
                case 0b000101:
                    MEM_ADDR = ip + 1;

                    state = IDLE;
                    state_after_idle = CONTD;
                    // FALLTHROUGH CONTD
                    break;
                
                
                // xch: exchange dst && src
                case 0b000110:
                    regs[src] = dst_content;
                    regs[dst] = src_content;

                    ip = ip + 1;
                    state = FETCH;
                    break;
                
                // add: puts dst+src into dst
                case 0b000111:
                    alu_op_out = dst_content + src_content;

                    // result
                    regs[dst] = DOWN_TO(alu_op_out, 15, 0);

                    // flags
                    if (DOWN_TO(alu_op_out, 15, 0) == 0x0000) {
                        SET_BIT(regs[Reg::RFL], 0);
                    } else {
                        CLEAR_BIT(regs[Reg::RFL], 0);
                    }

                    SET_BIT_TO(regs[Reg::RFL], 1, GET_BIT(alu_op_out, 15));
                    SET_BIT_TO(regs[Reg::RFL], 2, GET_BIT(alu_op_out, 16));

                    if ((GET_BIT(alu_op_out, 15) == 1 && GET_BIT(dst_content, 15) == 0 && GET_BIT(src_content, 15) == 0)
                            ||
                            (GET_BIT(alu_op_out, 15) == 0 && GET_BIT(dst_content, 15) == 1 && GET_BIT(src_content, 15) == 1)) {
                        SET_BIT(regs[Reg::RFL], 3);
                    } else {
                        CLEAR_BIT(regs[Reg::RFL], 3);
                    }


                    ip = ip + 1;
                    state = FETCH;
                    break;
                
                // sub: put dst-src into dst
                case 0b001000:
                    alu_minus = 0x0000 - src_content;
                    alu_op_out = dst_content - src_content;

                    // result
                    regs[dst] = DOWN_TO(alu_op_out, 15, 0);

                    // flags
                    if (DOWN_TO(alu_op_out, 15, 0) == 0x0000) {
                        SET_BIT(regs[Reg::RFL], 0);
                    } else {
                        CLEAR_BIT(regs[Reg::RFL], 0);
                    }

                    SET_BIT_TO(regs[Reg::RFL], 1, GET_BIT(alu_op_out, 15));
                    SET_BIT_TO(regs[Reg::RFL], 2, GET_BIT(alu_op_out, 16));

                    if ((GET_BIT(alu_op_out, 15) == 1 && GET_BIT(dst_content, 15) == 0 && GET_BIT(alu_minus, 15) == 0)
                            ||
                            (GET_BIT(alu_op_out, 15) == 0 && GET_BIT(dst_content, 15) == 1 && GET_BIT(alu_minus, 15) == 1)) {
                        SET_BIT(regs[Reg::RFL], 3);
                    } else {
                        CLEAR_BIT(regs[Reg::RFL], 3);
                    }

                    ip = ip + 1;
                    state = FETCH;
                    break;
                
                // cmp: compares dst && src
                case 0b001001:
                    alu_minus = 0x0000 - src_content;
                    alu_op_out = dst_content - src_content;

                    if (DOWN_TO(alu_op_out, 15, 0) == 0x0000) {
                        SET_BIT(regs[Reg::RFL], 0);
                    } else {
                        CLEAR_BIT(regs[Reg::RFL], 0);
                    }

                    SET_BIT_TO(regs[Reg::RFL], 1, GET_BIT(alu_op_out, 15));
                    SET_BIT_TO(regs[Reg::RFL], 2, GET_BIT(alu_op_out, 16));

                    if ((GET_BIT(alu_op_out, 15) == 1 && GET_BIT(dst_content, 15) == 0 && GET_BIT(alu_minus, 15) == 0)
                            ||
                            (GET_BIT(alu_op_out, 15) == 0 && GET_BIT(dst_content, 15) == 1 && GET_BIT(alu_minus, 15) == 1)) {
                        SET_BIT(regs[Reg::RFL], 3);
                    } else {
                        CLEAR_BIT(regs[Reg::RFL], 3);
                    }

                    ip = ip + 1;
                    state = FETCH;
                    break;

                // inc: increment dst
                case 0b001010:
                    alu_op_out = dst_content + 0x0001;

                    // result
                    regs[dst] = DOWN_TO(alu_op_out, 15, 0);

                    // flags
                    if (DOWN_TO(alu_op_out, 15, 0) == 0x0000) {
                        SET_BIT(regs[Reg::RFL], 0);
                    } else {
                        CLEAR_BIT(regs[Reg::RFL], 0);
                    }

                    SET_BIT_TO(regs[Reg::RFL], 1, GET_BIT(alu_op_out, 15));
                    SET_BIT_TO(regs[Reg::RFL], 2, GET_BIT(alu_op_out, 16));

                    if ((GET_BIT(alu_op_out, 15) == 1 && GET_BIT(dst_content, 15) == 0 && GET_BIT(src_content, 15) == 0)
                            ||
                            (GET_BIT(alu_op_out, 15) == 0 && GET_BIT(dst_content, 15) == 1 && GET_BIT(src_content, 15) == 1)) {
                        SET_BIT(regs[Reg::RFL], 3);
                    } else {
                        CLEAR_BIT(regs[Reg::RFL], 3);
                    }


                    ip = ip + 1;
                    state = FETCH;
                    break;
                
                // dec: decrement dst
                case 0b001011:
                    alu_minus = 0xFFFF;
                    alu_op_out = dst_content + alu_minus;

                    // result
                    regs[dst] = DOWN_TO(alu_op_out, 15, 0);

                    // flags
                    if (DOWN_TO(alu_op_out, 15, 0) == 0x0000) {
                        SET_BIT(regs[Reg::RFL], 0);
                    } else {
                        CLEAR_BIT(regs[Reg::RFL], 0);
                    }

                    SET_BIT_TO(regs[Reg::RFL], 1, GET_BIT(alu_op_out, 15));
                    SET_BIT_TO(regs[Reg::RFL], 2, GET_BIT(alu_op_out, 16));

                    if ((GET_BIT(alu_op_out, 15) == 1 && GET_BIT(dst_content, 15) == 0 && GET_BIT(alu_minus, 15) == 0)
                            ||
                            (GET_BIT(alu_op_out, 15) == 0 && GET_BIT(dst_content, 15) == 1 && GET_BIT(alu_minus, 15) == 1)) {
                        SET_BIT(regs[Reg::RFL], 3);
                    } else {
                        CLEAR_BIT(regs[Reg::RFL], 3);
                    }

                    ip = ip + 1;
                    state = FETCH;
                    break;

                // mul: put dst*src into d:dst
                case 0b001100:
                    mult_tmp = dst_content*src_content;

                    regs[dst] = DOWN_TO(mult_tmp, 15, 0); 
                    regs[Reg::RXT] = DOWN_TO(mult_tmp, 31, 16);

                    // TODO: fill flags correctly
                    if (mult_tmp == 0x00000000) {
                        SET_BIT(regs[Reg::RFL], 0);
                    } else {
                        CLEAR_BIT(regs[Reg::RFL], 0);
                    }

                    SET_BIT_TO(regs[Reg::RFL], 1, GET_BIT(mult_tmp, 31));

                    if (DOWN_TO(mult_tmp, 31, 16) == 0x0000) {
                        CLEAR_BIT(regs[Reg::RFL], 2);
                    } else {
                        SET_BIT(regs[Reg::RFL], 2);
                    }

                    ip = ip + 1;
                    state = FETCH;
                    break;

                
                // xor: dst == dst ^ b
                case 0b001101:
                    tmp = dst_content ^ src_content;
                    regs[dst] = tmp;

                    if (tmp == 0x0000) {
                        SET_BIT(regs[Reg::RFL], 0);
                    } else {
                        CLEAR_BIT(regs[Reg::RFL], 0);
                    }
                    SET_BIT_TO(regs[Reg::RFL], 1, GET_BIT(tmp, 15));
                    CLEAR_BIT(regs[Reg::RFL], 2);

                    ip = ip + 1;
                    state = FETCH;
                    break;
                
                // and: dst == dst && src
                case 0b001110:
                    tmp = dst_content & src_content;
                    regs[dst] = tmp;

                    if (tmp == 0x0000) {
                        SET_BIT(regs[Reg::RFL], 0);
                    } else {
                        CLEAR_BIT(regs[Reg::RFL], 0);
                    }
                    SET_BIT_TO(regs[Reg::RFL], 1, GET_BIT(tmp, 15));
                    CLEAR_BIT(regs[Reg::RFL], 2);

                    ip = ip + 1;
                    state = FETCH;
                    break;

                // or: dst == dst || src
                case 0b001111:
                    tmp = dst_content | src_content;
                    regs[dst] = tmp;

                    if (tmp == 0x0000) {
                        SET_BIT(regs[Reg::RFL], 0);
                    } else {
                        CLEAR_BIT(regs[Reg::RFL], 0);
                    }
                    SET_BIT_TO(regs[Reg::RFL], 1, GET_BIT(tmp, 15));
                    CLEAR_BIT(regs[Reg::RFL], 2);

                    ip = ip + 1;
                    state = FETCH;
                    break;
                
                // shl: shift left
                case 0b010000: 
                    regs[dst] = dst_content << src;

                    ip = ip + 1;
                    state = FETCH;
                    break;

                // shr: shift right
                case 0b010001:
                    regs[dst] = dst_content >> src;

                    ip = ip + 1;
                    state = FETCH;
                    break;

                // push (r): push dst
                case 0b010010:
                    // write dst_content into [rsp - 1]
                    MEM_ADDR = regs[Reg::RSP] - 1;
                    MEM_IN = dst_content;
                    MEM_WE = 1;

                    // decrement dst
                    regs[Reg::RSP] = regs[Reg::RSP] - 1;

                    ip = ip + 1;
                    state_after_idle = FETCH;
                    state = IDLE;
                    break;

                // push (i): push imm
                case 0b010011:
                    MEM_ADDR = ip + 1;
                    regs[Reg::RSP] = regs[Reg::RSP] - 1;

                    state_after_idle = CONTD;
                    state = IDLE;
                    break;

                // pop: pop to dst
                case 0b010100:
                    MEM_ADDR = regs[Reg::RSP];

                    regs[Reg::RSP] = regs[Reg::RSP] + 1;
                    
                    state_after_idle = CONTD;
                    state = IDLE;
                    break;

                // jmp: jump to dst
                case 0b010110:
                case 0b010111:
                    switch (DOWN_TO(src, 3, 0)) {
                        // bit 0: 0 indicates equality
                        // bit 1: 0 indicates >
                        // bit 2: 0 indicates <
                        // bit 3: 0 indicates unsignedness (no regard on sign)

                        // unconditional
                        case 0b0000:
                            jmp_cond_ok = 1;
                            break;
                        // ==
                        case 0b0110:
                            jmp_cond_ok = GET_BIT(regs[Reg::RFL], 0);
                            break;
                        // !=
                        case 0b0001:
                            jmp_cond_ok = !GET_BIT(regs[Reg::RFL], 0);
                            break;
                        // >
                        case 0b1101:
                            jmp_cond_ok = !(GET_BIT(regs[Reg::RFL], 1) ^ GET_BIT(regs[Reg::RFL], 3)) && !GET_BIT(regs[Reg::RFL], 0);
                            break;
                        // >=
                        case 0b1100:
                            jmp_cond_ok = !(GET_BIT(regs[Reg::RFL], 1) ^ GET_BIT(regs[Reg::RFL], 3));
                            break;
                        // <
                        case 0b1011:
                            jmp_cond_ok = GET_BIT(regs[Reg::RFL], 1) ^ GET_BIT(regs[Reg::RFL], 3);
                            break;
                        // =
                        case 0b1010:
                            jmp_cond_ok = (GET_BIT(regs[Reg::RFL], 1) ^ GET_BIT(regs[Reg::RFL], 3)) || GET_BIT(regs[Reg::RFL], 0);
                            break;
                        // above
                        case 0b0101:
                            jmp_cond_ok = !GET_BIT(regs[Reg::RFL], 2) && !GET_BIT(regs[Reg::RFL], 0);
                            break;
                        // above || equal
                        case 0b0100:
                            jmp_cond_ok = !GET_BIT(regs[Reg::RFL], 2);
                            break;
                        // below / carry set
                        case 0b0011:
                            jmp_cond_ok = GET_BIT(regs[Reg::RFL], 2);
                            break;
                        // below || equal
                        case 0b0010:
                            jmp_cond_ok = GET_BIT(regs[Reg::RFL], 2) || GET_BIT(regs[Reg::RFL], 0);
                            break;
                        default:
                            jmp_cond_ok = 0;
                            break;
                    }


                    if (jmp_cond_ok == 1) {
                        switch ((opcode)) {
                            case 0b010110:
                                ip = dst_content;
                                state = FETCH;
                                break;
                            default:
                                MEM_ADDR = ip + 1;
                                state_after_idle = CONTD;
                                state = IDLE;
                                break;
                        }
                    } else {
                        switch ((opcode)) {
                            case 0b010110:
                                ip = ip + 1;
                                break;
                            default:
                                ip = ip + 2;
                                break;
                        }
                        state = FETCH;
                    }
                    
                    // END jmp
                    break;

                // call: puts ip + 1 on the stack, jumps to location
                case 0b011000:
                case 0b011001:
                    MEM_ADDR = regs[Reg::RSP] - 1;
                    MEM_WE = 1;

                    regs[Reg::RSP] = regs[Reg::RSP] - 1;

                    switch ((opcode)) {
                        case 0b011000:
                            MEM_IN = ip + 1;

                            ip = dst_content;
                            state_after_idle = FETCH;
                            break;
                        default: 
                            MEM_IN = ip + 2;

                            state_after_idle = CONTD;
                            break;
                    }

                    state = IDLE;
                    break;

                // ret: return using IP from stack
                case 0b011010:
                    MEM_ADDR = regs[Reg::RSP];

                    regs[Reg::RSP] = regs[Reg::RSP] + 1;
                    
                    state_after_idle = CONTD;
                    state = IDLE;
                    break;

                // hlt: halt
                case 0b011011:
                    state = FETCH;
                    break;

                default:
                    break;
            }
            break;

        case CONTD:
            // restore the variable
            opcode = opcode_contd;
            dst = dst_contd;
            src = src_contd;
            dst_content = dst_content_contd;
            src_content = src_content_contd;

            switch (opcode) {
                // ld (r): load [src] into dst
                case 0b000000:
                    regs[dst] = MEM_OUT;

                    ip = ip + 1;
                    state = FETCH;
                    break;

                // ld (i): puts [imm] in dst
                case 0b000001:
                    MEM_ADDR = MEM_OUT;

                    state_after_idle = CONTD2;
                    state = IDLE;
                    break;

                // st (i): stores src into [imm]
                case 0b000011:
                    MEM_ADDR = MEM_OUT;
                    MEM_IN = src_content;
                    MEM_WE = 1;

                    ip = ip + 2;
                    state_after_idle = FETCH;
                    state = IDLE;
                    break;

                // ldi: put imm into dst
                case 0b000101:
                    regs[dst] = MEM_OUT;

                    ip = ip + 2;
                    state = FETCH;
                    break;
                
                // psi: push immediate
                case 0b010011:
                    MEM_ADDR = regs[Reg::RSP];
                    MEM_IN = MEM_OUT;
                    MEM_WE = 1;

                    ip = ip + 2;
                    state_after_idle = FETCH;
                    state = IDLE;
                    break;

                // pop: pop to dst
                case 0b010100:
                    regs[dst] = MEM_OUT;

                    ip = ip + 1;
                    state = FETCH;
                    break;
                
                // jmp: contd for immediate
                case 0b010111:
                    ip = MEM_OUT;
                    state = FETCH;    
                    break;           

                // call: puts ip + 1 on the stack, jumps to location
                case 0b011001:
                    // if here, immediate value jump
                    MEM_ADDR = ip + 1;
                    MEM_WE = 0;

                    state_after_idle = CONTD2;
                    state = IDLE;
                    break;

                // ret: return using IP from the stack
                case 0b011010:
                    ip = MEM_OUT;
                    state = FETCH;
                    break;

                default:
                    break;
            }
            break;

        case CONTD2:
            opcode = opcode_contd;
            dst = dst_contd;
            dst_content = dst_content_contd;

            switch (opcode) {
                // ld (i): puts [imm] in dst
                case 0b000001:
                    regs[dst] = MEM_OUT;

                    ip = ip + 2;
                    state = FETCH;
                    break;

                // call: puts ip + 1 on the stack, jumps to location
                case 0b011001:
                    // if here, immediate value jump
                    ip = MEM_OUT;
                    state = FETCH;
                    break;

                default:
                    break;
            }
            break;
    }
}

uint16_t RaccoonCore::getRegister(Reg reg) {
    return regs[reg];
}

void RaccoonCore::memCycle() {
    if (MEM_WE) {
        _mapper->write(MEM_ADDR, MEM_IN);
    } else {
        MEM_OUT = _mapper->read(MEM_ADDR);
    }
}