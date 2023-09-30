#include <stdio.h>
#include "shell.h"


void handle_r_type(uint32_t instruction);
void handle_j_type(uint32_t instruction);
void handle_i_type(uint32_t instruction);
void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */
	uint32_t instruction = 0x0;
	instruction = mem_read_32(CURRENT_STATE.PC);

	uint8_t op = ((instruction >> 26) & 0x3F);
    if(op==0x0){
        handle_r_type(instruction);//op field of  R type instruction is 0
    }else if(op==0x02 || op==0x03){
        handle_j_type(instruction);//op field of J type instruction is 0x02 or 0x03
    }else{
        handle_i_type(instruction);
    }
}


//handle R type instruction
void handle_r_type(uint32_t instruction){
    uint32_t rs = (instruction >> 21) & 0x1F;
    uint32_t rt = ((instruction >> 16) & 0x1F);
    uint32_t rd = ((instruction >> 11) & 0x1F);
    uint32_t shamt = ((instruction >> 6) & 0x1F);
    uint32_t funct = (instruction & 0x3F);

    switch(funct){
        case 0x20: //add
            NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x21: //addu
            NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x22: //sub
            NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x23: //subu
            NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x24: //and
            NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] & CURRENT_STATE.REGS[rt];
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x25: //or
            NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt];
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x26: //xor
            NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] ^ CURRENT_STATE.REGS[rt];
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x27: //nor
            NEXT_STATE.REGS[rd] = ~(CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt]);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;

        case 0x2A: //slt
            NEXT_STATE.REGS[rd] = ((int32_t)CURRENT_STATE.REGS[rs] < (int32_t)CURRENT_STATE.REGS[rt]) ? 1 : 0;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x2B: //sltu
            NEXT_STATE.REGS[rd] = (CURRENT_STATE.REGS[rs] < CURRENT_STATE.REGS[rt]) ? 1 : 0;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;

        case 0x00: //sll
            NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << shamt;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x02: //srl
            NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> shamt;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x03: //sra
            NEXT_STATE.REGS[rd] = (int32_t)CURRENT_STATE.REGS[rt] >> shamt;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x04: //sllv
            NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << CURRENT_STATE.REGS[rs];
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x06: //srlv
            NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> CURRENT_STATE.REGS[rs];
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x07: //srav
            NEXT_STATE.REGS[rd] = (int32_t)CURRENT_STATE.REGS[rt] >> CURRENT_STATE.REGS[rs];
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x08: //jr
            NEXT_STATE.PC = CURRENT_STATE.REGS[rs];
            break;
        case 0x09: //jalr
            NEXT_STATE.REGS[rd] = CURRENT_STATE.PC + 4;//ignore brach delay slot
            NEXT_STATE.PC = CURRENT_STATE.REGS[rs];
            break;

        case 0x10: //mfhi
            NEXT_STATE.REGS[rd] = CURRENT_STATE.HI;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x11: //mthi
            NEXT_STATE.HI = CURRENT_STATE.REGS[rs];
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x12: //mflo
            NEXT_STATE.REGS[rd] = CURRENT_STATE.LO;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x13: //mtlo
            NEXT_STATE.LO = CURRENT_STATE.REGS[rs];
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;

        case 0x18:{ //mult
            int64_t result_mult = (int32_t)CURRENT_STATE.REGS[rs] * (int32_t)CURRENT_STATE.REGS[rt];
            NEXT_STATE.LO = (uint32_t) result_mult;
            NEXT_STATE.HI = (uint32_t) (result_mult >> 32);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}
        case 0x19: {//multu
            uint64_t result_multu = CURRENT_STATE.REGS[rs] * CURRENT_STATE.REGS[rt];
            NEXT_STATE.LO = (uint32_t) result_multu ;
            NEXT_STATE.HI = (uint32_t) (result_multu >> 32);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}
        case 0x1A: //div
            NEXT_STATE.LO = (int32_t)CURRENT_STATE.REGS[rs] / (int32_t)CURRENT_STATE.REGS[rt];
            NEXT_STATE.HI = (int32_t)CURRENT_STATE.REGS[rs] % (int32_t)CURRENT_STATE.REGS[rt];
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x1B: //divu
            NEXT_STATE.LO = CURRENT_STATE.REGS[rs] / CURRENT_STATE.REGS[rt];
            NEXT_STATE.HI = CURRENT_STATE.REGS[rs] % CURRENT_STATE.REGS[rt];
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0xC: //syscall
            if(CURRENT_STATE.REGS[2]==0xA){
                RUN_BIT = 0;
            }
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        };
}

void handle_j_type(uint32_t instruction){
    uint32_t op = (instruction >> 26) & 0x3F;
    uint32_t target = (instruction & 0x03FFFFFF);
    if(op==0x02){ //j
        NEXT_STATE.PC = (CURRENT_STATE.PC & 0xF0000000) +(target << 2);
    }else if(op==0x03){ //jal
        NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;//ignore branch delay slot
        NEXT_STATE.PC = (CURRENT_STATE.PC & 0xF0000000) +(target << 2);
    }
}

