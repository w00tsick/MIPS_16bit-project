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
string *uniqueInstruct = NULL; 
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
    cout << "entering main \n";
    int clock = 0;
    
    Registers[0] = 0x0000; //$zero
    Registers[1] = 0x0001; //$t0
    Registers[2] = 0x0008; //$t1
    Registers[3] = 0x0005; //$t2
    Registers[4] = 0x1010; //$a0
    Registers[5] = 0xFF00; //$a1
    Registers[6] = 0x00FF; //$v0
    Registers[7] = 0x00F0; //$v1
            
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
    cout << "Number of instructions: " << instruct_count << endl;
    uniqueInstruct = new string[instruct_count+1];
    
    int i = 0;
    input.open("source.txt");
    while(input.good()){
        getline(input, uniqueInstruct[i]);
        i++;
    }
    input.close();
    
    cout << "entering while loop" << endl;;
    
    while(PC < instruct_count){
        if (clock == 0) {
            fetch();
            decode();
            execute();
            memAccess();
            updateBuffer();
            writeBack();
            clock = 0;
            PC++;
        }
        else {
            
            clock = 1;
        }
    }
    return 0;
}

void fetch(){
    cout << "Entering fetch()" << endl;
    //clear the properties
    ID_EX.clear();
    //get the instruction from instruction array
    IF_ID.instruction = uniqueInstruct[PC];
    //increase iteration of PC
    IF_ID.currentPC = PC + 1;
    cout << "Instruction: " << IF_ID.instruction << endl;
}

void decode(){
    ID_EX = IF_ID;
    
    bool I = false;
    bool R = false;
    bool J = false;
    char *pointer;
    
    string opCode = ID_EX.instruction.substr(0,4);
    cout << "OpCode: " << opCode << endl;
    
    for (int i = 0; i < sizeof(R_CODE)/sizeof(R_CODE[0]); i++){
        if(opCode.compare(R_CODE[i]) == 0){
            cout << "R type opCode \n";
            R = true;
            break;
        }
    }
    
    if(!R){
        for (int i = 0; i < sizeof(I_CODE)/sizeof(I_CODE[0]); i++){
                if(opCode.compare(I_CODE[i]) == 0){
                    cout << "I type opCode \n";
                        I = true;
                        break;
                }
        }
    }
    
    if(!R && !I){
        for (int i = 0; i < sizeof(J_CODE)/sizeof(J_CODE[0]); i++){
                if(opCode.compare(J_CODE[i]) == 0){
                    cout << "J type opCode \n";
                        J = true;
                        break;
                }
        }
    }
    
    ID_EX.opCode=strtol(ID_EX.instruction.substr(0,4).c_str(),&pointer,2);
    
    if(R){
        //R type
        cout << "R type instruction: " << endl;
        //get rs
        ID_EX.regRs = strtol(ID_EX.instruction.substr(4,3).c_str(),&pointer,2);
        cout << "regRs: " << ID_EX.regRs << endl;
        //get rt
        ID_EX.regRt = strtol(ID_EX.instruction.substr(7,3).c_str(),&pointer,2);
        cout << "regRt: " << ID_EX.regRt << endl;
        //get rd
        ID_EX.regRd = strtol(ID_EX.instruction.substr(10,3).c_str(),&pointer,2);
        cout << "regRd: " << ID_EX.regRd << endl;
        //set destination register
	ID_EX.destRegister = ID_EX.regRd;
        //set signal to R signal
        ID_EX.sig-> R_exec(ID_EX.opCode);
    }
    
    else if(I){
        //I type
        cout << "I type instruction: " << endl;
        //get rs
        ID_EX.regRs=strtol(ID_EX.instruction.substr(4,3).c_str(),&pointer,2);
        cout << "regRs: " << ID_EX.regRs << endl;
        //get rt	
        ID_EX.regRt=strtol(ID_EX.instruction.substr(7,3).c_str(),&pointer,2);
        cout << "regRt: " << ID_EX.regRt << endl;
        //get address for constant
        ID_EX.address=strtol(ID_EX.instruction.substr(10,6).c_str(),&pointer,2);
        cout << "address: " << ID_EX.address << endl;
        //set destination register
	ID_EX.destRegister = ID_EX.regRt;
        //set signal to I
	ID_EX.sig-> I_exec(ID_EX.opCode);
    }
    
    else if (J){
        //J type
        cout << "J type instruction: " << endl;
        decodeAddress =strtol(ID_EX.instruction.substr(4,12).c_str(),&pointer,2);
        ID_EX.sig->J_exec(ID_EX.opCode);
    }
    else{
    	//Error
    	cout << "THIS ADDRESS != R|I|J" << endl;
    }
    //save signals for updating buffer
    /*decodeSigALUOp = IF_ID.sig->ALUOp;
    decodeSigALUSrc = IF_ID.sig->ALUSrc;
    decodeSigBranch = IF_ID.sig->branch;
    decodeSigJump = IF_ID.sig->jump;
    decodeSigMemRead = IF_ID.sig->memRead;
    decodeSigMemToReg = IF_ID.sig->MemToReg;
    decodeSigMemWrite = IF_ID.sig->memWrite;
    decodeSigRegDest = IF_ID.sig->regDest;
    decodeSigRegWrite = IF_ID.sig->regWrite;*/
    
    ID_EX.regOut1 = Registers[ID_EX.regRs];
    ID_EX.regOut2 = Registers[ID_EX.regRt];
    cout << "Register File Output: " << ID_EX.regOut1 << ", " << ID_EX.regOut2 << endl;
}

