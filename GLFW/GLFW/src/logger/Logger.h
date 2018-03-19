#pragma once

#include "..\includes\Includes.h"

class Logger
{
private:
    std::ofstream logfile;

public:
    ///<summary>Конструктор.</summary>
    Logger();

    ///<summary>Деструктор.</summary>
    ~Logger();

    ///<summary>Логирует ошибку.</summary>
    ///<param name = 'source'>Источник.</param>
    ///<param name = 'error_type'>Тип ошибки.</param>
    ///<param name = 'message'>Сообщение.</param>
    void log(std::string source, enum QErrorType error_type, std::string message);

    ///<summary>Логирует ошибку из stringstream.</summary>
    ///<param name = 'ss'>Поток.</param>
    void log(std::stringstream ss);

    ///<summary>Логирует запуск программы.</summary>
    ///<param name = 'source'>Источник.</param>
    void start(std::string source);

    ///<summary>Логирует завершение программы.</summary>
    ///<param name = 'source'>Источник.</param>
    ///<param name = 'message'>Сообщение.</param>
    ///<param name = 'error'>Завершение работы из-за ошибки.</param>
    void stop(std::string source, bool error = true, std::string message = "These are not the droids you are looking for.");
};

extern Logger logger;