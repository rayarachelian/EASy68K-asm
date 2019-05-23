# EASy68k Command Line Assembler port for unix/Linux #

This is a port of just the assembler portion of the EASy68k project intended to be used on the command line so as to automate workflows with CI/CD, Makefiles, etc.

I've built this from the v5.15.4 source, because I wanted such a tool, and one did not exist, the latest is v5.16.01, so may repeat again with the newer version later on.
I've left in comments to many of the edits with // RA to make it easier to port future source releases in the same manner.

**NOTE:** I noticed that there are differences in both the L68 and S68 outputs of the two assemblers. I don't know whether this affects the output code or not, so please use with care and test it for yourself.

##To build the assembler:##

```
	cd ./ASM68Kv5.15.4
	make
	sudo make install # optional
```

##Invoking the command line assembler:##
```
asy68k cli 68000 assembler Version 5.15.04 based on source code from http://www.easy68k.com/
Distributed under the GNU General Public Use License. The software is not warranted in any way. Use at your own risk.
	
Ported to the *nix CLI so it can be used in a Makefile workflow w/o WINE by Ray Arachelian May 2019
	
Usage:
   asy68k {options} file1.X68 {file2.X68} ... {fileN.X68}
	
(Options with "default:" are enabled, use --no-{option} to turn off, i.e. --no-list)
	--list               default: produce listing (file.L68)
	--object             default: produce  S-Record object code file (file.S68)
	--bitfields          default: assemble bitfield instructions
	--warnings           default: show warnings in listing file
	--symbols            add symbol table to listing file
	--macroexpand        expand macros in listing file
	--structureexpand    expand structures in code listing file
	--expandconstants    expand constants in listing file
	
```
The options map to the assembly preferences in the IDE and the same default options from the IDE are enabled here. You can turn off any option by using the prefix `--no-` i.e. if for some reason you don't want a listing file you can pass `--no-list`.
Options should go before assembly source, as they take effect in order.  i.e.`asy68k foo.X68 --no-list bar.X68` will produce `foo.L68` and `foo.S68` and `bar.S68`, but not `bar.L68`.

I've named the binary asy68k because the "E" in EASy68k stands for "Editor" and the IDE around this has been removed. It could have been named ay68k but that sounds a bit too weird, and not assembler-y enough IMNSHO.

If you're not familiar with EASy68k, you should visit http://www.easy68k.com/ and follow their tutorials. Also the CHM help file included with the binary distribution is very helpful. If you're on Linux use one of the many CHM viewers to open it.
There's also a 68K quick reference PDF link in the CHM that's useful for beginners. This README is **not** going to address any of those topics, nor the syntax, it is intended for folks who already know how to use EASy68k and just want a command line assembler with the same macro syntax and outputs. But note that on *nix systems the line endings will be just LF's so no need to invoke dos2unix anymore.


### Porting Meta Discussion: ###

Feel free to stop reading right here unless you care about what was needed to port this assembler.

"But Ray, aren't you too busy with other stuff to work on such a thing?"
*"Why, yes, yes, I am"* ... but, then again ...
*"Ferb, I know what we're going to do today!"*

While perhaps, a better assembler would be asl ( http://john.ccac.rwth-aachen.de:8000/as/ ) which handles more than just 68000, there's a lot of pre-existing Lisa 68000 assembly code that uses this assembler ( i.e. https://github.com/stepleton/NeoWidEx ) and ofc my own NanoBug. Because of its macro language, transforming that EASy68k source to ASL is not trivial. I know, I gave it a go. I haven't quite given up on porting it that way, but meanwhile I thought this might be useful for other things too.

There does seem to be some interest in having a command line version of the EASy68k assembler, and not just from me, (see: http://www.easy68k.com/EASy68Kforum/viewtopic.php?f=3&t=1629 ) - and there's also some interest in porting/automating the simulator too.

I want to be able to build toolchains that I can invoke from a shell or Makefile to build the Lisa tools I use without having to invoke WINE and have to manually use a GUI. Nothing wrong with doing that while you're developing code, but once you have the code, you want to be able to automate testing and building, or perhaps you want to use some other IDE or editor, and not have to go back and forth between windows land and linux land, what with the CRLFs and non-case sensitive names and what not.

Some of the issues encountered in porting are related to using the old Borland C++ toolchain, which needed some functions replaced with standard ones, and the removal of GUI ones. There were a few issues here and there, such as things being defined as both bool's and int's and stuff like that. In one instance there was a conflicting shared variable named the same, one was a static invocation which I renamed for clarity. A whole bunch of const int's exist that were likely originally #defines, and so I switched them back as that's cleaner to me.

The biggest bug I've encountered is that the assembler internally uppercases everything but string constants, but that included file names used for the INCLUDE and INCBIN directives which caused File Not Found errors! I've had to put in some hacks around that to allow mixed case file names, to make it work on Linux as file named on Linux are case sensitive. I've also added code to remove CR's from X68 source that was originally edited in Windows, though this is less of an issue.

Some of the C++ vector syntax was different, but that seems to be a non-issue once edited. 
Of course the calls to the IDE to popup warning/error dialog messages had to be commented out.
The output of the date format in the L68 files is different, wasn't worth the trouble to get it exactly the same.

I've also added a quick and dirty command line switch set to enable/disable assembler settings, really it should use getopt(), but this was meant as a, "just good enough" for what I need.

(I mention Linux here, but in all likelyhood this will build and work fine on mac os X and \*BSD's, however as my main machine these days runs Linux...
I've also not tried to build this on Windows as I don't have any windows machines and I'm not so sure ReactOS in VirtualBox counts, so it would at least require a new Makefile (or a VS project file?), but perhaps an easy way would be to use cygwin and the mingw compilers?)

Because of the age of the original code, and the sizings of ints, shorts, and original Intel/16/32-bit Windows-centric/endianness moving to 64 bit, there may be  other issues, so I'm not sure it will work perfectly, nor can I say whether it will work on big-endian platforms such as PPC/SPARC.

I'm not currently planning to port either the EASy68k IDE nor the SIM68K in this way because it would need a lot more rewriting of the Borland/Windows GUI functions, but perhaps that would be an interesting project to port to wxWidgets or Qt in the far future, but I've got too many far higher inators on the to-do stack.

*"Hey! Where's Perry?"*