void execute(){
    cout << "entering execute(): " << endl;
    MEM = ID_EX;
    //J format

    if(MEM.sig -> jump == 1){
        cout << "execute J instruction: " << endl;
        J_instruct(MEM.opCode, MEM.address);
    }
    else{
        //R format
        if(MEM.sig->ALUSrc == 0){
            cout << "execute R instruction: " << endl;
            R_instruct(MEM.opCode, ID_EX.regOut1, ID_EX.regOut2);
        }else if(MEM.sig-> ALUSrc == 1){
            //I format
            cout << "Executing I format: " << endl;
            I_instruct(MEM.opCode, ID_EX.regOut1, ID_EX.regOut2, MEM.address);
        }else{
            cout << "Execute Error!" << endl;
        }
    }
}

void memAccess(){
    
    WB = MEM;
    
    memAdd = MEM.address;
    memWriteData = MEM.writeData;
    cout << "memAdd: "<< memAdd << endl;
    cout << "memWriteData: "<< memWriteData << endl;

    flagW = MEM.writeFlag;
    flagR = MEM.readFlag;
    
    if (MEM.sig->memRead == 1){
    	WB.memReadData = MemoryData[MEM.ALUResult];
        cout << "Writeback Read Data: " << WB.memReadData << endl;
    }
    
    if (MEM.sig->memWrite == 1){
    	MemoryData[MEM.ALUResult] = MEM.regOut1;
        cout << "Memory Data contents: " << MemoryData[MEM.ALUResult] << endl;
    }
    
    return;
}

void writeBack(){
    cout << "Entering WriteBack: " << endl;
    int mux0, mux1, wData, wAddress;
    mux0 = WB.memReadData;
    mux1 = WB.address;
    wData = WB.writeData;
    
    //writeBack mux
    if(WB.sig->jump != 1){
        if (WB.sig->MemToReg == 0){
                if(WB.sig->ALUSrc == 0){
                    //R format
                    cout << "entering R format WB: " << endl;
                    Registers[WB.regRd] = WB.ALUResult;
                }
                else{
                    cout << "entering I format WB: " << endl;
                    //I format
                    if(WB.sig->branch == 1){
                        cout << "Branching I format: " << endl;
                        PC = PC + mux1;
                    }else{
                        cout << "Writing to memory: " << endl;
                        if(WB.sig->memWrite != 1){
                            Registers[WB.regRt] = WB.ALUResult;
                            cout << "Register contents: " << Registers[WB.regRt] << endl;
                        }
                    }
                }
        }
        }else{
                //J format
                cout << "entering J format WB: " << endl;
                PC = WB.jumpValue;
        }
    return;
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
            rd = !(rs && rt) && (rt || rs);
            break;
    }
    cout << "R_rd: " << rd << endl;
    MEM.ALUResult = rd;
    
    if(rd == 0){
        MEM.zeroBit = 1;
    }
}

void J_instruct(int OpCode, int address){
    if(address <= 4096)//4 bit opCode + 12 bit address, 2^12 = 4096
        MEM.jumpValue = address;
    else
        cout << "ERR_J: invalid jump address" << endl;
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
            MEM.sig-> branch = 0;
        }
    }
    else{
        if(OpCode == 13)
            //if the result is not 0, BEQ no branch
            MEM.sig-> branch= 0;
    }
    cout << "I_r: "<< r << endl;
    MEM.ALUResult = r;
}

void updateBuffer(){
    /*//fetch buffer update vars
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
    WB.memReadData = memRData;*/
}
