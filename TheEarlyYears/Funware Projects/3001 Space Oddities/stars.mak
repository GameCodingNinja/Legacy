#
# Borland C++ IDE generated makefile
# Generated 12/26/01 at 10:40:59 PM 
#
.AUTODEPEND


#
# Borland C++ tools
#
IMPLIB  = Implib
BCC32   = Bcc32 +BccW32.cfg 
BCC32I  = Bcc32i +BccW32.cfg 
TLINK32 = TLink32
TLIB    = TLib
BRC32   = Brc32
TASM32  = Tasm32
#
# IDE macros
#


#
# Options
#
IDE_LinkFLAGS32 =  -LC:\program files\borland\BC5\LIB
IDE_ResFLAGS32 = 
LinkerLocalOptsAtW32_3001_Space_Odditiesdexe =  -Tpe -aa -V4.0 -c -v -LC:\PROGRA~1\BORLAND\BC5\LIB
ResLocalOptsAtW32_3001_Space_Odditiesdexe = 
BLocalOptsAtW32_3001_Space_Odditiesdexe = 
CompInheritOptsAt_3001_Space_Odditiesdexe = -IC:\PROGRA~1\BORLAND\BC5\INCLUDE;C:\ROUNDA~1\LIBRARY 
LinkerInheritOptsAt_3001_Space_Odditiesdexe = -x
LinkerOptsAt_3001_Space_Odditiesdexe = $(LinkerLocalOptsAtW32_3001_Space_Odditiesdexe)
ResOptsAt_3001_Space_Odditiesdexe = $(ResLocalOptsAtW32_3001_Space_Odditiesdexe)
BOptsAt_3001_Space_Odditiesdexe = $(BLocalOptsAtW32_3001_Space_Odditiesdexe)

#
# Dependency List
#
Dep_Stars = \
   3001 Space Oddities.exe

Stars : BccW32.cfg $(Dep_Stars)
  echo MakeNode

Dep_3001_Space_Odditiesdexe = \
   ssconfig.obj\
   settings.obj\
   ssanim.obj\
   C:\PROGRA~1\BORLAND\BC5\LIB\noeh32.lib\
   basecfg.obj\
   OrderOpt.obj\
   baseset.obj\
   printcnl.obj\
   helphint.obj\
   orderfrm.obj\
   tbitmap.obj\
   tobjlist.obj\
   mediadir.obj\
   mediaply.obj\
   tstrlist.obj\
   tcanvas.obj\
   ssthread.obj\
   ssbase.obj\
   metafile.obj\
   sstools.obj\
   printer.obj\
   dib.obj\
   decodgif.obj\
   graphics.res\
   ssbase.res\
   stars.res

3001 Space Oddities.exe : $(Dep_3001_Space_Odditiesdexe)
  $(TLINK32) @&&|
 /v $(IDE_LinkFLAGS32) $(LinkerOptsAt_3001_Space_Odditiesdexe) $(LinkerInheritOptsAt_3001_Space_Odditiesdexe) +
C:\PROGRA~1\BORLAND\BC5\LIB\c0w32.obj+
ssconfig.obj+
settings.obj+
ssanim.obj+
basecfg.obj+
OrderOpt.obj+
baseset.obj+
printcnl.obj+
helphint.obj+
orderfrm.obj+
tbitmap.obj+
tobjlist.obj+
mediadir.obj+
mediaply.obj+
tstrlist.obj+
tcanvas.obj+
ssthread.obj+
ssbase.obj+
metafile.obj+
sstools.obj+
printer.obj+
dib.obj+
decodgif.obj
$<,$*
C:\PROGRA~1\BORLAND\BC5\LIB\noeh32.lib+
C:\PROGRA~1\BORLAND\BC5\LIB\import32.lib+
C:\PROGRA~1\BORLAND\BC5\LIB\cw32.lib

graphics.res+
ssbase.res+
stars.res

|
ssconfig.obj :  ssconfig.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe) -o$@ ssconfig.cpp
|

settings.obj :  settings.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe) -o$@ settings.cpp
|

ssanim.obj :  ssanim.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe) -o$@ ssanim.cpp
|

basecfg.obj :  ..\library\basecfg.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe) -o$@ ..\library\basecfg.cpp
|

OrderOpt.obj :  ..\library\OrderOpt.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe) -o$@ ..\library\OrderOpt.cpp
|

baseset.obj :  ..\library\baseset.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe) -o$@ ..\library\baseset.cpp
|

printcnl.obj :  ..\library\printcnl.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe) -o$@ ..\library\printcnl.cpp
|

helphint.obj :  ..\library\helphint.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe) -o$@ ..\library\helphint.cpp
|

orderfrm.obj :  ..\library\orderfrm.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe) -o$@ ..\library\orderfrm.cpp
|

tbitmap.obj :  ..\library\tbitmap.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe) -o$@ ..\library\tbitmap.cpp
|

tobjlist.obj :  ..\library\tobjlist.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe) -o$@ ..\library\tobjlist.cpp
|

mediadir.obj :  ..\library\mediadir.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe) -o$@ ..\library\mediadir.cpp
|

mediaply.obj :  ..\library\mediaply.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe) -o$@ ..\library\mediaply.cpp
|

tstrlist.obj :  ..\library\tstrlist.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe) -o$@ ..\library\tstrlist.cpp
|

tcanvas.obj :  ..\library\tcanvas.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe) -o$@ ..\library\tcanvas.cpp
|

ssthread.obj :  ..\library\ssthread.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe) -o$@ ..\library\ssthread.cpp
|

ssbase.obj :  ..\library\ssbase.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe) -o$@ ..\library\ssbase.cpp
|

metafile.obj :  ..\library\metafile.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe) -o$@ ..\library\metafile.cpp
|

sstools.obj :  ..\library\sstools.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe) -o$@ ..\library\sstools.cpp
|

printer.obj :  ..\library\printer.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe) -o$@ ..\library\printer.cpp
|

dib.obj :  ..\library\dib.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe) -o$@ ..\library\dib.cpp
|

decodgif.obj :  ..\library\decodgif.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe) -o$@ ..\library\decodgif.cpp
|

graphics.res :  graphics.rc
  $(BRC32) -R @&&|
 $(IDE_ResFLAGS32) $(ROptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe)  -FO$@ graphics.rc
|
ssbase.res :  ..\library\ssbase.rc
  $(BRC32) -R @&&|
 $(IDE_ResFLAGS32) $(ROptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe)  -FO$@ ..\library\ssbase.rc
|
stars.res :  stars.rc
  $(BRC32) -R @&&|
 $(IDE_ResFLAGS32) $(ROptsAt_3001_Space_Odditiesdexe) $(CompInheritOptsAt_3001_Space_Odditiesdexe)  -FO$@ stars.rc
|
# Compiler configuration file
BccW32.cfg : 
   Copy &&|
-w
-R
-v
-vi
-H
-H=Stars.csm
-W
-wrpt
-wcln
-w-sig
-wucp
-wbbf
-wpin
-wnak
-wdef
-wnod
-wamb
-wuse
-wstv
-wasm
-wamp
-wobs
-wpch
-w-par
-v
-R
-x-
-xd-
-RT-
-O2
-g20
-j1
| $@


