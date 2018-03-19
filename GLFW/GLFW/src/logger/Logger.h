#pragma once

#include "..\includes\Includes.h"

class Logger
{
private:
    std::ofstream logfile;

public:
    ///<summary>�����������.</summary>
    Logger();

    ///<summary>����������.</summary>
    ~Logger();

    ///<summary>�������� ������.</summary>
    ///<param name = 'source'>��������.</param>
    ///<param name = 'error_type'>��� ������.</param>
    ///<param name = 'message'>���������.</param>
    void log(std::string source, enum QErrorType error_type, std::string message);

    ///<summary>�������� ������ �� stringstream.</summary>
    ///<param name = 'ss'>�����.</param>
    void log(std::stringstream ss);

    ///<summary>�������� ������ ���������.</summary>
    ///<param name = 'source'>��������.</param>
    void start(std::string source);

    ///<summary>�������� ���������� ���������.</summary>
    ///<param name = 'source'>��������.</param>
    ///<param name = 'message'>���������.</param>
    ///<param name = 'error'>���������� ������ ��-�� ������.</param>
    void stop(std::string source, bool error = true, std::string message = "These are not the droids you are looking for.");
};

extern Logger logger;