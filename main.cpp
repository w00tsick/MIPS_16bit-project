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

//define OpCodes
string R_CODE[] = { "0001", "0010", "0011", "0100", "0101", "0110" };
string I_CODE[] = { "0000", "0111", "1000", "1001", "1010", "1011", "1101", "1110" };
string J_CODE[] = { "1100" };

//Global variables
int Registers[8];
int PC = 0x0000;
int instruct_count = 0;
int Tot_Reg = 8;
int MemoryData[256];
string *uniqueInstruct = NULL;
//fetch buffer update vars
string fetchInstr;
int fetchPC = 0;
//decode buffer update vars
//still need controlsig globals
int decodePC = 0;
int decodeOpcode = 0;
int decodeRs = 0;
int decodeRt = 0;
int decodeRd = 0;
int decodeDest = 0;
int decodeAddress = 0;
int decodeRegOut1 = 0;
int decodeRegOut2 = 0;
//execute buffer update vars
int executeALUResult = 0;
int executeZeroBit = 0;
int executeJumpValue = 0;
int executeBranchValue = 0;
int executeWriteData = 0;
int executeDest = 0;
//memAccess buffer update vars
int memAdd = 0;
int memWriteData = 0;
int memoryDest = 0;
int flagW = 0;
int flagR = 0;
int memReadData = 0;

//Signal placeholders
int decodeSigALUOp = 0;
int decodeSigALUSrc = 0;
int decodeSigBranch = 0;
int decodeSigJump = 0;
int decodeSigMemRead = 0;
int decodeSigMemToReg = 0;
int decodeSigMemWrite = 0;
int decodeSigRegDest = 0;
int decodeSigRegWrite = 0;

int executeSigALUOp = 0;
int executeSigALUSrc = 0;
int executeSigBranch = 0;
int executeSigJump = 0;
int executeSigMemRead = 0;
int executeSigMemToReg = 0;
int executeSigMemWrite = 0;
int executeSigRegDest = 0;
int executeSigRegWrite = 0;

int memSigALUOp = 0;
int memSigALUSrc = 0;
int memSigBranch = 0;
int memSigJump = 0;
int memSigMemRead = 0;
int memSigMemToReg = 0;
int memSigMemWrite = 0;
int memSigRegDest = 0;
int memSigRegWrite = 0;

//Global Obj
RegProp IF_ID;
RegProp ID_EX;
RegProp MEM;
RegProp WB;

void fetch();
void decode();
void execute();
void memAccess();
void writeBack();
void updateBuffer();
void J_instruct(int, int);
void I_instruct(int, int, int, int);
void R_instruct(int, int, int);

