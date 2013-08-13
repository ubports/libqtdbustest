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

#ifndef SUICIDALPROCESS_H_
#define SUICIDALPROCESS_H_

#include <QProcess>
#include <libqtdbustest/config.h>

namespace QtDBusTest {

class SuicidalProcessPrivate;

class Q_DECL_EXPORT SuicidalProcess: public QProcess {
Q_OBJECT
public:
	explicit SuicidalProcess(QObject *parent = 0);

	virtual ~SuicidalProcess();

	void setWatchdogCommand(const QString &watchdogCommand);

protected Q_SLOTS:
	virtual void setSuicidal();

private:
	QScopedPointer<SuicidalProcessPrivate> d;
};

} /* namespace QtDBusTest */
#endif /* SUICIDALPROCESS_H_ */
