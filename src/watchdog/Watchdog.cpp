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

#include <watchdog/Watchdog.h>
#include <QTimer>
#include <QCoreApplication>
#include <csignal>

namespace QtDBusTest {

Watchdog::Watchdog(Q_PID parentPid, Q_PID childPid) :
		m_parentPid(parentPid), m_childPid(childPid) {
	m_timer.setInterval(1000);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
	m_timer.start();
}

Watchdog::~Watchdog() {
}

void Watchdog::timeout() {
	// if the parent process is dead
	if (kill(m_parentPid, 0) != 0) {
		kill(m_childPid, SIGKILL);
		QCoreApplication::exit(0);
	}
}

} /* namespace QtDBusTest */
