/* 
 * File:   main.cpp
 * Author: Owner
 *
 * Created on March 31, 2015, 11:43 AM
 */

#include<iostream>
#include<fstream>
#include<string.h>
#include <stdlib.h>
#include "RegProp.h"
#include "ControlUnit.h"

using namespace std;

/*
 * 
 */

//Global variables
int Registers[8];
int PC = 0x0000;
int instruct_count = 0;
int Tot_Reg = 8;
int MemoryData[256];
int MAX_INSTRUCTIONS = 1000;
int MAX_MEMORY = 1024;
int instructions[MAX_INSTRUCTIONS];   //array of strings might be easier
int memory[MAX_MEMORY];

//Global Obj
RegProp IF_ID;
RegProp ID_EX;
RegProp MEM;
RegProp WB;

int main() {
    
    Registers[0] = 0x0000 //$zero
    Registers[1] = 0x0000 //$t0
    Registers[2] = 0x0000 //$t1
    Registers[3] = 0x0000 //$t2
    Registers[4] = 0x0000 //$a0
    Registers[5] = 0x0000 //$a1
    Registers[6] = 0x0000 //$v0
    Registers[7] = 0x0000 //$v1
    
    int clock = 0;
    
    while(PC < instruct_count){
        if(clock = 0){
            fetch();
        }
        
        clock++;
        
        if(clock== 1){
            decode();
        }
        
        clock++;
        
        if(clock== 2){
            execute();
        }
        
        if(clock== 3){
            memAccess();
        }
        
        if(clock== 4){
            writeBack();
        }
        
        if(clock > 4){
            fetch();
            decode();
            execute();
            memAccess();
            writeBack();
        }
    }
    return 0;
}

void fetch(){
    
}

void decode(){
    opcode = IF_ID.instruction[0:3];
        ControlUnit(opcode);
        
        Rs = IF_ID.instruction[3:5];
        Rt = IF_ID.instruction[6:8];
        Rd = IF_ID.instruction[9:11];
        
        RegisterFile(Rs, Rt, 0, 0 ,0);
}

void execute(){
    if(MEM.sig -> jump = 1)
        J_instruct(MEM.opCode, MEM.address);
    else{
        if(MEM.sig->ALUSrc = 0){
            R_instruct(MEM.opCode, ID_EX.regOut1, ID_EX.regOut2);
        }else if(MEM.sig-> ALUSrc = 1){
            I_instruct(MEM.opCode, ID_EX.regOut1, ID_EX.regOut2, MEM.address);
        }
    }
}

void memAccess(){
    memory_output = DataMemory(MEM.ALU_out, MEM.ALU_mux_out, write_flag, read_flag);
}

void writeBack(){
    
}

void R_instruct(int OpCode, int rs, int rt){
    int rd;
    //R instruction
    switch(/*some signal variable->ALUOp*/)
    {
        case 1: //ADD
            rd = rs + rt;
            break;
        case 2: // sub
            rd = rs - rt;
            break;
        case 3: //and
            rd = (rs & 0xFFFF) & (rt & 0xFFFF);
            break;
        case 4: //or
            rd = (rs & 0xFFFF) | (rt & 0xFFFF);
            break;
        case 5: //not
            rd = ~(rd & 0xFFFF);
            break;
        case 6: //xor
            rd = (rs & 0xFFFF) ^ (rt & 0xFFFF);
            break;
    }
}

void J_instruct(int OpCode, int address){
    
}

void I_instruct(int OpCode, int rs, int rt, int address){
    
}


