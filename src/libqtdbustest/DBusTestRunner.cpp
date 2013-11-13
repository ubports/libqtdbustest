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

#include <libqtdbustest/DBusTestRunner.h>
#include <libqtdbustest/SuicidalProcess.h>

namespace QtDBusTest {

class DBusTestRunnerPrivate {
public:
	DBusTestRunnerPrivate() :
			m_sessionConnection("DBusTestRunner-session"), m_systemConnection(
					"DBusTestRunner-system") {
	}

	QString m_sessionBus;

	QDBusConnection m_sessionConnection;

	SuicidalProcess m_sessionDBus;

	QString m_systemBus;

	QDBusConnection m_systemConnection;

	SuicidalProcess m_systemDBus;

	QList<DBusServicePtr> m_services;
};

DBusTestRunner::DBusTestRunner(const QString &dbusSessionConfigFile,
		const QString &dbusSystemConfigFile) :
		d(new DBusTestRunnerPrivate()) {

	// If we are already running inside a QDBus test environment
	if (qEnvironmentVariableIsSet("QDBUS_TEST_RUNNER_PARENT")) {
		// session bus setup
		d->m_sessionBus = QString::fromUtf8(
				qgetenv("DBUS_SESSION_BUS_ADDRESS"));
		d->m_sessionConnection = QDBusConnection::sessionBus();

		// system bus setup
		d->m_systemBus = QString::fromUtf8(qgetenv("DBUS_SYSTEM_BUS_ADDRESS"));
		d->m_systemConnection = QDBusConnection::systemBus();
	} else {
		// session bus setup

		d->m_sessionDBus.setProcessChannelMode(QProcess::MergedChannels);
		d->m_sessionDBus.start("dbus-daemon",
				QStringList() << "--config-file" << dbusSessionConfigFile
						<< "--print-address");
		Q_ASSERT(d->m_sessionDBus.waitForStarted());

		d->m_sessionDBus.waitForReadyRead();
		d->m_sessionBus = d->m_sessionDBus.readAll().trimmed();

		qputenv("DBUS_SESSION_BUS_ADDRESS", d->m_sessionBus.toUtf8());
		qputenv("DBUS_STARTER_ADDRESS", d->m_sessionBus.toUtf8());
		qputenv("DBUS_STARTER_BUS_TYPE", "session");

		d->m_sessionConnection = QDBusConnection::connectToBus(d->m_sessionBus,
				d->m_sessionBus);

		// system bus setup

		d->m_systemDBus.setProcessChannelMode(QProcess::MergedChannels);
		d->m_systemDBus.start("dbus-daemon",
				QStringList() << "--config-file" << dbusSystemConfigFile
						<< "--print-address");
		Q_ASSERT(d->m_systemDBus.waitForStarted());

		d->m_systemDBus.waitForReadyRead();
		d->m_systemBus = d->m_systemDBus.readAll().trimmed();

		qputenv("DBUS_SYSTEM_BUS_ADDRESS", d->m_systemBus.toUtf8());

		d->m_systemConnection = QDBusConnection::connectToBus(d->m_systemBus,
				d->m_systemBus);
	}
}

DBusTestRunner::~DBusTestRunner() {
	d->m_services.clear();
}

void DBusTestRunner::registerService(DBusServicePtr service) {
	d->m_services.push_back(service);
}

void DBusTestRunner::startServices() {
	for (DBusServicePtr service : d->m_services) {
		switch (service->busType()) {
		case QDBusConnection::SystemBus:
			service->start(d->m_systemConnection);
			break;
		case QDBusConnection::SessionBus:
			service->start(d->m_sessionConnection);
			break;
		case QDBusConnection::ActivationBus:
			qWarning() << "Unknown bus type";
			break;
		}
	}
}

const QDBusConnection & DBusTestRunner::sessionConnection() const {
	return d->m_sessionConnection;
}

const QString & DBusTestRunner::sessionBus() const {
	return d->m_sessionBus;
}

const QDBusConnection & DBusTestRunner::systemConnection() const {
	return d->m_systemConnection;
}

const QString & DBusTestRunner::systemBus() const {
	return d->m_systemBus;
}

}
