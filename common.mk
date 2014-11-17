
# --------------------------------------------------------------------------
# Einstellungen in Abhaengigkeit vom Generierungssystem:
#
#    ASMOBJFORMAT: Ausgabeformat fuer den Assembler. Das Format muss dem
#                  jeweiligen Format des verwendeten C++ Compilers angepasst
#                  werde, damit gemischte Objektdateien gelinkt werden koennen.
#    OBJDIR:       Verzeichnis, in dem die Objektdateien abgelegt werden
#                  sollen.
#    DBGOBJDIR:    Verzeichnis, in dem die mit Debuginformationen kompilierten
#                  Objektdateien abgelegt werden sollen.
#    DEPDIR:       Verzeichnis, in dem die Abhaengigkeitsdateien abgelegt werden
#                  sollen
#    ASM:          Zu benutzender Assembler
#    CC/CXX:       Zu benutzender C/C++-Compiler
#    AR:           Zu benutzender Archivierer
#    CFLAGS:       Flags fuer den C-Compileraufruf
#    CXXFLAGS:     Flags fuer den C++-Compileraufruf
#    LD:           Zu benutzender Linker
#    LDFLAGS:      Flags fuer den Linkeraufruf
#    LDLIBS:       Dazuzulinkende Bibliotheken
#    LDHEAD:       Am Anfang zu linkende Dateien
#    LDTAIL:       Am Ende zu linkende Dateien

VERBOSE = @
ASMOBJFORMAT = elf
OBJDIR = ./build
DBGOBJDIR = ./build-noopt
DEPDIR = ./dep
DBGDEPDIR = ./dep-noopt
ASM = nasm
CXX = g++
LD = ld
QEMU = qemu-system-x86_64

WARNFLAGS = -Wall -Wextra -Werror -Wno-error=unused-parameter
#OPTFLAGS = -O3 -fomit-frame-pointer
OPTFLAGS = -O0
FLOATINGPOINT = -mno-mmx -mno-sse
STANDALONEFLAGS = -ffreestanding -fno-builtin -nostartfiles -nodefaultlibs -nostdlib -nostdinc
CXXFLAGS = -m32 -I. $(OPTFLAGS) $(WARNFLAGS) $(STANDALONEFLAGS) $(FLOATINGPOINT) -Wno-non-virtual-dtor -fno-rtti -fno-exceptions -Wno-write-strings -fno-stack-protector -mno-red-zone -std=c++11
ASMFLAGS = -f $(ASMOBJFORMAT)
LDFLAGS = -melf_i386
LDHEAD = $(shell g++ -m32 --print-file-name=crti.o && g++ -m32 --print-file-name=crtbegin.o)
LDTAIL = $(shell g++ -m32 --print-file-name=crtend.o && g++ -m32 --print-file-name=crtn.o)
AR = ar
QEMUCPUs = 4
QEMUINITRD = /dev/null

# Namen der Unterverzeichnisse mit den Quelltexten
VPATH = $(sort $(dir $(STARTUP_SOURCE) $(CC_SOURCES) $(ASM_SOURCES)))

# -------------------------------------------------------------------------
# Listen mit den Objektdateien, die beim Kompilieren entstehen:
# (Die Eingabevariablen werden vom eigentlichen Makefile zwar erst spaeter
# definiert, sind aber zum Auswertezeitpunkt *dieser* Variablen schon
# verfuegbar.)
STARTUP_OBJECT = $(addprefix $(OBJDIR)/,$(patsubst %.asm,_%.o, $(notdir $(STARTUP_SOURCE))))
CC_OBJECTS = $(notdir $(CC_SOURCES:.cc=.o))
DEP_FILES = $(patsubst %.o,$(DEPDIR)/%.d,$(CC_OBJECTS))
ASM_OBJECTS = $(patsubst %.asm,_%.o, $(notdir $(ASM_SOURCES)))
OBJPRE = $(addprefix $(OBJDIR)/,$(ASM_OBJECTS) $(CC_OBJECTS))

# --------------------------------------------------------------------------
# Standardrezept zur Erzeugung der Abhaengigkeitsdateien
$(DEPDIR)/%.d : %.cc $(MAKEFILE_LIST)
	@echo "DEP		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	$(VERBOSE) $(CXX) $(CXXFLAGS) -MM -MT $(OBJDIR)/$*.o -MF $@ $<

# --------------------------------------------------------------------------
# Standardrezept zur Erzeugung von Objektdateien
$(OBJDIR)/%.o : %.cc $(MAKEFILE_LIST)
	@echo "CXX		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	$(VERBOSE) $(CXX) -c $(CXXFLAGS) -o $@ $<

