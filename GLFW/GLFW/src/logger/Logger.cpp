#include "Logger.h"
#include <windows.h>

Logger logger(true);

///<summary>�������������.</summary>
///<param name = 'to_screen'>����� ������ �� �����.</param>
Logger::Logger(bool to_screen) : _to_screen(to_screen) 
{
    CreateDirectory(L"log", NULL);

    this->_fout.open("log\\log.txt", std::ios::app);
    
    if (!_fout)
    {
        std::cout << "Failed to initialize Logger." << std::endl;
        system("pause");
        exit(Q_BAD_EXIT);
    }
};

Logger::Logger() {};

Logger::~Logger() 
{
    this->_fout.close();
};

///<summary>�������� ������.</summary>
///<param name = 'source'>����� ������ �� �����.</param>
///<param name = 'error_type'>��� ������.</param>
///<param name = 'message'>���������.</param>
void Logger::log(std::string source, enum errorType error_type, std::string message)
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[25];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d.%m.%Y | %H:%M:%S", timeinfo);
    std::string date_time(buffer);

    if (this->_to_screen)
    {
        switch (error_type)
        {
            case errorType::info:       std::cout << "[" << date_time << "] " << "<INFO><" << source << "> " << message << std::endl; break;
            case errorType::warning:    std::cout << "[" << date_time << "] " << "<WARNING><" << source << "> " << message << std::endl; break;
            case errorType::error:      std::cout << "[" << date_time << "] " << "<ERROR><" << source << "> " << message << std::endl; break;           
            default:                    std::cout << "[" << date_time << "] " << "<UNKNOWN><" << source << "> " << message << std::endl; break;
        }
    }

    switch (error_type)
    {
        case errorType::info:       _fout << "[" << date_time << "] " << "<INFO><" << source << "> " << message << std::endl; break;
        case errorType::warning:    _fout << "[" << date_time << "] " << "<WARNING><" << source << "> " << message << std::endl; break;
        case errorType::error:      _fout << "[" << date_time << "] " << "<ERROR><" << source << "> " << message << std::endl; break;
        default:                    _fout << "[" << date_time << "] " << "<UNKNOWN><" << source << "> " << message << std::endl; break;
    }
}

///<summary>�������� ������ ���������.</summary>
///<param name = 'source'>����� ������ �� �����.</param>
void Logger::start(std::string source)
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[25];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d.%m.%Y | %H:%M:%S", timeinfo);
    std::string date_time(buffer);

    if (this->_to_screen)
    {
        std::cout << "[" << date_time << "] " << "<INFO><" << source << "> Program starts." << std::endl;
    }

    _fout << "[" << date_time << "] " << "<INFO><" << source << "> Program starts." << std::endl;
}

///<summary>�������� ���������� ���������.</summary>
///<param name = 'source'>����� ������ �� �����.</param>
///<param name = 'error'>���������� ������ ��-�� ������.</param>
void Logger::stop(std::string source, bool error)
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[25];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d.%m.%Y | %H:%M:%S", timeinfo);
    std::string date_time(buffer);

    if (this->_to_screen)
    {
        std::cout << "[" << date_time << "] " << "<INFO><" << source << "> Program stops." << std::endl;
        if (error) system("pause");
    }

    _fout << "[" << date_time << "] " << "<INFO><" << source << "> Program stops." << std::endl;
    _fout << "---------------------------------------------------" << std::endl;
}