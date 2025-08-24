#include <iostream>
#include <map>
#include "ConfigHandler.h"
#include "../StaticMacros.h"

class ConfigManager
{
        static ConfigManager *instance;
        std::map<std::string, std::string> appConfig;
        ConfigManager()
        {
                ConfigHandler configHandler;
                configHandler.createConfigFile();
                configHandler.loadConfigFile();
                std::string openInApp = configHandler.getConfigValue(CUSTOM_MEDIA_TOOLS, OPEN_IN);
                std::string openIn(OPEN_IN);
                appConfig.insert(std::pair<std::string, std::string>(openIn, openInApp));
        }
        public:
        static ConfigManager *getInstance()
        {
                std::cout << "ConfigManager_getInstance() init....\n";
                if (instance == nullptr)
                {
                        instance = new ConfigManager();
                        std::cout << "ConfigManager new instance\n";
                }
                return instance;
        }
        static void *deleteInstance()
        {
                if (instance != nullptr)
                        delete instance;
        }
        std::map<std::string, std::string> getAppConfig()
        {
                return appConfig;
        }
        void updateAppConfig(const std::pair<std::string, std::string> &configuration)
        {
                appConfig[configuration.first] = configuration.second;
                std::cout << "first: " << configuration.first << " second: " << configuration.second << "\n";
        }
        void updateAppConfig(const std::string &key, const std::string &value)
        {
                appConfig[key] = value;
        }
        void saveConfig()
        {
                ConfigHandler configHandler;
                configHandler.loadConfigFile();
                std::cout << "val before saving\n";
                for (auto it = appConfig.begin(); it != appConfig.end(); ++it)
                {
                        configHandler.updateConfigValue(CUSTOM_MEDIA_TOOLS, it->first, it->second);
                        std::cout << "fist: " << it->first << " second: " << it->second << "\n";
                }
        }
};

ConfigManager* ConfigManager::instance = nullptr;