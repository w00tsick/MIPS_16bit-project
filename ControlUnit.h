/* 
 * File:   ControlUnit.h
 * Author: Owner
 *
 * Created on March 31, 2015, 12:30 PM
 */

#ifndef CONTROLUNIT_H
#define	CONTROLUNIT_H

class ControlUnit {
public:
    int ALUOp;
    int ALUSrc;
    int branch;
    int jump;
    int memRead;
    int MemToReg;
    int regDest;
    int memWrite;
    int regWrite;
    
    ControlUnit(long);
    ~ControlUnit(void);
    void print();
    
    //parse for R format
    void R_exec(int);
    //parse for I format
    void I_exec(int);
    //parse for J format
    void J_exec(int);

};

#endif	/* CONTROLUNIT_H */

