#pragma once
#include "..\includes\Includes.h"

///<summary>Считывание настроек из файла конфигурации.</summary>
namespace config
{
    ///<summary>Считывание настроек из файла конфигурации.</summary>
    ///<param name = 'windowInfo'>Считанные даные об окне.</param>
    ///<param name = 'reflectionsResolution'>Размер карты отражений.</param>
    void readConfig(QWindowInfo &windowInfo, int &reflectionsResolution);
}