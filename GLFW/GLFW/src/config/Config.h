#pragma once
#include "..\includes\Includes.h"

namespace QConfig
{
    ///<summary>���������� �������� �� ����� ������������. 
    ///<para>����� ��������� ���������, ���� ���� ������������ �� ������.</para>
    ///</summary>
    ///<param name = 'Winfo'>��������� ����� �� ����.</param>
    ///<param name = 'Reflection_res'>������ ����� ���������.</param>
    void ReadConfig(QWindowInfo &Winfo, int &Reflection_res);
}