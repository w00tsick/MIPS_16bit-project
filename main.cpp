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
        if (clock == 1) {
            fetch();
            decode();
            execute();
            memAccess();
            clock = 0;
        }
        else {
            writeBack();
            clock = 1;
        }
    }
    return 0;
}

void fetch(){
    
}

void decode(){
    
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


