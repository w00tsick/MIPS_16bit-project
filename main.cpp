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

//Functions
int InstructionMemory(int pc) {
	return instructions[pc];
}

//separate function for writing?
//write_flag global variable or argument?
void RegisterFile(int Rs, int Rt, int write_reg, int write_flag, int write_data) {
	if (write_flag == 0){	
		//convert rs and rt into integers
		Rs = reg_to_int(Rs);
		Rt = reg_to_int(Rt);
		
		//outputs be global vars
		reg_out1 = registers[Rs];
		reg_out2 = registers[Rt];
	}
	else {
		registers[write_reg] = write_data;
	}
	return;
}

int DataMemory(int reg_input, int write_data, int write_flag, int read_flag){
	if (read_flag == 1) {
		output = memory[reg_input];
	}
	else if (write_flag == 1) {
		memory[reg_input] = write_data; 
	}
	
	return output;
}

int MUX(int input0, int input1, int control){
	if (control == 0)
		return input0;
	else 
		return input1;
}

//Buffer structs
struct IFID {
	int instruct;  //make this a string?
	**int IFID_write_flag;
}

struct IDEX {
	int WB_flag;
	int M_flag;
	int EX_flag;
	int reg_out1;
	int reg_out2;
	int IFID_rs;
	int IFID_rt;
	int IFID_rd;
}

struct EXMEM {
	int WB_flag;
	int M_flag;
	int ALU_out;
	int ALU_mux_out;
}

struct MEMWB {
	int WB_flag;
	int data_mem_out;
	
}


//Global Variables
int PC, reg_out1, reg_out2;
int MAX_INSTRUCTIONS = 1000;
int MAX_MEMORY = 1024;
int instructions[MAX_INSTRUCTIONS];   //array of strings might be easier
int registers[8];
int memory[MAX_MEMORY];

using namespace std;

/*
 * 
 */

//Global variables
int PC = 0x0000;
int instruct_count = 0;

int main() {
    int clock = 0;
    while(PC<instruct_count){
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
    instr = InstructionMemory(PC);
}

void decode(){
    opcode = IFID.instruction[0:3];
	Control(opcode);
	
	Rs = IFID.instruction[3:5];
	Rt = IFID.instruction[6:8];
	Rd = IFID.instruction[9:11];
	
	RegisterFile(Rs, Rt, 0, 0, 0);
	
}

void execute(){
    if(/*mem.sig -> jump = 1*/)
        J_instruct(mem.opCode, mem.address);
    else{
        if(mem.sig->ALUSrc = 0){
            R_instruct(mem.opCode, rs, rt);
        }else if(mem.sig-> ALUSrc = 1){
            I_instruct(mem.opCode, rs, rt, mem.address);
        }
    }
}

void memAccess(){
    memory_output = DataMemory(EXMEM.ALU_out, EXMEM.ALU_mux_out, write_flag, read_flag);
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


