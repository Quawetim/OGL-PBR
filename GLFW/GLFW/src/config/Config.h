#pragma once
#include "..\includes\Includes.h"

namespace QConfig
{
    ///<summary>Считывание настроек из файла конфигурации. 
    ///<para>Задаёт дефолтные настройки, если файл конфигурации не найден.</para>
    ///</summary>
    ///<param name = 'Winfo'>Считанные даные об окне.</param>
    ///<param name = 'Reflection_res'>Размер карты отражений.</param>
    void ReadConfig(QWindowInfo &Winfo, int &Reflection_res);
}