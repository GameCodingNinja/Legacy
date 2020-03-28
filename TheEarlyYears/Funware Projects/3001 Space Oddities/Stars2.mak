#
# Borland C++ IDE generated makefile
# Generated 1/28/01 at 12:37:04 PM 
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
LinkerLocalOptsAtW32_Starsdexe =  -Tpe -aa -V4.0 -c -v -LC:\PROGRA~1\BORLAND\BC5\LIB -n
ResLocalOptsAtW32_Starsdexe = 
BLocalOptsAtW32_Starsdexe = 
CompInheritOptsAt_Starsdexe = -IC:\PROGRA~1\BORLAND\BC5\INCLUDE;C:\ROUNDA~1\LIBRARY 
LinkerInheritOptsAt_Starsdexe = -m
LinkerOptsAt_Starsdexe = $(LinkerLocalOptsAtW32_Starsdexe)
ResOptsAt_Starsdexe = $(ResLocalOptsAtW32_Starsdexe)
BOptsAt_Starsdexe = $(BLocalOptsAtW32_Starsdexe)

#
# Dependency List
#
Dep_Stars2 = \
   Stars.exe

Stars2 : BccW32.cfg $(Dep_Stars2)
  echo MakeNode

Dep_Starsdexe = \
   ssconfig.obj\
   settings.obj\
   ssanim.obj\
   ..\..\progra~1\borland\bc5\lib\noeh32.lib\
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
   ssbase.res\
   stars.res

Stars.exe : $(Dep_Starsdexe)
  $(TLINK32) @&&|
 /v $(IDE_LinkFLAGS32) $(LinkerOptsAt_Starsdexe) $(LinkerInheritOptsAt_Starsdexe) +
C:\PROGRA~1\BORLAND\BC5\LIB\c0w32.obj+
ssconfig.obj+
settings.obj+
ssanim.obj+
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
..\..\progra~1\borland\bc5\lib\noeh32.lib+
C:\PROGRA~1\BORLAND\BC5\LIB\import32.lib+
C:\PROGRA~1\BORLAND\BC5\LIB\cw32.lib

ssbase.res+
stars.res

|
ssconfig.obj :  ssconfig.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_Starsdexe) $(CompInheritOptsAt_Starsdexe) -o$@ ssconfig.cpp
|

settings.obj :  settings.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_Starsdexe) $(CompInheritOptsAt_Starsdexe) -o$@ settings.cpp
|

ssanim.obj :  ssanim.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_Starsdexe) $(CompInheritOptsAt_Starsdexe) -o$@ ssanim.cpp
|

OrderOpt.obj :  ..\library\OrderOpt.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_Starsdexe) $(CompInheritOptsAt_Starsdexe) -o$@ ..\library\OrderOpt.cpp
|

baseset.obj :  ..\library\baseset.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_Starsdexe) $(CompInheritOptsAt_Starsdexe) -o$@ ..\library\baseset.cpp
|

printcnl.obj :  ..\library\printcnl.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_Starsdexe) $(CompInheritOptsAt_Starsdexe) -o$@ ..\library\printcnl.cpp
|

helphint.obj :  ..\library\helphint.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_Starsdexe) $(CompInheritOptsAt_Starsdexe) -o$@ ..\library\helphint.cpp
|

orderfrm.obj :  ..\library\orderfrm.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_Starsdexe) $(CompInheritOptsAt_Starsdexe) -o$@ ..\library\orderfrm.cpp
|

tbitmap.obj :  ..\library\tbitmap.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_Starsdexe) $(CompInheritOptsAt_Starsdexe) -o$@ ..\library\tbitmap.cpp
|

tobjlist.obj :  ..\library\tobjlist.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_Starsdexe) $(CompInheritOptsAt_Starsdexe) -o$@ ..\library\tobjlist.cpp
|

mediadir.obj :  ..\library\mediadir.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_Starsdexe) $(CompInheritOptsAt_Starsdexe) -o$@ ..\library\mediadir.cpp
|

mediaply.obj :  ..\library\mediaply.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_Starsdexe) $(CompInheritOptsAt_Starsdexe) -o$@ ..\library\mediaply.cpp
|

tstrlist.obj :  ..\library\tstrlist.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_Starsdexe) $(CompInheritOptsAt_Starsdexe) -o$@ ..\library\tstrlist.cpp
|

tcanvas.obj :  ..\library\tcanvas.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_Starsdexe) $(CompInheritOptsAt_Starsdexe) -o$@ ..\library\tcanvas.cpp
|

ssthread.obj :  ..\library\ssthread.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_Starsdexe) $(CompInheritOptsAt_Starsdexe) -o$@ ..\library\ssthread.cpp
|

ssbase.obj :  ..\library\ssbase.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_Starsdexe) $(CompInheritOptsAt_Starsdexe) -o$@ ..\library\ssbase.cpp
|

metafile.obj :  ..\library\metafile.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_Starsdexe) $(CompInheritOptsAt_Starsdexe) -o$@ ..\library\metafile.cpp
|

sstools.obj :  ..\library\sstools.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_Starsdexe) $(CompInheritOptsAt_Starsdexe) -o$@ ..\library\sstools.cpp
|

printer.obj :  ..\library\printer.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_Starsdexe) $(CompInheritOptsAt_Starsdexe) -o$@ ..\library\printer.cpp
|

dib.obj :  ..\library\dib.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_Starsdexe) $(CompInheritOptsAt_Starsdexe) -o$@ ..\library\dib.cpp
|

decodgif.obj :  ..\library\decodgif.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_Starsdexe) $(CompInheritOptsAt_Starsdexe) -o$@ ..\library\decodgif.cpp
|

ssbase.res :  ..\library\ssbase.rc
  $(BRC32) -R @&&|
 $(IDE_ResFLAGS32) $(ROptsAt_Starsdexe) $(CompInheritOptsAt_Starsdexe)  -FO$@ ..\library\ssbase.rc
|
stars.res :  stars.rc
  $(BRC32) -R @&&|
 $(IDE_ResFLAGS32) $(ROptsAt_Starsdexe) $(CompInheritOptsAt_Starsdexe)  -FO$@ stars.rc
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


