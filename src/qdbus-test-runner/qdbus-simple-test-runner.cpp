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
#include <libqtdbustest/DBusTestRunner.h>
#include <QCoreApplication>
#include <csignal>

using namespace QtDBusTest;

static void exitQt(int sig) {
	Q_UNUSED(sig);
	QCoreApplication::exit(0);
}

class ExitListener: public QObject {
Q_OBJECT

public Q_SLOTS:
	void finished(int exitCode, QProcess::ExitStatus exitStatus) {
		if (exitStatus == QProcess::CrashExit && exitCode == 0) {
			exitCode = 1;
		}
		QCoreApplication::exit(exitCode);
	}
};

int main(int argc, char **argv) {
	QCoreApplication application(argc, argv);

	signal(SIGINT, &exitQt);
	signal(SIGTERM, &exitQt);

	DBusTestRunner runner;

	if (argc < 2) {
		qWarning() << "Usage:" << argv[0] << "COMMAND <ARGUMENT>";
		return 1;
	}

	QString command(QString::fromUtf8(argv[1]));

	QStringList arguments;
	for (int i(2); i < argc; ++i) {
		arguments << QString::fromUtf8(argv[i]);
	}

	QSharedPointer<QProcessDBusService> service(
			new QProcessDBusService("", QDBusConnection::SessionBus, command,
					arguments));

	runner.registerService(service);
	runner.startServices();

	ExitListener listener;
	QObject::connect(&service->underlyingProcess(),
			SIGNAL(finished(int, QProcess::ExitStatus)), &listener,
			SLOT(finished(int, QProcess::ExitStatus)));

	return application.exec();
}

#include "qdbus-simple-test-runner.moc"
