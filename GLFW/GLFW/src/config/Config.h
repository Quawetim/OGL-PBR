#pragma once
#include "..\includes\Includes.h"

///<summary>���������� �������� �� ����� ������������.</summary>
namespace config
{
    ///<summary>���������� �������� �� ����� ������������.</summary>
    ///<param name = 'windowInfo'>��������� ����� �� ����.</param>
    ///<param name = 'reflectionsResolution'>������ ����� ���������.</param>
    void readConfig(QWindowInfo &windowInfo, int &reflectionsResolution);
}