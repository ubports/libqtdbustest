/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of version 3 of the GNU Lesser General Public License as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Pete Woods <pete.woods@canonical.com>
 */

#ifndef LIBQTDBUSTEST_WATHDOG_H_
#define LIBQTDBUSTEST_WATHDOG_H_

#include <QObject>
#include <QProcess>
#include <QTimer>

namespace QtDBusTest {

class Q_DECL_EXPORT Watchdog: public QObject {
Q_OBJECT
public:
	Watchdog(Q_PID parentPid, Q_PID childPid);

	virtual ~Watchdog();

protected Q_SLOTS:
	void timeout();

protected:
	Q_PID m_parentPid;

	Q_PID m_childPid;

	QTimer m_timer;
};

} /* namespace QtDBusTest */
#endif /* LIBQTDBUSTEST_WATHDOG_H_ */
