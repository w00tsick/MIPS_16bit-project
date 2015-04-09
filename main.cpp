/* 
 * File:   main.cpp
 * Author: Owner
 *
 * Created on March 31, 2015, 11:43 AM
 */

#include<iostream>
#include<fstream>
#include<string.h>
#include <sstream>
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
string *uniqueInstruct;

//Global Obj
RegProp IF_ID;
RegProp ID_EX;
RegProp MEM;
RegProp WB;

int main() {
    int clock = 0, i =0;
    
    Registers[0] = 0x0000; //$zero
    Registers[1] = 0x0000; //$t0
    Registers[2] = 0x0000; //$t1
    Registers[3] = 0x0000; //$t2
    Registers[4] = 0x0000; //$a0
    Registers[5] = 0x0000; //$a1
    Registers[6] = 0x0000; //$v0
    Registers[7] = 0x0000; //$v1
            
    ifstream input;
    input.open("source.txt");
    string line;
    
    if(input.is_open()){
        while(getline(input, line)){
            ++instruct_count;
        }
        input.close();
    }
    else{
        cout << "File not found";
        exit(0);
    }
    
    input.open("source.txt");
    while(input.good()){
        getline(input, uniqueInstruct[i]);
        i++;
    }
    input.close();
    
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
    //clear the properties
    ID_EX.clear();
    //get the instruction from instruction array
    IF_ID.instruction = uniqueInstruct[PC];
    //increase iteration of PC
    IF_ID.currentPC = PC + 1;
}

void decode(){
    ID_EX = IF_ID;
}

void execute(){
    MEM = ID_EX;
    //J format
    if(MEM.sig -> jump = 1)
        J_instruct(MEM.opCode, MEM.address);
    else{
        //R format
        if(MEM.sig->ALUSrc = 0){
            R_instruct(MEM.opCode, ID_EX.regOut1, ID_EX.regOut2);
        }else if(MEM.sig-> ALUSrc = 1){
            //I format
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
    switch(MEM.sig->ALUOp)
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
    
    MEM.ALUResult = rd;
    
    if(rd == 0){
        MEM.zeroBit = 1;
    }
}

void J_instruct(int OpCode, int address){
    if(address <= 256)
        MEM.jumpValue = address;
    else
        cout << "error, not valid jump address" << endl;
}

void I_instruct(int OpCode, int rs, int rt, int address){
    int r;
    if(MEM.sig->ALUOp == 0){ // add Immediate, and LW, SW
        r = address + rt;
    }
    if(MEM.sig->ALUOp == 6){ // SLL
        r = (rt<<address);
    }
    if(MEM.sig->ALUOp == 7){ // SRL
        r = (rt>>address);
    }
    if(MEM.sig->ALUOp == 1){ // BNE, BEQ
        MEM.branchValue = address;
        r = rs - rt;
    }
    if(r == 0){
        MEM.zeroBit = 1;
        if(OpCode == 14){
            //if the result is 0, they are equal, BNE no branch
            MEM.sig-> branch= 0;
        }
    }
    else{
        if(OpCode == 13)
            //if the result is not 0, BEQ no branch
            MEM.sig->branch = 0;
    }
    MEM.ALUResult = r;
}