int main() {
	IF_ID.instruction = "000000000000000";
	IF_ID.instrPC = 0;
	//decode buffer update vars
	ID_EX.opCode = 0;
	ID_EX.regRs = 0;
	ID_EX.regRt = 0;
	ID_EX.regRd = 0;
	ID_EX.destRegister = 0;
	ID_EX.address = 0;
	ID_EX.regOut1 = 0;
	ID_EX.regOut2 = 0;
	ID_EX.sig->ALUOp = 0;
	ID_EX.sig->ALUSrc = 0;
	ID_EX.sig->branch = 0;
	ID_EX.sig->jump = 0;
	ID_EX.sig->memRead = 0;
	ID_EX.sig->MemToReg = 0;
	ID_EX.sig->memWrite = 0;
	ID_EX.sig->regDest = 0;
	ID_EX.sig->regWrite = 0;
	//execute buffer update vars
	MEM.ALUResult = 0;
	MEM.zeroBit = 0;
	MEM.jumpValue = 0;
	MEM.branchValue = 0;
	MEM.sig->branch = 0;
	MEM.sig->jump = 0;
	//memAccess buffer update vars
	WB.address = 0;
	WB.writeData = 0;
	WB.writeFlag = 0;
	WB.readFlag = 0;
	WB.memReadData = 0;
	cout << "entering main \n";
	int clock = 0;

	Registers[0] = 0x0000; //$zero
	Registers[1] = 0x0008; //$t0
	Registers[2] = 0x0008; //$t1
	Registers[3] = 0x0005; //$t2
	Registers[4] = 0x1010; //$a0
	Registers[5] = 0x0F00; //$a1
	Registers[6] = 0x00FF; //$v0
	Registers[7] = 0x00F0; //$v1
	PC = 0;
	for (int m = 0; m < 256; m++){
		MemoryData[m] = 0;
	}
	MemoryData[255] = 10;
	MemoryData[254] = 20;

	ifstream input;
	input.open("source.txt");
	string line;

	if (input.is_open()){
		while (getline(input, line)){
			++instruct_count;
		}
		input.close();
	}
	else{
		cout << "File not found" << endl;
		exit(0);
	}
	cout << "Number of instructions: " << instruct_count << endl;
	uniqueInstruct = new string[instruct_count + 1];

	int i = 0;
	input.open("source.txt");
	while (input.good()){
		getline(input, uniqueInstruct[i]);
		i++;
	}
	input.close();

	cout << "entering while loop" << endl << endl << endl;

	while (PC < instruct_count){
		if (clock == 1) {
			clock = 0;
			fetch();
			decode();
			execute();
			memAccess();
			updateBuffer();
			cout << "******************************" << endl;
			cout << "Memory and Register Values:" << endl;
			for (int m = 0; m < 256; m++){
				if (MemoryData[m] != 0) {
					cout << "Memory Address " << m << "= " << MemoryData[m] << endl;
				}
			}
			for (int z = 0; z < 8; z++){
				cout << "Register " << z << " = " << Registers[z] << endl;
			}
			cout << endl;
		}
		else {
			writeBack();
			clock = 1;
		}
	}
	return 0;
}

void fetch(){
	cout << "******************************" << endl;
	cout << "Entering fetch()" << endl;
	//clear the properties
	//ID_EX.clear();
	
	//increase iteration of PC
	if ((MEM.sig->branch == 1) || (MEM.sig->jump == 1)) {
		PC = MEM.jumpValue; //set pc to new instructions
	}
	//get the instruction from instruction array
	fetchInstr = uniqueInstruct[PC];
	cout << "Instruction " << PC << ": " << fetchInstr << endl;
	PC++;
	fetchPC = PC;
}

