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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace std;
using namespace testing;
using namespace QtDBusTest;

namespace {

class TestDBusTestRunner: public Test {
protected:
	TestDBusTestRunner() :
			dbusTestRunner(TEST_DBUS_SESSION_CONFIG_FILE,
			TEST_DBUS_SYSTEM_CONFIG_FILE) {
	}

	virtual ~TestDBusTestRunner() {
	}

	DBusTestRunner dbusTestRunner;
};

TEST_F(TestDBusTestRunner, StartsSessionService) {
	QSharedPointer<QProcessDBusService> process(
			new QProcessDBusService("test.session.name",
					QDBusConnection::SessionBus, "/usr/bin/python3",
					QStringList() << "-m" << "dbusmock" << "test.session.name"
							<< "/test/object" << "test.Interface"));

	dbusTestRunner.registerService(process);
	dbusTestRunner.startServices();

	QProcess pgrep;
	pgrep.start("ps",
			QStringList() << "--no-headers" << "--pid"
					<< QString::number(process->pid()) << "-o" << "args");
	pgrep.waitForFinished();
	pgrep.waitForReadyRead();

	EXPECT_EQ(
			"/usr/bin/python3 -m dbusmock test.session.name /test/object test.Interface",
			QString::fromUtf8(pgrep.readAll().trimmed()).toStdString());
}

TEST_F(TestDBusTestRunner, StartsSystemService) {
	QSharedPointer<QProcessDBusService> process(
			new QProcessDBusService("test.system.name",
					QDBusConnection::SystemBus, "/usr/bin/python3",
					QStringList() << "-m" << "dbusmock" << "-s"
							<< "test.system.name" << "/test/object"
							<< "test.Interface"));

	dbusTestRunner.registerService(process);
	dbusTestRunner.startServices();

	QProcess pgrep;
	pgrep.start("ps",
			QStringList() << "--no-headers" << "--pid"
					<< QString::number(process->pid()) << "-o" << "args");
	pgrep.waitForFinished();
	pgrep.waitForReadyRead();

	EXPECT_EQ(
			"/usr/bin/python3 -m dbusmock -s test.system.name /test/object test.Interface",
			QString::fromUtf8(pgrep.readAll().trimmed()).toStdString());
}

TEST_F(TestDBusTestRunner, SetsEnvironmentVariables) {
	EXPECT_EQ(dbusTestRunner.sessionBus(), qgetenv("DBUS_SESSION_BUS_ADDRESS"));
	EXPECT_EQ(dbusTestRunner.systemBus(), qgetenv("DBUS_SYSTEM_BUS_ADDRESS"));
}

} // namespace
