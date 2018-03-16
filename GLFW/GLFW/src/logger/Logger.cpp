#include "Logger.h"
#include <windows.h>

Logger logger;

///<summary>Инициализация.</summary>
Logger::Logger()
{
    CreateDirectory(L"log", NULL);

    this->_fout.open("log\\log.txt", std::ios::app);
    
    if (!_fout)
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

Logger::~Logger() 
{
    this->_fout.close();
};

///<summary>Логирует ошибку.</summary>
///<param name = 'source'>Вывод ошибок на экран.</param>
///<param name = 'error_type'>Тип ошибки.</param>
///<param name = 'message'>Сообщение.</param>
void Logger::log(std::string source, enum errorType error_type, std::string message)
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
        case errorType::info:       ss_msg << "[" << date_time << "] " << "<INFO><" << source << "> " << message << std::endl; break;
        case errorType::warning:    ss_msg << "[" << date_time << "] " << "<WARNING><" << source << "> " << message << std::endl; break;
        case errorType::error:      ss_msg << "[" << date_time << "] " << "<ERROR><" << source << "> " << message << std::endl; break;
        default:                    ss_msg << "[" << date_time << "] " << "<UNKNOWN><" << source << "> " << message << std::endl; break;
    }

    _fout << ss_msg.str();

#ifdef _DEBUG
    std::cout << ss_msg.str();
#else
    if (error_type == errorType::error)
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

///<summary>Логирует запуск программы.</summary>
///<param name = 'source'>Вывод ошибок на экран.</param>
void Logger::start(std::string source)
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[25];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d.%m.%Y | %H:%M:%S", timeinfo);
    std::string date_time(buffer);

    _fout << "[" << date_time << "] " << "<INFO><" << source << "> Program starts." << std::endl;

#ifdef _DEBUG
    std::cout << "[" << date_time << "] " << "<INFO><" << source << "> Program starts." << std::endl;
#endif   
}

///<summary>Логирует завершение программы.</summary>
///<param name = 'source'>Вывод ошибок на экран.</param>
///<param name = 'error'>Завершение работы из-за ошибки.</param>
void Logger::stop(std::string source, bool error)
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[25];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d.%m.%Y | %H:%M:%S", timeinfo);
    std::string date_time(buffer);

    _fout << "[" << date_time << "] " << "<INFO><" << source << "> Program stops." << std::endl;
    _fout << "---------------------------------------------------" << std::endl;

#ifdef _DEBUG
    if (error)
    {
        std::cout << "[" << date_time << "] " << "<INFO><" << source << "> Program stops." << std::endl;
        system("pause");
    }
#endif   
}