$(OBJDIR)/_%.o : %.asm $(MAKEFILE_LIST)
	@echo "ASM		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	$(VERBOSE) $(ASM) $(ASMFLAGS) -o $@ $<

# --------------------------------------------------------------------------
# Standardrezept 'clean' loescht das generierte System, die Objektdateien und
# die Abhaengigkeitsdateien

clean:	
	@echo "RM		$(OBJDIR)"
	$(VERBOSE) rm -rf $(OBJDIR)
	@echo "RM		$(DBGOBJDIR)"
	$(VERBOSE) rm -rf $(DBGOBJDIR)
	@echo "RM		$(DEPDIR)"
	$(VERBOSE) rm -rf $(DEPDIR)
	@echo "RM		$(DBGDEPDIR)"
	$(VERBOSE) rm -rf $(DBGDEPDIR)

# --------------------------------------------------------------------------
# Rezept fuer rekursiven Aufruf mit deaktivierten Optimierungen
%-noopt:
	make OBJDIR="$(DBGOBJDIR)" DEPDIR="$(DBGDEPDIR)" OPTFLAGS="-O0 -g" $*

# --------------------------------------------------------------------------
# Standardrezepte zum Ausfuehren und Debuggen
# --------------------------------------------------------------------------

qemu: all
	$(QEMU) -kernel $(QEMUKERNEL) -initrd $(QEMUINITRD) -k en-us -smp $(QEMUCPUs)

# --------------------------------------------------------------------------
# 'qemu-gdb' ruft den qemu-Emulator mit aktiviertem GDB-Stub mit dem System
# auf, sodass es per GDB oder DDD inspiziert werden kann.
qemu-gdb: all
	@# Studenten, ignoriert diesen Kommentar: Doppelte Backticks/Hochkommata sind notwendig damits im m4 nicht kaputt geht
	file=`mktemp` \
	     && trap "rm -f -- '$$file'" INT EXIT \
	     && echo "target remote | exec $(QEMU) -gdb stdio -kernel $(QEMUKERNEL) -initrd $(QEMUINITRD) -k en-us -smp $(QEMUCPUs) -S" > "$$file" \
	     && gdb -x "$$file" $(QEMUKERNEL)

# --------------------------------------------------------------------------
# 'qemu-ddd' ruft den qemu-Emulator mit aktiviertem GDB-Stub mit dem System
# auf und startet direkt ddd.
qemu-ddd: all
	@# Studenten, ignoriert diesen Kommentar: Doppelte Backticks/Hochkommata sind notwendig damits im m4 nicht kaputt geht
	file=`mktemp` \
	     && trap "rm -f -- '$$file'" INT EXIT \
	     && echo "target remote | exec $(QEMU) -gdb stdio -kernel $(QEMUKERNEL) -initrd $(QEMUINITRD) -k en-us -smp $(QEMUCPUs) -S" > "$$file" \
	     && ddd --gdb -x "$$file" $(QEMUKERNEL)

# --------------------------------------------------------------------------
# 'kvm' startet euer System in einem QEMU mit kvm-Unterstützung, um auch echt
# parallel arbeitende virtuelle CPUs nutzen zu können. Maximal sind zwei
# virtuelle CPUs möglich. Dieses Kommando ist lediglich ein Wrapper um kvm, und
# funktioniert nur im CIP-Pool, da es dort aus Sicherheitsgründen nicht möglich
# ist kvm direkt selbst zu starten.  Wenn ihr zuhause mit kvm testen wollt,
# müsst ihr das selbst aufrufen, was genauso funktioniert wie der Aufruf von
# QEMU in den beiden vorherigen Makefiletargets.
kvm: all
	/usr/local/bin/kvm-sgid-wrap mpstubs $(QEMUKERNEL) $(QEMUCPUs) $(QEMUINITRD)

kvm-local: all
	$(QEMU) -enable-kvm -kernel $(QEMUKERNEL) -initrd $(QEMUINITRD) -k en-us -smp $(QEMUCPUs)

# --------------------------------------------------------------------------
# Das Image wirds ins Verzeichnis des tftp-servers kopiert, um es ueber
# pxelinux auf den Testrechnern starten zu können.
netboot: all
	install -m 644 $(QEMUKERNEL) /proj/i4bs/tftpboot/kernels/$(shell id -run)/kernel	
	initrd="$(QEMUINITRD)"; if [ ! -s "$$initrd" ]; then initrd="$(QEMUKERNEL)"; fi; install -m 644 "$$initrd" /proj/i4bs/tftpboot/kernels/$(shell id -run)/initrd.img

# --------------------------------------------------------------------------
# Einbindung der Abhaengigkeitsdateien

ifneq ($(MAKECMDGOALS),clean)
-include $(DEP_FILES)
endif

.PHONY: all clean qemu qemu-gdb qemu-ddd kvm netboot
