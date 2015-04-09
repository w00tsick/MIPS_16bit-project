/* 
 * File:   ControlUnit.cpp
 * Author: Owner
 * 
 * Created on March 31, 2015, 12:30 PM
 */
#include <cstdlib>
#include <string.h>
#include <iostream>
#include "ControlUnit.h"

#define ADDI	(0)
#define ADD     (1)
#define SUB	(10)
#define AND	(11)
#define OR	(100)
#define NOT	(101)
#define XOR	(110)
#define LW	(111)
#define SW	(1000)
#define LUI	(1001)
#define SLL	(1010)
#define SRL	(1011)
#define JMP	(1100)
#define BEQ	(1101)
#define BNE	(1110)

using namespace std;

//control Sig initial
ControlUnit::ControlUnit(long opcode){
    this-> ALUOp = 0;
    this-> ALUSrc = 0;
    this-> branch = 0;
    this-> jump = 0;
    this-> memRead = 0;
    this-> MemToReg = 0;
    this-> memWrite = 0;
    this-> regDest = 0;
    this-> regWrite = 0;
}

ControlUnit::ControlUnit(){
    this-> ALUOp = 0;
    this-> ALUSrc = 0;
    this-> branch = 0;
    this-> jump = 0;
    this-> memRead = 0;
    this-> MemToReg = 0;
    this-> memWrite = 0;
    this-> regDest = 0;
    this-> regWrite = 0;
}

void ControlUnit::R_exec(int OpCode){
	switch (OpCode)
	{
		case ADD:
			this->ALUOp = 1;
			this->ALUSrc = 0;
			this->branch = 0;
			this->jump = 0;
			this->memRead = 0;
			this->MemToReg = 0;
			this->memWrite = 0;
			this->regDest = 1;
			this->regWrite = 1;
			break;

		case SUB:
			this->ALUOp = 2;
			this->ALUSrc = 0;
			this->branch = 0;
			this->jump = 0;
			this->memRead = 0;
			this->MemToReg = 0;
			this->memWrite = 0;
			this->regDest = 1;
			this->regWrite = 1;
			break;

		case AND:
			this->ALUOp = 3;
			this->ALUSrc = 0;
			this->branch = 0;
			this->jump = 0;
			this->memRead = 0;
			this->MemToReg = 0;
			this->memWrite = 0;
			this->regDest = 1;
			this->regWrite = 1;
			break;

		case OR:
			this->ALUOp = 4;
			this->ALUSrc = 0;
			this->branch = 0;
			this->jump = 0;
			this->memRead = 0;
			this->MemToReg = 0;
			this->memWrite = 0;
			this->regDest = 1;
			this->regWrite = 1;
			break;

		case NOT:
			this->ALUOp = 5;
			this->ALUSrc = 0;
			this->branch = 0;
			this->jump = 0;
			this->memRead = 0;
			this->MemToReg = 0;
			this->memWrite = 0;
			this->regDest = 1;
			this->regWrite = 1;
			break;

		case XOR:
			this->ALUOp = 6;
			this->ALUSrc = 0;
			this->branch = 0;
			this->jump = 0;
			this->memRead = 0;
			this->MemToReg = 0;
			this->memWrite = 0;
			this->regDest = 1;
			this->regWrite = 1;
			break;
	}
}

void ControlUnit::J_exec(int OpCode){
	switch (OpCode)
	{
		case JMP:
			this->ALUOp = 0;
			this->ALUSrc = 2;
			this->branch = 0;
			this->jump = 1;
			this->memRead = 0;
			this->MemToReg = 0;
			this->memWrite = 0;
			this->regDest = 0;
			this->regWrite = 0;
			break;
	}
}

void ControlUnit::I_exec(int OpCode){
	switch (OpCode)
	{
		case LW:
			this->ALUOp = 0;
			this->ALUSrc = 1;
			this->branch = 0;
			this->jump = 0;
			this->memRead = 1;
			this->MemToReg = 1;
			this->memWrite = 0;
			this->regDest = 0;
			this->regWrite = 1;
			break;

		case SW:
			this->ALUOp = 0;
			this->ALUSrc = 1;
			this->branch = 0;
			this->jump = 0;
			this->memRead = 0;
			this->MemToReg = 0;
			this->memWrite = 1;
			this->regDest = 0;
			this->regWrite = 0;
			break;

		case SLL:
			this->ALUOp = 6;
			this->ALUSrc = 1;
			this->branch = 0;
			this->jump = 0;
			this->memRead = 0;
			this->MemToReg = 0;
			this->memWrite = 0;
			this->regDest = 0;
			this->regWrite = 1;
			break;

		case SRL:
			this->ALUOp = 7;
			this->ALUSrc = 1;
			this->branch = 0;
			this->jump = 0;
			this->memRead = 0;
			this->MemToReg = 0;
			this->memWrite = 0;
			this->regDest = 0;
			this->regWrite = 1;
			break;

		case BEQ:
			this->ALUOp = 1;
			this->ALUSrc = 1;
			this->branch = 1;
			this->jump = 0;
			this->memRead = 0;
			this->MemToReg = 0;
			this->memWrite = 0;
			this->regDest = 0;
			this->regWrite = 0;
			break;

		case ADDI:
			this->ALUOp = 0;
			this->ALUSrc = 1;
			this->branch = 0;
			this->jump = 0;
			this->memRead = 0;
			this->MemToReg = 0;
			this->memWrite = 0;
			this->regDest = 0;
			this->regWrite = 1;
			break;
                        
                case BNE:
                    	this->ALUOp = 1;
			this->ALUSrc = 1;
			this->branch = 1;
			this->jump = 0;
			this->memRead = 0;
			this->MemToReg = 0;
			this->memWrite = 0;
			this->regDest = 0;
			this->regWrite = 0;
                        break;
	}
}

void ControlUnit::print(){
        cout << "Control Unit Signals: " << endl;
        cout << "ALUOp :" << this->ALUOp << endl;
        cout << "ALUSrc: " << this->ALUSrc << endl;
        cout<<"branch :" << this->branch<< endl;
        cout<<"jump : " << this->jump<< endl;
	cout<<"memRead : " << this->memRead<< endl;
	cout<<"MemToReg : " << this->MemToReg << endl;
	cout<<"memWrite : " << this->memWrite << endl;
	cout<<"regDest : " << this->regDest << endl;
	cout<<"regWrite : " << this->regWrite << endl;
}

ControlUnit::~ControlUnit(void) {
    
}

