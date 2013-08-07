/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
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

class TestQProcessDBusService: public Test {
protected:
	TestQProcessDBusService() {
	}

	virtual ~TestQProcessDBusService() {
	}

	DBusTestRunner dbusTestRunner;
};

TEST_F(TestQProcessDBusService, WaitsForServiceAppeared) {
	QProcessDBusService process("test.name", QDBusConnection::SessionBus,
			"python3",
			QStringList() << "-m" << "dbusmock" << "test.name" << "/test/object"
					<< "test.Interface");

	process.start(dbusTestRunner.sessionConnection());

	QProcess pgrep;
	pgrep.start("ps",
			QStringList() << "--no-headers" << "--pid"
					<< QString::number(process.pid()) << "-o" << "args");
	pgrep.waitForFinished();
	pgrep.waitForReadyRead();

	EXPECT_EQ("python3 -m dbusmock test.name /test/object test.Interface",
			QString::fromUtf8(pgrep.readAll().trimmed()).toStdString());
}

} // namespace