void decode(){
	//ID_EX = IF_ID;
	bool I = false;
	bool R = false;
	bool J = false;
	char *pointer; 

	cout << "******************************" << endl;
	cout << "Entering Decode" << endl;
	string opCode = IF_ID.instruction.substr(0, 4);
	cout << "OpCode: " << opCode << endl; 

	//decodePC = IF_ID.instrPC;
	//decodeSigJump = IF_ID.sig->jump;
	for (int i = 0; i < sizeof(R_CODE) / sizeof(R_CODE[0]); i++){
		if (opCode.compare(R_CODE[i]) == 0){
			cout << "R type opCode \n";
			R = true;
			break;
		}
	}

	if (!R){
		for (int i = 0; i < sizeof(I_CODE) / sizeof(I_CODE[0]); i++){
			if (opCode.compare(I_CODE[i]) == 0){
				cout << "I type opCode \n";
				I = true;
				break;
			}
		}
	}

	if (!R && !I){
		for (int i = 0; i < sizeof(J_CODE) / sizeof(J_CODE[0]); i++){
			if (opCode.compare(J_CODE[i]) == 0){
				cout << "J type opCode \n";
				J = true;
				break;
			}
		}
	}

	decodeOpcode = strtol(IF_ID.instruction.substr(0, 4).c_str(), &pointer, 2);

	if (R){
		//R type
		cout << "R type instruction: " << endl;
		//get rs
		decodeRs = strtol(IF_ID.instruction.substr(4, 3).c_str(), &pointer, 2);
		cout << "regRs: " << decodeRs << endl;
		//get rt
		decodeRt = strtol(IF_ID.instruction.substr(7, 3).c_str(), &pointer, 2);
		cout << "regRt: " << decodeRt << endl;
		//get rd
		decodeRd = strtol(IF_ID.instruction.substr(10, 3).c_str(), &pointer, 2);
		cout << "regRd: " << decodeRd << endl;
		//set destination register
		decodeDest = decodeRd;
		//set signal to R signal
		IF_ID.sig->R_exec(decodeOpcode);
	}

	else if (I){
		//I type
		cout << "I type instruction: " << endl;
		//get rs
		decodeRs = strtol(IF_ID.instruction.substr(4, 3).c_str(), &pointer, 2);
		cout << "regRs: " << decodeRs << endl;
		//get rt	
		decodeRt = strtol(IF_ID.instruction.substr(7, 3).c_str(), &pointer, 2);
		cout << "regRt: " << decodeRt << endl;
		//get address for constant
		decodeAddress = strtol(IF_ID.instruction.substr(10, 6).c_str(), &pointer, 2);
		cout << "address: " << decodeAddress << endl;
		//set destination register
		decodeDest = decodeRt;
		//set signal to I
		IF_ID.sig->I_exec(decodeOpcode);
	}

	else if (J){
		//J type
		cout << "J type instruction: " << endl;
		decodeAddress = strtol(IF_ID.instruction.substr(4, 12).c_str(), &pointer, 2);
		IF_ID.sig->J_exec(decodeOpcode);
	}
	else{
		//Error
		cout << "THIS ADDRESS != R|I|J" << endl;
	}
	//save signals for updating buffer
	decodeSigALUOp = IF_ID.sig->ALUOp;
	decodeSigALUSrc = IF_ID.sig->ALUSrc;
	decodeSigBranch = IF_ID.sig->branch;
	decodeSigJump = IF_ID.sig->jump;
	decodeSigMemRead = IF_ID.sig->memRead;
	decodeSigMemToReg = IF_ID.sig->MemToReg;
	decodeSigMemWrite = IF_ID.sig->memWrite;
	decodeSigRegDest = IF_ID.sig->regDest;
	decodeSigRegWrite = IF_ID.sig->regWrite;

	decodeRegOut1 = Registers[decodeRs];
	decodeRegOut2 = Registers[decodeRt];
	cout << "Register File Output, Rs:" << decodeRegOut1 << ", Rt:" << decodeRegOut2 << endl;
}

void execute(){
	cout << "******************************" << endl;
	cout << "entering execute(): " << endl;
	//MEM = ID_EX;
	//J format
	//executeSig = ID_EX.sig;
	//executeSigJump = ID_EX.sig->jump;
	executeWriteData = ID_EX.regOut2;
	executeSigALUOp = ID_EX.sig->ALUOp;
	executeSigALUSrc = ID_EX.sig->ALUSrc;
	executeSigBranch = ID_EX.sig->branch;
	executeSigJump = ID_EX.sig->jump;
	executeSigMemRead = ID_EX.sig->memRead;
	executeSigMemToReg = ID_EX.sig->MemToReg;
	executeSigMemWrite = ID_EX.sig->memWrite;
	executeSigRegDest = ID_EX.sig->regDest;
	executeSigRegWrite = ID_EX.sig->regWrite;

	//regDest mux
	if (ID_EX.sig->regDest == 0){
		executeDest = ID_EX.regRt;
	}
	else {
		executeDest = ID_EX.regRd;
	}

	if (ID_EX.sig->jump == 1){
		//cout << "execute J instruction: " << endl;
		J_instruct(ID_EX.opCode, ID_EX.address);
	}
	else{
		//R format
		if (ID_EX.sig->ALUSrc == 0){
			//cout << "execute R instruction: " << endl;
			R_instruct(ID_EX.opCode, ID_EX.regOut1, ID_EX.regOut2);
		}
		else if (ID_EX.sig->ALUSrc == 1){
			//I format
			//cout << "Executing I format: " << endl;
			I_instruct(ID_EX.opCode, ID_EX.regOut1, ID_EX.regOut2, ID_EX.address);
		}
		else{
			cout << "Execute Error!" << endl;
		}
	}

}

