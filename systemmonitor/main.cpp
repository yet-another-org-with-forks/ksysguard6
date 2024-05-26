/*
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include <QApplication>
#include <QDBusConnection>

#include <KAboutData>
#include <KLocalizedString>

#include "ksystemactivitydialog.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("systemmonitor");

    KAboutData about(QStringLiteral("systemmonitor"), i18n("System Activity"));
    KAboutData::setApplicationData(about);

    QDBusConnection con = QDBusConnection::sessionBus();
    if (!con.registerService(QStringLiteral("org.kde.systemmonitor"))) {
        return 0;
    }

    KSystemActivityDialog *dialog = new KSystemActivityDialog;
    dialog->show();

    return app.exec();
}