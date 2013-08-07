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

#include <libqtdbustest/QProcessDBusService.h>

#include <QDebug>
#include <QSignalSpy>
#include <QDBusServiceWatcher>

namespace QtDBusTest {

class QProcessDBusServicePrivate {

public:
	QProcessDBusServicePrivate(const QString &program,
			const QStringList &arguments) :
			m_program(program), m_arguments(arguments) {
	}

	QString m_program;

	QStringList m_arguments;

	QProcess m_process;
};

QProcessDBusService::QProcessDBusService(const QString &interface,
		QDBusConnection::BusType busType, const QString &program,
		const QStringList &arguments) :
		DBusService(interface, busType), p(
				new QProcessDBusServicePrivate(program, arguments)) {
}

QProcessDBusService::~QProcessDBusService() {
	p->m_process.terminate();
	p->m_process.waitForFinished();

//	p->m_process.waitForReadyRead();
//	qDebug() << p->m_process.readAll();
}

void QProcessDBusService::start(const QDBusConnection &connection) {
	QDBusServiceWatcher watcher(name(), connection,
			QDBusServiceWatcher::WatchForRegistration);
	QSignalSpy spy(&watcher,
			SIGNAL(serviceOwnerChanged(const QString &,const QString &,const QString &)));

	p->m_process.setProcessChannelMode(QProcess::MergedChannels);
	p->m_process.start(p->m_program, p->m_arguments);

	spy.wait();
	if (spy.empty()) {
		p->m_process.waitForReadyRead(50);
		qWarning() << "Process " << p->m_program << " with arguments "
				<< p->m_arguments << " for service " << name()
				<< "failed to start";
		qWarning() << p->m_process.readAll();
		return;
	}
	QVariantList arguments(spy.takeFirst());
	if (name() != arguments.first().toString()) {
		qWarning() << "Process " << p->m_program << " with arguments "
				<< p->m_arguments << " for interface " << name()
				<< " - incorrect service appeared";
	}
}

Q_PID QProcessDBusService::pid() const {
	return p->m_process.pid();
}

}
