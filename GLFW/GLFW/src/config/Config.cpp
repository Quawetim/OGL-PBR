#include "Config.h"

extern Logger logger;

///<summary>Считывание настроек из файла конфигурации. 
///<para>Задаёт дефолтные настройки, если файл конфигурации не найден.</para>
///</summary>
///<param name = 'Winfo'>Считанные даные об окне.</param>
///<param name = 'Reflection_res'>Размер карты отражений.</param>
void QConfig::ReadConfig(QWindowInfo &Winfo, int &Reflection_res)
{
    std::ifstream fin;
    fin.open("config\\config.ini");

    // Настройки по-умолчанию
    if (!fin)
    {
        logger.log("ReadConfig", QErrorType::warning, "Config file not found.");
        Winfo.FullScreen = false; Winfo.Vsync = true; Winfo.ShowCursor = false;
        Winfo.Width = 800; Winfo.Height = 600;
        Winfo.HalfWidth = Winfo.Width / 2.0f; Winfo.HalfHeight = Winfo.Height / 2.0f;
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
                    if (s == "true") Winfo.FullScreen = true;
                    else Winfo.FullScreen = false;

                    continue;
                }
            }

            if (s == "Vsync")
            {
                fin >> s;
                if (s == "=")
                {
                    fin >> s;
                    if (s == "true") Winfo.Vsync = true;
                    else Winfo.Vsync = false;

                    continue;
                }
            }

            if (s == "WindowWidth")
            {
                fin >> s;
                if (s == "=")
                {
                    fin >> Winfo.Width;
                    Winfo.HalfWidth = Winfo.Width / 2.0f;

                    continue;
                }
            }

            if (s == "WindowHeight")
            {
                fin >> s;
                if (s == "=")
                {
                    fin >> Winfo.Height;
                    Winfo.HalfHeight = Winfo.Height / 2.0f;

                    continue;
                }
            }

            if (s == "ReflectionResolution")
            {
                fin >> s;
                if (s == "=")
                {
                    fin >> Reflection_res;

                    continue;
                }
            }
        }
    }

    fin.close();
}