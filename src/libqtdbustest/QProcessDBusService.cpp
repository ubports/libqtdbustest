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
#include <libqtdbustest/QProcessDBusService.h>

#include <QDebug>
#include <QSignalSpy>
#include <QDBusServiceWatcher>

#include <stdexcept>

namespace QtDBusTest {

namespace {
	int timeoutInSeconds() {
		const int defaultTimeout = 15;
		QByteArray timeoutString = qgetenv("QDBUS_DBUS_START_TIMEOUT");
		if (!timeoutString.isEmpty()) {
			bool ok;
			const int timeout = timeoutString.toInt(&ok);
			if (ok) {
				return timeout;
			}
		}
		return defaultTimeout;
	}
} // anonymous namespace

class QProcessDBusServicePrivate {
public:
	QProcessDBusServicePrivate(const QString &program,
			const QStringList &arguments) :
			m_program(program), m_arguments(arguments) {
	}

	QString m_program;

	QStringList m_arguments;

	SuicidalProcess m_process;
};

QProcessDBusService::QProcessDBusService(const QString &name,
		QDBusConnection::BusType busType, const QString &program,
		const QStringList &arguments) :
		DBusService(name, busType), p(
				new QProcessDBusServicePrivate(program, arguments)) {
}

QProcessDBusService::~QProcessDBusService() {
}

void QProcessDBusService::start(const QDBusConnection &connection) {
	QDBusServiceWatcher watcher(name(), connection,
			QDBusServiceWatcher::WatchForRegistration);
	QSignalSpy spy(&watcher,
			SIGNAL(serviceOwnerChanged(const QString &,const QString &,const QString &)));

	QProcessEnvironment environment(QProcessEnvironment::systemEnvironment());
	environment.insert("QDBUS_TEST_RUNNER_PARENT", "1");
	p->m_process.setProcessEnvironment(environment);
	p->m_process.setProcessChannelMode(QProcess::ForwardedChannels);
	p->m_process.start(p->m_program, p->m_arguments);

	if (name().isEmpty()) {
		if (!p->m_process.waitForStarted()) {
			QString error = "Process [" + p->m_program + "] for service ["
					+ name() + "] failed to start";
			throw std::logic_error(error.toStdString());
		}
	} else {
		spy.wait(timeoutInSeconds() * 1000);
		if (spy.empty()) {
			QString error = "Process [" + p->m_program + "] for service ["
					+ name() + "] failed to appear on bus";
			throw std::logic_error(error.toStdString());
		}
	}
}

Q_PID QProcessDBusService::pid() const {
	return p->m_process.pid();
}

const QProcess & QProcessDBusService::underlyingProcess() const {
	return p->m_process;

}

}

