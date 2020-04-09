/***********************************************************************
 *
 *		ASM.H
 *		Global Definitions for 68000 Assembler
 *
 *      Author: Paul McKee
 *		ECE492    North Carolina State University
 *
 *        Date:	12/13/86
 *
 *    Modified: Charles Kelly
 *              Monroe County Community College
 *              http://www.monroeccc.edu/ckelly
 *
 ************************************************************************/
#ifndef asmH
#define asmH

/* include system header files for prototype checking */
#include <stdio.h>
#include <string.h>
// include <system.hpp>
// include <process.h>
#include <stdlib.h>
#include <malloc.h>
// include <vcl.h>
//---- added by RA----
#include <iostream>
#include <string>
using std::string;
// RA
#ifndef AnsiString
#define AnsiString string
#endif
#ifndef String
#define String string
#endif
#include <stack>
//--------------------
#define stricmp   strcasecmp
#define strcmpi   strcasecmp
#define strnicmpi strncasecmp
void REMOVECR(char *line);
//------------------

/* Define a couple of useful tests */

#define isTerm(c)   (c == ',' || c == '/' || c == '-' || isspace(c) || !c || c == '{')
#define isRegNum(c) ((c >= '0') && (c <= '7'))

//extern const AnsiString VERSION =                     "5.15.04";  // don't forget to change version.txt on easy68k.com
#define VERSION "5.15.04"
#define TITLE "ASy68K Assembler v5.15.04"
/* Status values */

/* These status values are 12 bits long with
   a severity code in the upper 4 bits */

#define OK                    0x00
#define SEVERE		         0x400
#define SYNTAX	             0x401
#define INV_OPCODE	         0x402
#define INV_ADDR_MODE	     0x403
#define LABEL_REQUIRED       0x404
#define NO_ENDM              0x405
#define TOO_MANY_ARGS        0x406
#define INVALID_ARG          0x407
#define COMMA_EXPECTED       0x408
#define PHASE_ERROR	         0x409
#define FILE_ERROR           0x40A
#define MACRO_NEST           0x40B
#define NO_IF                0x40C
#define NO_WHILE             0x40D
#define NO_REPEAT            0x40E
#define NO_FOR               0x40F
#define ENDI_EXPECTED        0x410
#define ENDW_EXPECTED        0x411
#define ENDF_EXPECTED        0x412
#define REPEAT_EXPECTED      0x413
#define LABEL_ERROR          0x414
#define NO_DBLOOP            0x415
#define DBLOOP_EXPECTED      0x416
#define BAD_BITFIELD         0x417
#define ILLEGAL_SYMBOL       0x418
#define EXCEPTION            0x999
#define ERRORN	             0x300
#define UNDEFINED	         0x301
#define DIV_BY_ZERO	         0x302
#define MULTIPLE_DEFS	     0x303
#define REG_MULT_DEFS	     0x304
#define REG_LIST_UNDEF       0x305
#define INV_FORWARD_REF      0x306
#define INV_LENGTH	         0x307
#define MINOR		         0x200
#define INV_SIZE_CODE	     0x201
#define INV_QUICK_CONST      0x202
#define INV_MOVE_QUICK_CONST 0x203
#define INV_VECTOR_NUM       0x204
#define INV_BRANCH_DISP      0x205
#define INV_DISP	         0x206
#define INV_ABS_ADDRESS      0x207
#define INV_8_BIT_DATA       0x208
#define INV_16_BIT_DATA      0x209
#define NOT_REG_LIST	     0x20A
#define REG_LIST_SPEC	     0x20B
#define INV_SHIFT_COUNT      0x20C
#define INV_OPERATOR         0x20D
#define FAIL_ERROR           0x20E
#define WARNING		         0x100
#define ASCII_TOO_BIG	     0x101
#define NUMBER_TOO_BIG       0x102
#define INCOMPLETE	         0x103
#define FORCING_SHORT        0x104
#define ODD_ADDRESS	         0x105
#define END_MISSING          0x106
#define ADDRESS_MISSING      0x107
#define THEN_EXPECTED        0x108
#define DO_EXPECTED          0x109
#define FORWARD_REF          0x10A
#define LABEL_TOO_LONG       0x10B
#define SEVERITY	         0xF00
#define BACKREF	              0x01
#define REDEFINABLE           0x02	
#define REG_LIST_SYM          0x04	
#define MACRO_SYM             0x08    
#define DS_SYM                0x10    
#define IMMEDIATE		   0x00800
#define BYTE_SIZE                1
#define WORD_SIZE                2
#define LONG_SIZE                4
#define SHORT_SIZE               8
#define MEM_SIZE        0x00FFFFFF
#define NORMAL                   0
#define MILD_ERROR               1
#define CRITICAL                 2
#define TAB1                    12
#define TAB2                    20
#define TAB3                    44
#define MACRO_NEST_LIMIT       256