void memAccess(){

	//WB = MEM;
	//memorySig = MEM.sig;
	cout << "******************************" << endl;
	cout << "Entering Memory Access" << endl;
	memAdd = MEM.ALUResult;
	memWriteData = MEM.writeData;
	cout << "memAdd: " << memAdd << endl;
	cout << "memWriteData: " << memWriteData << endl;

	flagW = MEM.writeFlag;
	flagR = MEM.readFlag;
	memoryDest = MEM.destRegister;
	memSigALUOp = MEM.sig->ALUOp;
	memSigALUSrc = MEM.sig->ALUSrc;
	memSigBranch = MEM.sig->branch;
	memSigJump = MEM.sig->jump;
	memSigMemRead = MEM.sig->memRead;
	memSigMemToReg = MEM.sig->MemToReg;
	memSigMemWrite = MEM.sig->memWrite;
	memSigRegDest = MEM.sig->regDest;
	memSigRegWrite = MEM.sig->regWrite;

	if (MEM.sig->memRead == 1){
		if (MEM.ALUResult > 255){
			cout << "ERROR: Read address exceeds memory" << endl;
		}
		else {
			cout << "Memory Address for read: " << MEM.ALUResult << endl;
			memReadData = MemoryData[MEM.ALUResult];
			cout << "Writeback Read Data: " << memReadData << endl;
		}
	}

	if (MEM.sig->memWrite == 1){
		if (MEM.ALUResult > 255){
			cout << "ERROR: Write address exceeds memory" << endl;
		}
		else {
			MemoryData[MEM.ALUResult] = memWriteData;
			cout << "Memory Data contents: " << MemoryData[MEM.ALUResult] << endl;
		}
	}

	return;
}

void writeBack(){
	cout << "******************************" << endl;
	cout << "Entering WriteBack: " << endl;
	int mux0, mux1, wData, wAddress;
	mux0 = WB.memReadData;
	mux1 = WB.address;
	wData = WB.writeData;

	//writeBack mux
	if (WB.sig->MemToReg == 1){
		wData = mux0;
	}
	else {
		wData = mux1;
	}
	cout << "Writing to memory: " << endl;
	if (WB.sig->regWrite == 1){
		Registers[WB.destRegister] = wData;
		cout << "Register contents: " << Registers[WB.destRegister] << endl;
	}
	return;
}

void R_instruct(int OpCode, int rs, int rt){
	int rd = 0;
	//R instruction
	switch (ID_EX.sig->ALUOp)
	{
	case 1: //ADD
		rd = rs + rt;
		cout << "Executing Add: Rs(" << rs << ") + Rt(" << rt << ") =" << rd << endl;
		break;
	case 2: // sub
		rd = rs - rt;
		cout << "Executing Sub: Rs(" << rs << ") - Rt(" << rt << ") =" << rd << endl;
		break;
	case 3: //and
		rd = (rs & 0xFFFF) & (rt & 0xFFFF);
		cout << "Executing And: Rs(" << rs << ") && Rt(" << rt << ") =" << rd << endl;
		break;
	case 4: //or
		rd = (rs & 0xFFFF) | (rt & 0xFFFF);
		cout << "Executing Or: Rs(" << rs << ") || Rt(" << rt << ") =" << rd << endl;
		break;
	case 5: //not
		cout << "Executing Not: !Rs(" << rs << ") =";
		rd = ~(rs & 0xFFFF);
		cout << rd << endl;
		break;
	case 6: //xor
		rd = (rs & 0xFFFF) ^ (rt & 0xFFFF);
		cout << "Executing Xor: Rs(" << rs << ") Xor Rt(" << rt << ") =" << rd << endl;
		break;
	}
	//cout << "R_rd: " << rd << endl;
	executeALUResult = rd;

	if (rd == 0){
		executeZeroBit = 1;
	}
}

void J_instruct(int OpCode, int address){
	if (address <= 4096) {//4 bit opCode + 12 bit address, 2^12 = 4096
		cout << "Jumping to address " << address + ID_EX.instrPC << endl;
		executeJumpValue = address + ID_EX.instrPC;
	}
	else
		cout << "ERR_J: invalid jump address" << endl;
}

