#include "Logger.h"
#include <windows.h>

Logger logger;

///<summary>Конструктор.</summary>
Logger::Logger()
{
    CreateDirectory(L"log", NULL);

    this->logfile.open("log\\log.txt", std::ios::app);
    
    if (!logfile)
    {
#ifdef _DEBUG
        std::cout << "Failed to initialize Logger." << std::endl;
        system("pause");
#else
        MessageBox(NULL, L"Failed to initialize Logger.", L"ERROR:Logger", MB_OK | MB_ICONERROR);
#endif
        exit(Q_BAD_EXIT);
    }
};

///<summary>Деструктор.</summary>
Logger::~Logger() 
{
    this->logfile.close();
};

///<summary>Логирует ошибку.</summary>
///<param name = 'source'>Источник.</param>
///<param name = 'error_type'>Тип ошибки.</param>
///<param name = 'message'>Сообщение.</param>
void Logger::log(std::string source, enum QErrorType error_type, std::string message)
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[25];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d.%m.%Y | %H:%M:%S", timeinfo);
    std::string date_time(buffer);

    std::stringstream ss_msg, ss_src;

    switch (error_type)
    {
        case QErrorType::info:      ss_msg << "[" << date_time << "] " << "<INFO><" << source << "> " << message << std::endl; break;
        case QErrorType::warning:   ss_msg << "[" << date_time << "] " << "<WARNING><" << source << "> " << message << std::endl; break;
        case QErrorType::error:     ss_msg << "[" << date_time << "] " << "<ERROR><" << source << "> " << message << std::endl; break;
        default:                    ss_msg << "[" << date_time << "] " << "<UNKNOWN><" << source << "> " << message << std::endl; break;
    }

    this->logfile << ss_msg.str();

#ifdef _DEBUG
    std::cout << ss_msg.str();
#else
    if (error_type == QErrorType::error)
    {
        std::wstring w_msg, w_src;

        ss_src << "ERROR:" << source;

        std::string src;
        src = ss_src.str();

        w_msg = std::wstring(message.begin(), message.end());
        w_src = std::wstring(src.begin(), src.end());

        MessageBox(NULL, w_msg.c_str(), w_src.c_str(), MB_OK | MB_ICONERROR);
    }
#endif
}

///<summary>Логирует ошибку из stringstream.</summary>
///<param name = 'ss'>Поток.</param>
void Logger::log(std::stringstream ss)
{
    this->logfile << ss.str();

#ifdef _DEBUG
    std::cout << ss.str();
#endif
}

///<summary>Логирует запуск программы.</summary>
///<param name = 'source'>Источник.</param>
void Logger::start(std::string source)
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[25];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d.%m.%Y | %H:%M:%S", timeinfo);
    std::string date_time(buffer);

    this->logfile << "[" << date_time << "] " << "<INFO><" << source << "> Program starts." << std::endl;

#ifdef _DEBUG
    std::cout << "[" << date_time << "] " << "<INFO><" << source << "> Program starts." << std::endl;
#endif   
}

///<summary>Логирует завершение программы.</summary>
///<param name = 'source'>Источник.</param>
///<param name = 'message'>Сообщение.</param>
///<param name = 'error'>Завершение работы из-за ошибки.</param>
void Logger::stop(std::string source, bool error, std::string message)
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[25];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d.%m.%Y | %H:%M:%S", timeinfo);
    std::string date_time(buffer);

    this->logfile << "[" << date_time << "] " << "<INFO><" << source << "> Program stops." << std::endl;
    this->logfile << "---------------------------------------------------" << std::endl;


    if (error)
    {
#ifdef _DEBUG
        std::cout << "[" << date_time << "] " << "<INFO><" << source << "> Program stops." << std::endl;
        system("pause");
#else
        std::stringstream ss_src;
        std::wstring w_msg, w_src;

        ss_src << "ERROR:" << source;

        std::string src;
        src = ss_src.str();

        w_msg = std::wstring(message.begin(), message.end());
        w_src = std::wstring(src.begin(), src.end());

        MessageBox(NULL, w_msg.c_str(), w_src.c_str(), MB_OK | MB_ICONERROR);
#endif 
    }
}