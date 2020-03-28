
// A set of functions to handle real-mode addresses in 32-bit mode,
// set the video mode, and give access to the video memory.

#include <Dos.h>
#include <Conio.h>

unsigned int BaseDs = 0u;

int InitDPMI(void)
  {
  REGS Register;
  // Do not proceed if BaseDs has already been initialized
  if (BaseDs == 0)
     {
     // Get the base linear address for DS.
     Register.w.bx = _DS;
     Register.w.ax = 0x0006;
     int386(0x31, &Register, &Register);
     // If we encounter an error, return zero
     if (Register.x.cflag)
        return 0;
     // Multiply by 65,536 and mask out un-wanted bits
     BaseDs = ((unsigned int)(Register.w.cx) << 16) | Register.w.dx;

     Register.w.bx = _DS;
     Register.w.ax = 0x0008;
     Register.w.cx = Register.w.dx = 0xFFFF;
     int386(0x31, &Register, &Register);
     return !Register.x.cflag;
     }
  else
      return 1;
  }

// Call this one to get a real-mode address  
unsigned int GetAddress(unsigned int RMLocation)
   {
   if ( ( BaseDs == 0 ) && ( InitDPMI() == 0 ) )
      return 0;
   return ( RMLocation - BaseDs );
   }

// Call this one to get a real-mode address   
// Don't call this one without calling InitDPMI
unsigned int ConvertAddress(unsigned int Address)
   {
   return Address - BaseDs;
   }

// Function returns video address   
unsigned char *VideoAddress ()
   {
   return (unsigned char *)GetAddress(0xA0000UL);
   }
   
// Function to set video mode
void SetVideo ( short int mode )
   {
   REGS regs;
	regs.w.ax = mode;
	regs.h.ah = 0;
	int386 ( 0x10, &regs, &regs );
   }
