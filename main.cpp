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
string R_CODE[] = {"0001", "0010", "0011", "0100", "0101", "0110"};
string I_CODE[] = {"0000", "0111", "1000", "1001", "1010", "1011", "1101", "1110"};
string J_CODE[] = {"1100"};

//Global variables
int Registers[8];
int PC = 0x0000;
int instruct_count = 0;
int Tot_Reg = 8;
int MemoryData[256];
string *uniqueInstruct;
//fetch buffer update vars
string fetchInstr;
int fetchPC = 0;
//decode buffer update vars
    //still need controlsig globals
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
int executeSigBranch = 0;
//memAccess buffer update vars
int memAdd = 0;
int memWriteData = 0;
int flagW = 0;
int flagR = 0;
int memRData = 0;

//Signal placeholders
int decodeSigALUOp;
int decodeSigALUSrc;
int decodeSigBranch;
int decodeSigJump;
int decodeSigMemRead;
int decodeSigMemToReg;
int decodeSigMemWrite;
int decodeSigRegDest;
int decodeSigRegWrite;

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
void I_instruct(int, int, int ,int);
void R_instruct(int, int, int);

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
        cout << "File not found" << endl;
        exit(0);
    }
    
    input.open("source.txt");
    while(input.good()){
        getline(input, uniqueInstruct[i]);
        i++;
    }
    input.close();
    
    cout << "entering while loop \n";
    while(PC < instruct_count){
        if (clock == 1) {
            fetch();
            decode();
            execute();
            memAccess();
            updateBuffer();
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
    //ID_EX.clear();
    //get the instruction from instruction array
    fetchInstr = uniqueInstruct[PC];
    //increase iteration of PC
    fetchPC = PC + 1;
}

void decode(){
    //ID_EX = IF_ID;
    
    bool I = false;
    bool R = false;
    bool J = false;
    char *pointer;
    
    string opCode = ID_EX.instruction.substr(0,4);
    cout << "OpCode: " << opCode << endl;
    
    for (int i = 0; i < instruct_count; i++){
        if(opCode.compare(R_CODE[i]) == 0){
            R = true;
            break;
        }
    }
    
    if(!R){
        for (int i = 0; i < instruct_count; i++){
                if(opCode.compare(I_CODE[i]) == 0){
                        I = true;
                        break;
                }
        }
    }
    
    if(!R && !I){
        for (int i = 0; i < instruct_count; i++){
                if(opCode.compare(J_CODE[i]) == 0){
                        J = true;
                        break;
                }
        }
    }
    
    decodeOpcode = strtol(IF_ID.instruction.substr(0, 4).c_str(), &pointer, 2);
    
    if(R){
        //R type
        cout << "R type instruction: " << endl;
        //get rs
        decodeRs = strtol(IF_ID.instruction.substr(4,3).c_str(),&pointer,2);
        //get rt
        decodeRt = strtol(IF_ID.instruction.substr(7,3).c_str(),&pointer,2);
        //get rd
        decodeRd = strtol(IF_ID.instruction.substr(10,3).c_str(),&pointer,2);
        //set destination register
	decodeDest = ID_EX.regRd;
        //set signal to R signal
        IF_ID.sig-> R_exec(IF_ID.opCode);
    }
    
    else if(I){
        //I type
        cout << "I type instruction: " << endl;
        //get rs
        decodeRs=strtol(IF_ID.instruction.substr(4,3).c_str(),&pointer,2);
        //get rt	
        decodeRt=strtol(IF_ID.instruction.substr(7,3).c_str(),&pointer,2);
        //get address for constant
        decodeAddress=strtol(IF_ID.instruction.substr(10,6).c_str(),&pointer,2);
        //set destination register
	decodeDest = IF_ID.regRt;//SHOULD THIS BE RS OR RT????????????????????????????????
        //set signal to I
	IF_ID.sig->I_exec(IF_ID.opCode);

    }
    
    else if (J){
        //J type
        cout << "J type instruction: " << endl;
        decodeAddress =strtol(IF_ID.instruction.substr(4,12).c_str(),&pointer,2);
        IF_ID.sig->J_exec(IF_ID.opCode);
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
    
    decodeRegOut1 = Registers[IF_ID.regRs];
    decodeRegOut2 = Registers[IF_ID.regRt];
    cout << "Register File Output: " << decodeRegOut1 << ", " << decodeRegOut2 << endl;
}

void execute(){//WTF IS THIS??????? why not just use a switch statement with some variable holding the opcode?
    //MEM = ID_EX;
    //J format
    if(ID_EX.sig -> jump = 1)
        J_instruct(ID_EX.opCode, ID_EX.address);
    else{
        //R format
        if(ID_EX.sig->ALUSrc = 0){
            R_instruct(ID_EX.opCode, ID_EX.regOut1, ID_EX.regOut2);
        }else if(ID_EX.sig-> ALUSrc = 1){
            //I format
            I_instruct(ID_EX.opCode, ID_EX.regOut1, ID_EX.regOut2, ID_EX.address);
        }else{
            cout << "Execute Error!" << endl;
        }
    }
}

void memAccess(){
    memAdd = MEM.address;
    memWriteData = MEM.writeData;
    cout << "memAdd: "<< memAdd << endl;
    cout << "memWriteData: "<< memWriteData << endl;

    flagW = MEM.writeFlag;
    flagR = MEM.readFlag;
    
    if (flagR == 1){
    	memRData = MemoryData[memAdd];
    }
    
    if (flagW == 1){
    	MemoryData[memAdd] = memWriteData;
    }
    
    return;
}

void writeBack(){
    int mux0, mux1, wData, wAddress;
    mux0 = WB.memReadData;
    mux1 = WB.address;
    wData = WB.writeData;
    
    //writeBack mux
    if (WB.sig->MemToReg == 0){
    	wAddress = mux0;
    }
    else {
    	wAddress = mux1;
    }
    
    //Register Write
    if (WB.sig->regWrite == 1){
    	Registers[wAddress] = wData;
    }
    cout << "wAddress: "<< wAddress << endl;
    return;
}

void R_instruct(int OpCode, int rs, int rt){
    int rd;
    //R instruction
    switch(ID_EX.sig->ALUOp)
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
    cout << "R_rd: " << rd << endl;
    executeALUResult = rd;
    
    if(rd == 0){
        executeZeroBit = 1;
    }
}

void J_instruct(int OpCode, int address){
    if(address <= 4096)//4 bit opCode + 12 bit address, 2^12 = 4096
        executeJumpValue = address;
    else
        cout << "ERR_J: invalid jump address" << endl;
}

void I_instruct(int OpCode, int rs, int rt, int address){
    int r;
    if(ID_EX.sig->ALUOp == 0){ // add Immediate, and LW, SW
        r = address + rt;
    }
    if(ID_EX.sig->ALUOp == 6){ // SLL
        r = (rt<<address);
    }
    if(ID_EX.sig->ALUOp == 7){ // SRL
        r = (rt>>address);
    }
    //why is this op == 1??????????????????????????????????????????????????????????????????????
    if(ID_EX.sig->ALUOp == 1){ // BNE, BEQ
        executeBranchValue = address;
        r = rs - rt;
    }
    if(r == 0){
        ID_EX.zeroBit = 1;
        if(OpCode == 14){
            //if the result is 0, they are equal, BNE no branch
            executeSigBranch= 0;
        }
    }
    else{
        if(OpCode == 13)
            //if the result is not 0, BEQ no branch
            executeSigBranch = 0;
    }
    cout << "I_r: "<< r << endl;
    executeALUResult = r;
}

void updateBuffer(){
    //fetch buffer update vars
    IF_ID.instruction = fetchInstr;
    IF_ID.currentPC = fetchPC;
    //decode buffer update vars
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
    MEM.sig->branch = executeSigBranch;
    //memAccess buffer update vars
    WB.address = memAdd;
    WB.writeData = memWriteData;
    WB.writeFlag = flagW;
    WB.readFlag = flagR;
    WB.memReadData = memRData;
}
