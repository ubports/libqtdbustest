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

namespace QtDBusTest {

class SuicidalProcess: public QProcess {
public:
	SuicidalProcess();

	virtual ~SuicidalProcess();

protected Q_SLOTS:
	virtual void setSuicidal();

protected:
	QProcess m_watchdog;
};

} /* namespace QtDBusTest */
#endif /* SUICIDALPROCESS_H_ */
