/* 
 * File:   RegProp.cpp
 * Author: Owner
 * 
 * Created on April 7, 2015, 6:13 PM
 */

#include "RegProp.h"


using namespace std;


RegProp::RegProp() {
    this->clear();
}

void RegProp::clear(void){
    	this->ALUResult = 0;
	this->currentPC = 0;
	this->destRegister = 0;
	this->instruction = "";
	this->zeroBit = 0;
	this->jumpValue = 0;
	this->branchValue = 0;
	this->memReadData = 0;
	this->opCode = 1;
	this->regRd = 0;
	this->regRs = 0;
	this->regRt = 0;
	this->regOut1 = 0;
	this->regOut2 = 0;
	this->address = 0;
	this->writeReg = 0;
	this->writeData=0;
	sig = new ControlUnit;
}

RegProp::~RegProp() {
}

void RegProp::print(void){
    	cout<<this->ALUResult<<endl;
	cout<<this->currentPC<<endl;
	cout<<this->destRegister<<endl;
	cout<<this->instruction<<endl;
	cout<<this->zeroBit<<endl;
	cout<<this->jumpValue<<endl;
	cout<<this->branchValue<<endl;
	cout<<this->memReadData<<endl;
	cout<<this->opCode<<endl;
	cout<<this->regRd<<endl;
	cout<<this->regRs<<endl;
	cout<<this->regRt<<endl;
	cout<<this->address<<endl;
	cout<<this->regOut1<<endl;
	cout<<this->regOut2<<endl;
	cout<<this->writeReg<<endl;
	cout<<this->writeData<<endl;
}

