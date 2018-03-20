#pragma once
#include "..\includes\Includes.h"

namespace QConfig
{
    ///<summary>���������� �������� �� ����� ������������. 
    ///<para>����� ��������� ���������, ���� ���� ������������ �� ������.</para>
    ///</summary>
    ///<param name = 'windowInfo'>��������� ����� �� ����.</param>
    ///<param name = 'reflectionRes'>������ ����� ���������.</param>
    void ReadConfig(QWindowInfo &windowInfo, int &reflectionRes);
}