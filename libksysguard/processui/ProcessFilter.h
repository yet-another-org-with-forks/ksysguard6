/*
    KSysGuard, the KDE System Guard

	Copyright (c) 1999, 2000 Chris Schlaeger <cs@kde.org>
	Copyright (c) 2006 John Tapsell <john.tapsell@kdemail.net>

    This program is free software; you can redistribute it and/or
    modify it under the terms of version 2 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

*/

#ifndef PROCESSFILTER_H_
#define PROCESSFILTER_H_

#include <QSortFilterProxyModel>
#include <QObject>
#define PROCESS_FILTER_ALL_SIMPLE 0
#define PROCESS_FILTER_ALL_TREE 1
#define PROCESS_FILTER_SYSTEM 2
#define PROCESS_FILTER_USER 3
#define PROCESS_FILTER_OWN 4

class QModelIndex;

class ProcessFilter : public QSortFilterProxyModel
{
	Q_OBJECT
  public:
	ProcessFilter(QObject *parent=0) : QSortFilterProxyModel(parent) {mFilter = 0;}
	virtual ~ProcessFilter() {}
	int mFilter;
	bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

  public slots:
	void setFilter(int index);
        /** This slot calls filterChanged().
	 *  This is because for example we sort the username also by the cpu used.  Tell the model that the cpu changed
	 *  woudln't normally trigger a resort if we are sorting by the user, so we basically call resort every 3 seconds.
	 *  While this sounds kinda hackish, it's actually the preferable action anyway.  We don't want the processes to jump about
	 *  constantly. */
  	void resort();
	
  protected:
	virtual bool filterAcceptsRow( int source_row, const QModelIndex & source_parent ) const;
};

#endif
