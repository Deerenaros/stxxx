//    include/utils/locked.h is part of STx
//
//    STx is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    STx is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with STx.  If not, see <http://www.gnu.org/licenses/>.

#ifndef LOCKED_H
#define LOCKED_H

#include <QMutex>

#define COMBINE(X, Y) X##Y

#define LOCKED Locked COMBINE(_LOCK, __LINE__) (m_mutex);
#define LOCKEDX(...) { m_mutex.lock(); __VA_ARGS__; m_mutex.unlock(); }
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
