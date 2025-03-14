#include "preference/preference_manager.h"

PreferenceManager::PreferenceManager() {
}

PreferenceManager::~PreferenceManager() {
}

PreferenceManager* PreferenceManager::getInstance() {
    static PreferenceManager instance;
    return &instance;
}

void PreferenceManager::setPreference(const String key, const String value) {
    Serial.printf("Set preference: %s=%s\n", key.c_str(), value.c_str());
    preferences.begin("preference", false);
    preferences.putString(key.c_str(), value.c_str());
    preferences.end();
}

String PreferenceManager::getPreference(const String key) {
    preferences.begin("preference", true);
    String value =  preferences.getString(key.c_str(), "");
    preferences.end();
    return value;
}

void PreferenceManager::clearPreference(const String key) {
    preferences.begin("preference", false);
    preferences.remove(key.c_str());
    preferences.end();
}

