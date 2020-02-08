/* Blinken-qt (2020) http://github.com/dualword/Blinken-qt License:GNU GPL*/

#ifndef SRC_SETTINGS_H_
#define SRC_SETTINGS_H_

#include <QSettings>

class blinkenSettings {

public:

	blinkenSettings() = delete;

	static bool playSounds() {
		QSettings s;
		return s.value("playSounds", "0").toBool();
	}

	static bool customFont() {
		QSettings s;
		return s.value("customFont", "0").toBool();
	}

	static void setCustomFont(const bool& b) {
		QSettings s;
		s.setValue("customFont", b);
	}

	static void setPlaySounds(const bool& b) {
		QSettings s;
		s.setValue("playSounds", b);
	}

};



#endif /* SRC_SETTINGS_H_ */
