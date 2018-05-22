#pragma once
#include "..\includes\Includes.h"

///<summary>Логгер. Логирует ошибки на экран и в файл "log.txt".</summary>
class Logger
{
private:
	///<summary>Файл.</summary>
    std::ofstream logfile_;

public:
    ///<summary>Конструктор.</summary>
    Logger();

    ///<summary>Деструктор.</summary>
    ~Logger();

    ///<summary>Логирует ошибку.</summary>
    ///<param name = 'source'>Источник.</param>
    ///<param name = 'error_type'>Тип ошибки.</param>
    ///<param name = 'message'>Сообщение.</param>
    void log(std::string source, enum ErrorType error_type, std::string message);

    ///<summary>Логирует запуск программы.</summary>
    ///<param name = 'source'>Источник.</param>
    void start(std::string source);

    ///<summary>Логирует завершение программы.</summary>
    ///<param name = 'source'>Источник.</param>
    ///<param name = 'message'>Сообщение.</param>
    ///<param name = 'error'>Завершение работы из-за ошибки.</param>
    void stop(std::string source, bool error = true, std::string message = "These are not the droids you are looking for.");
};

///<summary>Логгер. Логирует ошибки на экран и в файл "log.txt".</summary>
extern Logger logger;