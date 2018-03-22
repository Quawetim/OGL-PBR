#pragma once
#include "..\includes\Includes.h"

///<summary>���������� �������� �� ����� ������������.</summary>
namespace Config
{
    ///<summary>���������� �������� �� ����� ������������. 
    ///<para>����� ��������� ���������, ���� ���� ������������ �� ������.</para>
    ///</summary>
    ///<param name = 'windowInfo'>��������� ����� �� ����.</param>
    ///<param name = 'reflectionRes'>������ ����� ���������.</param>
    void readConfig(QWindowInfo &windowInfo, int &reflectionRes);
}