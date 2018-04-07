#pragma once
#include "..\includes\Includes.h"

///<summary>������. �������� ������ �� ����� � � ���� "log.txt".</summary>
class Logger
{
private:
    std::ofstream logfile_;

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

    ///<summary>�������� ������ ���������.</summary>
    ///<param name = 'source'>��������.</param>
    void start(std::string source);

    ///<summary>�������� ���������� ���������.</summary>
    ///<param name = 'source'>��������.</param>
    ///<param name = 'message'>���������.</param>
    ///<param name = 'error'>���������� ������ ��-�� ������.</param>
    void stop(std::string source, bool error = true, std::string message = "These are not the droids you are looking for.");
};

///<summary>������. �������� ������ �� ����� � � ���� "log.txt".</summary>
extern Logger logger;