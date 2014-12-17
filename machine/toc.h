// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält die Struktur toc.
 */

#ifndef TOC_INCLUDE
#define TOC_INCLUDE

/*! \brief Die Struktur toc dient dazu, bei einem Koroutinenwechsel die Werte
 *  der nicht-flüchtigen Register zu sichern.
 *
 *  Beim GNU C Compiler sind eax, ecx und edx flüchtige Register, die bei
 *  Funktionsaufrufen und somit auch bei einem Koroutinenwechsel keine später
 *  noch benötigten Werte haben duerfen. Daher muss in der Struktur toc auch
 *  kein Platz für sie bereitgestellt werden.
 *
 *  Achtung: Für den Zugriff auf die Elemente der Struktur toc aus einer
 *  Assemblerfunktion heraus werden in der Datei toc.inc Namen für die
 *  benötigten Abstände der einzelnen Elemente zum Anfang der Struktur
 *  definiert. Damit dann auch auf die richtigen Elemente zugegriffen wird,
 *  müssen sich die Angaben von toc.h und toc.inc exakt entsprechen. Wer also
 *  toc.h ändert, muss auch toc.inc anpassen (und umgekehrt).
 */
struct toc {
    public:
    void *ebx;
    void *esi;
    void *edi;
    void *ebp;
    void *esp;
} __attribute__ ((packed));

class Thread;

void toc_settle (struct toc *regs, void *tos, void (*kickoff)(Thread*), Thread *object);

extern "C" {
    void toc_go (struct toc *regs);
    void toc_switch (struct toc *regs_now, struct toc *reg_then);
}

#endif

