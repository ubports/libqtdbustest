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

#include <libqtdbustest/DBusService.h>

namespace QtDBusTest {

class DBusServicePrivate {

public:
	DBusServicePrivate(const QString &name, QDBusConnection::BusType busType) :
			m_name(name), m_busType(busType) {
	}

	QString m_name;

	QDBusConnection::BusType m_busType;
};

DBusService::DBusService(const QString &name, QDBusConnection::BusType busType) :
		d(new DBusServicePrivate(name, busType)) {
}

DBusService::~DBusService() {
}

const QString & DBusService::name() const {
	return d->m_name;
}

QDBusConnection::BusType DBusService::busType() const {
	return d->m_busType;
}

}
