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

#ifndef QTDBUSTEST_QPROCESSDBUSSERVICE_H_
#define QTDBUSTEST_QPROCESSDBUSSERVICE_H_

#include <libqtdbustest/DBusService.h>

#include <QProcess>
#include <QString>
#include <QStringList>

namespace QtDBusTest {

class QProcessDBusServicePrivate;

class Q_DECL_EXPORT QProcessDBusService: public DBusService {

public:
	explicit QProcessDBusService(const QString &name,
			QDBusConnection::BusType busType, const QString &program,
			const QStringList &arguments);

	virtual ~QProcessDBusService();

	virtual void start(const QDBusConnection &connection);

	virtual Q_PID pid() const;

	virtual const QProcess & underlyingProcess() const;

private:
	QScopedPointer<QProcessDBusServicePrivate> p;
};

}

#endif /* QTDBUSTEST_QPROCESSDBUSSERVICE_H_ */
