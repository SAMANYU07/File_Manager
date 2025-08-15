#include <iostream>
#include <glib-2.0/glib.h>
#include <filesystem>
#include "../StaticMacros.h"


class ConfigHandler
{
        public:
        g_autoptr(GKeyFile) keyFile = g_key_file_new();

        static bool configFileExists(std::string configFileName)
        {
                if (std::filesystem::exists(configFileName))
                        return true;
                return false;
        }
        
        void createConfigFile()
        {
                if (configFileExists(CONFIG_FILE_NAME))
                        return;
                const gchar *val = CUSTOM_APP;
                g_autoptr(GError) error = NULL;
                g_key_file_set_string(keyFile, CUSTOM_MEDIA_TOOLS, OPEN_IN, val);
                if (!g_key_file_save_to_file(keyFile, CONFIG_FILE_NAME, &error))
                        g_warning("settings file not saved!!", error->message);
        }

        gboolean gErrorMatches(g_autoptr(GError) error, GQuark domain, gint code)
        {
                return g_error_matches(error, domain, code);
        }

        void loadConfigFile()
        {
                g_autoptr(GError) error = NULL;
                if (!g_key_file_load_from_file(keyFile, CONFIG_FILE_NAME, G_KEY_FILE_KEEP_COMMENTS, &error) && !gErrorMatches(error, G_FILE_ERROR, G_FILE_ERROR_NOENT))
                {
                        g_warning("config file not loaded!!\n", error->message);
                        return;
                }
        }


        std::string getConfigValue(std::string groupName, std::string key)
        {
                if (!configFileExists(CONFIG_FILE_NAME))
                        return "";
                const gchar *grpName = groupName.c_str();
                const gchar *Key = key.c_str();
                g_autoptr(GError) error = NULL;
                g_autofree gchar *value = g_key_file_get_string(keyFile, grpName, Key, &error);
                if (value == NULL && gErrorMatches(error, G_KEY_FILE_ERROR, G_KEY_FILE_ERROR_KEY_NOT_FOUND))
                {
                        g_warning("Error while searching for key!!\n", error->message);
                        return NULL;
                }
                else if (value == NULL)
                        return "";
                else
                {
                        std::string v(value);
                        return v;
                }
        }
      
};