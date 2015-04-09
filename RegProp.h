/* 
 * File:   RegProp.h
 * Author: Owner
 *
 * Created on April 7, 2015, 6:13 PM
 */

#include <string.h>
#include <iostream>
#include "ControlUnit.h"

#ifndef REGPROP_H
#define	REGPROP_H

using namespace std;

class RegProp {
public:
    	string instruction;
	int currentPC;
	int regRd;
	int regRt;
	int regRs;
	int regOut1;
	int regOut2;
	int jumpValue;
	int branchValue;
	int opCode;
	int ALUResult;
	bool zeroBit;
	int destRegister;
	int memReadData;
	int address;
	int writeData;
	int writeReg;
	ControlUnit* sig;
        RegProp();
        ~RegProp();
        void clear();
        void print();
private:

};

#endif	/* REGPROP_H */

