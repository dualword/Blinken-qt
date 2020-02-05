/* Blinken-qt (2020) http://github.com/dualword/Blinken-qt License:GNU GPL*/
/***************************************************************************
 *   Copyright (C) 2005-2007 by Albert Astals Cid <aacid@kde.org>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "blinken.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QCoreApplication::setApplicationName("Blinken-qt");
	QCoreApplication::setOrganizationName("Blinken-qt");
	QCoreApplication::setOrganizationDomain("Blinken-qt");

	app.setWindowIcon(QIcon(":/sc-apps-blinken.svgz"));

 	QFont f(QStringLiteral("Steve"), 12, QFont::Normal, true);
	// Works with Steve may need some tweaking to work with other fonts
	if (!QFontInfo(f).exactMatch())
	{
		QFontDatabase::addApplicationFont(QStringLiteral(":/steve.ttf"));
	}

	new blinken();
	return app.exec();
}
