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

void ControlUnit::print(){
        cout << "Control Unit Signals: " << endl;
        cout << "ALUOp :" << this->ALUOp << endl;
        cout << "ALUSrc: " << this->ALUSrc << endl;
        cout<<"branch :" << this->branch<< endl;
        cout<<"jump : " << this->jump<< endl;
	cout<<"memRead : " << this->memRead<< endl;
	cout<<"memToReg : " << this->MemToReg << endl;
	cout<<"memWrite : " << this->memWrite << endl;
	cout<<"regDest : " << this->regDest << endl;
	cout<<"regWrite : " << this->regWrite << endl;
}

ControlUnit::~ControlUnit(void) {
    
}

