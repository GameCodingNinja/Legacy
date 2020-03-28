//
// File name: FixASM.HPP
//
// Description: Several fixed-point assembly macros.
//
// Target: 32-bit
//
// Project: Cutting Edge 3-D Game Programming
//

#ifndef ASMFNCT_ASM
#define ASMFNCT_ASM


/*****************************************************************************
/  These are C-code functions - don't mangle the names
/ ***************************************************************************/
extern "C"
{

	/*****************************************************************************
	/  Function declarations
	/ ***************************************************************************/
	void memor( void * dest, void * source, unsigned int size );
	void memand( void * dest, void * source, unsigned int size );
}



#endif // ASMFNCT_ASM



