// vim: set et ts=4 sw=4


class Spinlock {

public:
    // Konstruktor; Initialisierung des Spinlocks als ungesperrt.
    Spinlock();
    // Betritt den gesperrten Abschnitt. Ist dieser besetzt,
    // so wird solange aktiv gewartet, bis er betreten werden kann.
    void lock();
    // Gibt den gesperrten Abschnitt wieder frei.
    void unlock();
private:
    bool locked;
};
