#ifndef LOCKED_H
#define LOCKED_H

#include <QMutex>

#define COMBINE(X, Y) X##Y

#define LOCKED Locked COMBINE(_LOCK, __LINE__) (m_mutex);
#define LOCKEDX(...) m_mutex.lock(); __VA_ARGS__; m_mutex.unlock()
#define LOCKABLE QMutex m_mutex;

struct Locked {
    Locked(QMutex& mutex) : m_mutex(mutex) {
        m_mutex.lock();
    }

    ~Locked() {
        m_mutex.unlock();
    }

private:
    QMutex& m_mutex;
};

#endif // LOCKED_H