/* The NEWERROR macros updates the error variable var only if the
   new error code is more severe than all previous errors.  Throughout
   ASM this is the standard means of reporting errors. */

//#define NEWERROR(var, code)	if ((code & SEVERITY) > var) var = code
// ck: the previous line was causing errors when placed inside if-else
#define NEWERROR(var, code)      var = ((code & SEVERITY) > var) ? code : var


/* Symbol table definitions */

//extern #define SIGCHARS; // RA = 33;        // significant characters in symbol
// RA some of these are better off as defines because they're used as array sizing and
// if they're not available at compile time, errors are thrown, so they can't be
// extern consts. (I've been converting most of this file to extern X because of
// duplicate symbol errors when linking, as this is really ancient code which breaks
// modern C and C++ rules.) Also I'm guessing that these may have originally been
// #defines because they're all uppercase.

#define SIGCHARS 33
#define MAX_ARGS 36       // maximum number of macro arguments
#define ARG_SIZE 256      // maximum size of each argument

/* Structure for operand descriptors */
struct opDescriptor
{
  int  mode;	// Mode number (see below)
  int  data;	// IMMEDIATE value, displacement, or absolute address
  int  field;   // for bitField instructions
  char reg;	// Principal register number (0-7)
  char index;	// Index register number (0-7 = D0-D7, 8-15 = A0-A7)
  char size;	// Size of index register (WORD or LONG, see below)
                // or forced size of IMMEDIATE instruction
                // BYTE_SIZE, WORD_SIZE, LONG_SIZE
                // Also used to prevent MOVEQ, ADDQ & SUBQ optimizations (see OPPARSE.CPP)
  bool backRef;	// True if data field is known on first pass
};


/* Structure for a symbol table entry */
typedef struct symbolEntry {
	int value;			/* 32-bit value of the symbol */
	struct symbolEntry *next;	/* Pointer to next symbol in linked list */
	char flags;			/* Flags (see below) */
	char name[SIGCHARS+1];		/* Name */
	} symbolDef;


/* Instruction table definitions */

/* Structure to describe one "flavor" of an instruction */

typedef struct {
	int source,		/* Bit masks for the legal source...	*/
	    dest;		/*  and destination addressing modes	*/
	char sizes;		/* Bit mask for the legal sizes */
	int (*exec)(int, int, opDescriptor *, opDescriptor *, int *);
                                /* Pointer to routine to build the instruction */
	unsigned short int bytemask,	/* Skeleton instruction masks for byte size...  */ // RA added unsigned
		  wordmask,	/*  word size, ...			        */
		  longmask;	/*  and long sizes of the instruction	        */
	} flavor;


/* Structure for the instruction table */
typedef struct {
	char *mnemonic;		/* Mnemonic */
	flavor *flavorPtr;	/* Pointer to flavor list */
	char flavorCount;	/* Number of flavors in flavor list */
	bool parseFlag;		/* Should assemble() parse the operands? */
	int (*exec)(int, char *, char *, int *);
			/* Routine to be called if parseFlag is FALSE */
	} instruction;


/* Addressing mode codes/bitmasks */
#define DnDirect               0x00001
#define AnDirect               0x00002
#define AnInd                  0x00004
#define AnIndPost              0x00008
#define AnIndPre               0x00010
#define AnIndDisp              0x00020
#define AnIndIndex             0x00040
#define AbsShort               0x00080
#define AbsLong                0x00100
#define PCDisp                 0x00200
#define PCIndex                0x00400
#define SRDirect               0x01000
#define CCRDirect     		   0x02000
#define USPDirect     		   0x04000
#define SFCDirect    		   0x08000
#define DFCDirect     		   0x10000
#define VBRDirect     		   0x20000

enum tabTypes{ Assembly, Fixed };

extern bool listFlag;	       // True if a listing is desired
extern bool objFlag;	       // True if an S-Record object code file is desired
extern bool CEXflag;	       // True is Constants are to be EXpanded
extern bool BITflag;           // True to assemble bitfield instructions
extern bool CREflag;           // true adds symbol table to listing
extern bool MEXflag;           // true expands macro calls in listing
extern bool SEXflag;           // true expands structured code in listing
extern bool WARflag;           // true shows Warnings during assembly
extern bool noFileName;        // true indicates no name for current source file



// was  defined in both INSTTABL.CPP and MOVEM.CPP
#define ControlAlt  (AnInd | AnIndDisp | AnIndIndex | AbsShort | AbsLong)

#define NEW_PAGE_MARKER "<------------------------------ PAGE ------------------------------>"
// function prototype definitions
#include "proto.h"

#endif
