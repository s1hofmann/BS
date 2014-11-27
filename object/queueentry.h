
// vim: set et ts=4 sw=4:

/*! \file
 *  \brief Enthält die Klasse QueueEntry
 */
#ifndef __queueentry_include__
#define __queueentry_include__

/* Forward declaration: */
template<typename T> class Queue;

/*! \brief Verkettungszeiger zum Einfügen eines Objektes in eine einfach
 *  verkettete Liste.
 *
 *  Die Klasse QueueEntry stellt einen Verkettungszeiger auf ein Element mit dem
 *  Typ T zur Verfügung und ist damit Basis aller Klassen, deren Instanzen in
 *  Listen (Queue Objekten) verwaltet werden sollen.
 */
template<typename T>
class QueueEntry
{
    /* Erlaube Klasse Queue Zugriff auf den privaten Member next. */
    friend class Queue<T>;

private:
    /*! \brief next Gibt das nächste Element der Liste an.
     */
    T *next;

public:
    /*! \brief Gibt das nächste Element in der Liste zurück (read-only-Zugriff).
     */
    T *getnext() {
        return next;
    }
};

#endif

