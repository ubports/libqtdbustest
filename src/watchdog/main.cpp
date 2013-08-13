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

#include <QCoreApplication>
#include <QDebug>
#include <csignal>

#include <watchdog/Watchdog.h>

using namespace QtDBusTest;

static void exitQt(int sig) {
	Q_UNUSED(sig);
	QCoreApplication::exit(0);
}

int main(int argc, char **argv) {
	QCoreApplication application(argc, argv);

	signal(SIGINT, &exitQt);
	signal(SIGTERM, &exitQt);

	if (argc != 3) {
		qWarning() << "Usage: " << argv[0] << "PARENT_PID TARGET_ID";
		exit(1);
	}

	Watchdog watchdog(QString::fromUtf8(argv[1]).toLongLong(),
			QString::fromUtf8(argv[2]).toLongLong());

	return application.exec();
}
