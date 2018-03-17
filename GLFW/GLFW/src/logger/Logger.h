#pragma once

#include "..\includes\Includes.h"

class Logger
{
private:
    std::ofstream _fout;

public:
    ///<summary>Конструктор.</summary>
    ///<param name = 'to_screen'>Вывод ошибок на экран.</param>
    Logger();
    ~Logger();

    ///<summary>Логирует ошибку.</summary>
    ///<param name = 'source'>Вывод ошибок на экран.</param>
    ///<param name = 'error_type'>Тип ошибки.</param>
    ///<param name = 'message'>Сообщение.</param>
    void log(std::string source, enum QErrorType error_type, std::string message);

    ///<summary>Логирует запуск программы.</summary>
    ///<param name = 'source'>Вывод ошибок на экран.</param>
    void start(std::string source);

    ///<summary>Логирует завершение программы.</summary>
    ///<param name = 'source'>Вывод ошибок на экран.</param>
    ///<param name = 'error'>Завершение работы из-за ошибки.</param>
    void stop(std::string source, bool error = true);
};