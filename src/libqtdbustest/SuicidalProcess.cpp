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
#include <QCoreApplication>

namespace QtDBusTest {

class SuicidalProcessPrivate {
public:
	SuicidalProcessPrivate() :
			m_watchdogCommand(QTDBUSTEST_WATCHDOG_BIN) {
	}

	~SuicidalProcessPrivate() {
	}

	QString m_watchdogCommand;

	QProcess m_watchdog;
};

SuicidalProcess::SuicidalProcess(QObject *parent) :
		QProcess(parent), d(new SuicidalProcessPrivate()) {
	connect(this, SIGNAL(started()), this, SLOT(setSuicidal()));
}

SuicidalProcess::~SuicidalProcess() {
	d->m_watchdog.kill();
	d->m_watchdog.waitForFinished();

	terminate();
	waitForFinished();
}

void SuicidalProcess::setSuicidal() {
	d->m_watchdog.start(d->m_watchdogCommand,
			QStringList() << QString::number(QCoreApplication::applicationPid())
					<< QString::number(pid()));
}

void SuicidalProcess::setWatchdogCommand(const QString &watchdogCommand) {
	d->m_watchdogCommand = watchdogCommand;
}

} /* namespace QtDBusTest */
