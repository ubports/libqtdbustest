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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace std;
using namespace testing;
using namespace QtDBusTest;

namespace {

class TestSuicidalProcess: public Test {
protected:
	TestSuicidalProcess() {
		process.setWatchdogCommand(TEST_QTDBUSTEST_WATCHDOG_BIN);
	}

	virtual ~TestSuicidalProcess() {
		process.kill();
		process.waitForFinished();
	}

	SuicidalProcess process;
};

TEST_F(TestSuicidalProcess, BehavesLikeNormalQProcess) {
	process.start("sleep", QStringList() << "5");

	QProcess pgrep;
	pgrep.start("ps",
			QStringList() << "--no-headers" << "--pid"
					<< QString::number(process.pid()) << "-o" << "args");
	pgrep.waitForFinished();
	pgrep.waitForReadyRead();

	EXPECT_EQ("sleep 5",
			QString::fromUtf8(pgrep.readAll().trimmed()).toStdString());
}

} // namespace
