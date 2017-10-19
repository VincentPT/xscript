# Sample makefile

cc = cl
link = link
outdir = dll^\x64^\Release^\
cflags= /c /WX- /Zc:forScope /Gd /Oi /MD /EHsc /nologo
lflags= /NXCOMPAT /DYNAMICBASE /DLL /MACHINE:X64 /INCREMENTAL /SUBSYSTEM:WINDOWS /NOLOGO
cdefines=/DNDEBUG /D_WINDOWS /D_USRDLL /D_WINDLL /D_UNICODE /DUNICODE /DFFSCRIPT_DYNAMIC /DFFSCIPT_EXPORTS
llibs="kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib"

project= ffscriptd
target = $(project).dll
#objects = ExpresionParser.obj Expression.obj


all: $(target)

.cpp.obj:
	$(cc) $(cflags) $(cdefines) $< /Fo$(outdir)

$(target): $(objects)
	$(link) $(lflags) /LIBPATH:$(outdir) /out:$(outdir)$(target) /IMPLIB:$(outdir)$(target).lib $(objects) $(llibs)
  
clean:
  del /q $(outdir)*.*