void handle_i_type(uint32_t instruction){
    uint32_t op = (instruction >> 26) & 0x3F;
    uint32_t rs = (instruction >> 21) & 0x1F;
    uint32_t rt = ((instruction >> 16) & 0x1F);
    uint32_t imm = (instruction & 0xFFFF);
    int32_t offset = (int16_t)(instruction & 0xFFFF);

    switch(op){
        case 0x01: //bgez, bgezal, bltz, bltzal
            if(rt==0x0 && (int32_t)CURRENT_STATE.REGS[rs] < 0){//bltz
                NEXT_STATE.PC = CURRENT_STATE.PC + (offset << 2)+4;
            }else if(rt==0x0 && (int32_t)CURRENT_STATE.REGS[rs] >= 0){
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;}
            if(rt==0x01 && (int32_t)CURRENT_STATE.REGS[rs] >= 0){//bgez
                NEXT_STATE.PC = CURRENT_STATE.PC + (offset << 2)+4;
            }else if (rt==0x01 && (int32_t)CURRENT_STATE.REGS[rs] < 0){
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            if(rt==0x10 && (int32_t)CURRENT_STATE.REGS[rs] < 0){//bltzal
                NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
                NEXT_STATE.PC = CURRENT_STATE.PC + (offset << 2)+4;
            }else if (rt==0x10 && (int32_t)CURRENT_STATE.REGS[rs] >= 0){
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            if(rt==0x11 && (int32_t)CURRENT_STATE.REGS[rs] >= 0){//bgezal
                NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
                NEXT_STATE.PC = CURRENT_STATE.PC + (offset << 2)+4;
            }else if (rt==0x11 && (int32_t)CURRENT_STATE.REGS[rs] < 0){
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            break;
        case 0x04: //beq
            if(CURRENT_STATE.REGS[rs] == CURRENT_STATE.REGS[rt]){
                NEXT_STATE.PC = CURRENT_STATE.PC + (offset << 2)+4;
            }else{
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            break;
        case 0x05: //bne
            if(CURRENT_STATE.REGS[rs] != CURRENT_STATE.REGS[rt]){
                NEXT_STATE.PC = CURRENT_STATE.PC + (offset << 2)+4;
            }else{
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            break;
        case 0x06: //blez
            if((int32_t)CURRENT_STATE.REGS[rs] <= 0){
                NEXT_STATE.PC = CURRENT_STATE.PC + (offset << 2)+4;
            }else{
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            break;
        case 0x07: //bgtz
            if((int32_t)CURRENT_STATE.REGS[rs] > 0){
                NEXT_STATE.PC = CURRENT_STATE.PC + (offset << 2)+4;
            }else {
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            break;
        case 0x08: //addi
            NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] + imm;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x09: //addiu
            NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] + imm;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x0A: //slti
            NEXT_STATE.REGS[rt] = ((int32_t)CURRENT_STATE.REGS[rs] < (int32_t)imm) ? 1 : 0;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x0B: //sltiu
            NEXT_STATE.REGS[rt] = (CURRENT_STATE.REGS[rs] < imm) ? 1 : 0;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x0C: //andi
            NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] & imm;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x0D: //ori
            NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] | imm;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x0E: //xori
            NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] ^ imm;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x0F: //lui
            NEXT_STATE.REGS[rt] = imm << 16;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        case 0x20: {//lb
            uint32_t address_lb = (int32_t)CURRENT_STATE.REGS[rs] + offset;
            int32_t value_lb = (int8_t)mem_read_32(address_lb);
            NEXT_STATE.REGS[rt] = value_lb;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}
        case 0x21: {//lh
            uint32_t address_lh = (int32_t)CURRENT_STATE.REGS[rs] + offset;
            int32_t value_lh = (int16_t)mem_read_32(address_lh);
            NEXT_STATE.REGS[rt] = value_lh;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}
        case 0x23: {//lw
            uint32_t address_lw = (int32_t)CURRENT_STATE.REGS[rs] + offset;
            NEXT_STATE.REGS[rt] = mem_read_32(address_lw);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}
        case 0x24: {//lbu
            uint32_t address_lbu = (int32_t)CURRENT_STATE.REGS[rs] + offset;
            uint32_t value_lbu = (uint8_t)mem_read_32(address_lbu);
            NEXT_STATE.REGS[rt] = value_lbu;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}
        case 0x25: {//lhu
            uint32_t address_lhu = (int32_t)CURRENT_STATE.REGS[rs] + offset;
            uint32_t value_lhu = (uint16_t)mem_read_32(address_lhu);
            NEXT_STATE.REGS[rt] = value_lhu;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}
        case 0x28: {//sb
            uint32_t address_sb = (int32_t)CURRENT_STATE.REGS[rs] + offset;
            uint32_t pre_value_sb = mem_read_32(address_sb)&0xFFFFFF00;
            uint32_t byte_sb= CURRENT_STATE.REGS[rt] & 0xFF;
            uint32_t value_sb = pre_value_sb | byte_sb;
            mem_write_32(address_sb, value_sb);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}
        case 0x29: {//sh
            uint32_t address_sh = (int32_t)CURRENT_STATE.REGS[rs] + offset;
            uint32_t pre_value_sh = mem_read_32(address_sh)&0xFFFF0000;
            uint32_t halfword_sh= CURRENT_STATE.REGS[rt] & 0xFFFF;
            uint32_t value_sh = pre_value_sh | halfword_sh;
            mem_write_32(address_sh, value_sh);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}
        case 0x2B: {//sw
            uint32_t address_sw = (int32_t)CURRENT_STATE.REGS[rs] + offset;
            mem_write_32(address_sw, CURRENT_STATE.REGS[rt]);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;}
        
    }
}