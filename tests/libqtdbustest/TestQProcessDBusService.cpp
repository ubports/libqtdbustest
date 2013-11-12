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

#include <stdexcept>

using namespace std;
using namespace testing;
using namespace QtDBusTest;

namespace {

class TestQProcessDBusService: public Test {
protected:
	TestQProcessDBusService() :
			dbusTestRunner(TEST_DBUS_SESSION_CONFIG_FILE,
			TEST_DBUS_SYSTEM_CONFIG_FILE) {
	}

	virtual ~TestQProcessDBusService() {
	}

	DBusTestRunner dbusTestRunner;
};

TEST_F(TestQProcessDBusService, WaitsForServiceAppeared) {
	QProcessDBusService process("test.name", QDBusConnection::SessionBus,
			"/usr/bin/python3",
			QStringList() << "-m" << "dbusmock" << "test.name" << "/test/object"
					<< "test.Interface");

	process.start(dbusTestRunner.sessionConnection());

	QProcess pgrep;
	pgrep.start("ps",
			QStringList() << "--no-headers" << "--pid"
					<< QString::number(process.pid()) << "-o" << "args");
	pgrep.waitForFinished();
	pgrep.waitForReadyRead();

	EXPECT_EQ(
			"/usr/bin/python3 -m dbusmock test.name /test/object test.Interface",
			QString::fromUtf8(pgrep.readAll().trimmed()).toStdString());
}

TEST_F(TestQProcessDBusService, ThrowsErrorForFailToStart) {
	QProcessDBusService process("test.name", QDBusConnection::SessionBus,
			"/usr/bin/python3",
			QStringList() << "-m" << "dbusmock" << "not.test.name"
					<< "/test/object" << "test.Interface");

	ASSERT_THROW(process.start(dbusTestRunner.sessionConnection()),
			std::logic_error);
}

} // namespace
