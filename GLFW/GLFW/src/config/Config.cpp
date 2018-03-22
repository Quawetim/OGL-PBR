#include "Config.h"

///<summary>Считывание настроек из файла конфигурации. 
///<para>Задаёт дефолтные настройки, если файл конфигурации не найден.</para>
///</summary>
///<param name = 'windowInfo'>Считанные даные об окне.</param>
///<param name = 'reflectionRes'>Размер карты отражений.</param>
void Config::readConfig(QWindowInfo &windowInfo, int &reflectionMapResolution)
{
    std::ifstream fin;
    fin.open("config\\config.ini");

    if (!fin)
    {
        logger.log("QConfig::readConfig", QErrorType::warning, "Config file not found.");
    }
    else
    {
        std::string s;

        while (!fin.eof())
        {
            fin >> s;

            if (s == "Fullscreen")
            {
                fin >> s;
                if (s == "=")
                {
                    fin >> s;
                    if (s == "true") windowInfo.setFullScreen(true);
                    else windowInfo.setFullScreen(false);

                    continue;
                }
            }

            if (s == "Vsync")
            {
                fin >> s;
                if (s == "=")
                {
                    fin >> s;
                    if (s == "true") windowInfo.setVsync(true);
                    else windowInfo.setVsync(false);

                    continue;
                }
            }

            if (s == "WindowWidth")
            {
                fin >> s;
                if (s == "=")
                {
                    int width;
                    fin >> width;
                    windowInfo.setWidth(width);

                    continue;
                }
            }

            if (s == "WindowHeight")
            {
                fin >> s;
                if (s == "=")
                {
                    int height;
                    fin >> height;
                    windowInfo.setHeight(height);

                    continue;
                }
            }

            if (s == "ReflectionResolution")
            {
                fin >> s;
                if (s == "=")
                {
                    fin >> reflectionMapResolution;

                    continue;
                }
            }
        }
    }

    fin.close();
}