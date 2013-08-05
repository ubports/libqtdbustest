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

#include <libqtdbustest/SuicidalProcess.h>
#include <libqtdbustest/config.h>
#include <QCoreApplication>

namespace QtDBusTest {

SuicidalProcess::SuicidalProcess() {
	connect(this, SIGNAL(started()), this, SLOT(setSuicidal()));
}

SuicidalProcess::~SuicidalProcess() {
	m_watchdog.kill();
	m_watchdog.waitForFinished();
}

void SuicidalProcess::setSuicidal() {
	m_watchdog.start(QTDBUSTEST_WATCHDOG_BIN,
			QStringList() << QString::number(QCoreApplication::applicationPid())
					<< QString::number(pid()));
}

} /* namespace QtDBusTest */
