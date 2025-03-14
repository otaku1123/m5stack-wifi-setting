#pragma once

#include <Preferences.h>

class PreferenceManager
{
public:
    static PreferenceManager *getInstance();
    void setPreference(const String key, const String value);
    String getPreference(const String key);
    void clearPreference(const String key);

private:
    PreferenceManager();
    ~PreferenceManager();
    Preferences preferences;
};