void I_instruct(int OpCode, int rs, int rt, int address){
	int r;
	if (ID_EX.sig->ALUOp == 0){ // add Immediate, and LW, SW
		r = address + rt;
		cout << "addi,lw,sw sum=" << r << endl;
	}
	if (ID_EX.sig->ALUOp == 6){ // SLL
		r = (rt << address);
		cout << "Executing SLL: Rt(" << rt << ") << constant(" << address << ") =" << r << endl;
	}
	if (ID_EX.sig->ALUOp == 7){ // SRL
		r = (rt >> address);
		cout << "Executing SRL: Rt(" << rt << ") >> constant(" << address << ") =" << r << endl;
	}

	if (ID_EX.sig->ALUOp == 1){ // BNE, BEQ
		executeJumpValue = address + ID_EX.instrPC;
		r = rs - rt;
	}
	if (r == 0){
		executeZeroBit = 1;
		if (OpCode == 14){
			//if the result is 0, they are equal, BNE no branch
			executeSigBranch = 0;
		}
		else if (OpCode == 13) {
			executeSigBranch = 1;
		}
	}
	else{
		if (OpCode == 13){
			//if the result is not 0, BEQ no branch
			executeSigBranch = 0;
		}
		else if (OpCode == 14) {
			executeSigBranch = 1;
		}
	}
	//cout << "I_r: " << r << endl;
	executeALUResult = r;
}

void updateBuffer(){
	//fetch buffer update vars
	IF_ID.instruction = fetchInstr;
	IF_ID.instrPC = fetchPC;
	//decode buffer update vars
	ID_EX.instrPC = decodePC;
	ID_EX.opCode = decodeOpcode;
	ID_EX.regRs = decodeRs;
	ID_EX.regRt = decodeRt;
	ID_EX.regRd = decodeRd;
	ID_EX.destRegister = decodeDest;
	ID_EX.address = decodeAddress;
	ID_EX.regOut1 = decodeRegOut1;
	ID_EX.regOut2 = decodeRegOut2;
	ID_EX.sig->ALUOp = decodeSigALUOp;
	ID_EX.sig->ALUSrc = decodeSigALUSrc;
	ID_EX.sig->branch = decodeSigBranch;
	ID_EX.sig->jump = decodeSigJump;
	ID_EX.sig->memRead = decodeSigMemRead;
	ID_EX.sig->MemToReg = decodeSigMemToReg;
	ID_EX.sig->memWrite = decodeSigMemWrite;
	ID_EX.sig->regDest = decodeSigRegDest;
	ID_EX.sig->regWrite = decodeSigRegWrite;
	//execute buffer update vars
	MEM.ALUResult = executeALUResult;
	MEM.zeroBit = executeZeroBit;
	MEM.jumpValue = executeJumpValue;
	MEM.branchValue = executeBranchValue;
	MEM.writeData = executeWriteData;
	MEM.destRegister = executeDest;
	MEM.sig->ALUOp = executeSigALUOp;
	MEM.sig->ALUSrc = executeSigALUSrc;
	MEM.sig->branch = executeSigBranch;
	MEM.sig->jump = executeSigJump;
	MEM.sig->memRead = executeSigMemRead;
	MEM.sig->MemToReg = executeSigMemToReg;
	MEM.sig->memWrite = executeSigMemWrite;
	MEM.sig->regDest = executeSigRegDest;
	MEM.sig->regWrite = executeSigRegWrite;
	//memAccess buffer update vars
	WB.destRegister = memoryDest;
	WB.address = memAdd;
	WB.writeData = memWriteData;
	WB.writeFlag = flagW;
	WB.readFlag = flagR;
	WB.memReadData = memReadData;
	WB.sig->branch = memSigBranch;
	WB.sig->jump = memSigJump;
	WB.sig->ALUOp = memSigALUOp;
	WB.sig->ALUSrc = memSigALUSrc;
	WB.sig->branch = memSigBranch;
	WB.sig->jump = memSigJump;
	WB.sig->memRead = memSigMemRead;
	WB.sig->MemToReg = memSigMemToReg;
	WB.sig->memWrite = memSigMemWrite;
	WB.sig->regDest = memSigRegDest;
	WB.sig->regWrite = memSigRegWrite;
}
