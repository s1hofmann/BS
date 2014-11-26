
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse Queue
 */
#ifndef __queue_include__
#define __queue_include__

/*! \brief Die Klasse Queue realisiert eine einfach verkettete Liste von
 *  Objekten mit next Zeiger.
 *
 *  Objekte die verkettet werden sollen erben von QueueEntry und geben ihren eigenen
 *  Typ als Template-Parameter an.
 *  Beispiel: class Foo : public QueueEntry<Foo> {};
 *  Damit lassen sich Zeiger auf Instanzen von Foo z.B. in die Queue:
 *  Queue<Foo> myqueue;
 *  einfügen.
 *  Zur Implementierung: tail verweist nicht, wie oft üblich, auf das letzte
 *  Element der Liste, sondern
 *  auf den next Zeiger des letzten Elements, bzw., solange die Liste noch leer
 *  ist, auf den head Zeiger der Liste. Dadurch muss beim Einfügen eines
 *  Elements an die Liste nicht überprüft werden, ob bereits Elemente in ihr
 *  enthalten sind. Beim Entfernen von Elementen kann auf die Fallunterscheidung
 *  allerdings nicht verzichtet werden.
 */
template<typename T>
class Queue
{
private:
    Queue(const Queue<T> &copy); // Verhindere Kopieren
    T *head;
    T **tail;

	static bool eqCmp(T *a, T *b) {
		return a == b;
	}

public:
    /*! \brief Der Konstruktor initialisiert die Liste als leere Liste
     */
    Queue() : head(0), tail(&head) {}

    /*! \brief Das Listenelement item wird an das Ende der Liste angefügt.
     *  \param item Listenelement, welches angefügt werden soll.
     */
    void enqueue(T *item) {
        item->next = 0;     // Das neue Element besitzt noch keinen Nachfolger.
        *tail = item;       // Das Element an das Ende der Liste anfuegen
        tail = &item->next; // und den tail Zeiger aktualisieren.
    }

    /*! \brief Liefert das erste Element der Liste und entfernt es gleichzeitig
     *  aus dieser.
     *  \return entferntes Kopfelement oder 0 wenn die Liste leer ist.
     */
    T* dequeue() {
        T *out;
        if((out = head) && !(head = out->next)) tail = &head;
        return out;
    }

    /*! \brief Mit dieser Methode kann das angegebene Element item aus der
     *  Liste entfernt werden, unabhängig von seiner Position dort.
     *  Verglichen werden die Pointer-Adressen, optional kann eine
     *  Vergleichsfunktion angegeben werden.
     *  \param item Element, welches entfernt werden soll.
     *  \param cmp Vergleichsfunktion.
     *  \return Gibt das entfernte Element zurück, \b 0 wenn keins entfernt
     *  wurde.
     */
	T* remove(T *item, bool (*cmp)(T*,T*) = eqCmp) {
        T **prev = &head;
        for(T *current = head; current != 0; current = current->next) {
            if(cmp(item, current)) {
                *prev = current->next;
                if(&current->next == tail) tail = prev;
                return current;
            }
            prev = &current->next;
        }
        return 0;
    }

    /*! \brief Fügt das \b item am Anfang der Liste ein.
     *  \param item Einzufügendes Element.
     */
    void insert_first(T *item) {
        item->next = head;
        head = item;

        if(tail == &head) tail = &item->next;
    }

    /*! \brief Fügt das Element new_item hinter dem Element old_item in die
     *  Liste ein.
     *  \param old_item Element, nach dem eingefügt werden soll.
     *  \param new_item Einzufügendes Element.
     */
    void insert_after(T *old_item, T *new_item) {
        new_item->next = old_item->next;
        old_item->next = new_item;

        if((*tail) == new_item)
            tail = &(new_item->next);
    }

    /*! \brief Liefert das erste Element der Liste ohne es zu entfernen.
     *  \return Erstes Element der Liste.
     */
    T* first() {
        T *out;
        out = head;
        return out;
    }
};

#endif
