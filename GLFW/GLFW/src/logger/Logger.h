#pragma once

#include "..\includes\Includes.h"

class Logger
{
private:
    std::ofstream _fout;

public:
    ///<summary>�����������.</summary>
    ///<param name = 'to_screen'>����� ������ �� �����.</param>
    Logger();
    ~Logger();

    ///<summary>�������� ������.</summary>
    ///<param name = 'source'>����� ������ �� �����.</param>
    ///<param name = 'error_type'>��� ������.</param>
    ///<param name = 'message'>���������.</param>
    void log(std::string source, enum QErrorType error_type, std::string message);

    ///<summary>�������� ������ ���������.</summary>
    ///<param name = 'source'>����� ������ �� �����.</param>
    void start(std::string source);

    ///<summary>�������� ���������� ���������.</summary>
    ///<param name = 'source'>����� ������ �� �����.</param>
    ///<param name = 'error'>���������� ������ ��-�� ������.</param>
    void stop(std::string source, bool error = true);
};