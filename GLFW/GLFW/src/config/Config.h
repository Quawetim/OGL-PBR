#pragma once
#include "..\includes\Includes.h"

namespace QConfig
{
    ///<summary>Считывание настроек из файла конфигурации. 
    ///<para>Задаёт дефолтные настройки, если файл конфигурации не найден.</para>
    ///</summary>
    ///<param name = 'windowInfo'>Считанные даные об окне.</param>
    ///<param name = 'reflectionRes'>Размер карты отражений.</param>
    void ReadConfig(QWindowInfo &windowInfo, int &reflectionRes);
}