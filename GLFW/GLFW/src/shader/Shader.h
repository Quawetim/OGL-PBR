#pragma once

#include "..\includes\Includes.h"

// Класс шейдер
class Shader
{
private:
    // Идентификатор шейдера
    unsigned int ID;

    ///<summary>Проверка на ошибки компиляции шейдера.</summary>
    ///<param name = 'id'>Идентификатор шейдера.</param>
    ///<param name = 'type'>Тип шейдера.</param>
    void checkCompileErrors(unsigned int id, std::string type);

public:
    ///<summary>Конструктор.</summary>
    ///<param name = 'vs_path'>Путь к вершинному шейдеру.</param>
    ///<param name = 'fs_path'>Путь к фрагментному шейдеру.</param>
    Shader(const std::string vs_path, const std::string fs_path);

    ///<summary>Возвращает идентификатор шейдера.</summary>
    unsigned int getID();

    ///<summary>Активация шейдера.</summary>
    void activate();

    ///<summary>Задаёт (передаёт) значение переменной типа bool в шейдере по имени.</summary>
    ///<param name = 'name'>Имя переменной в шейдере.</param>
    ///<param name = 'value'>Задаваемое значение.</param>
    void setBool(const std::string &name, bool value) const;
    
    ///<summary>Задаёт (передаёт) значение переменной типа int в шейдере по имени.</summary>
    ///<param name = 'name'>Имя переменной в шейдере.</param>
    ///<param name = 'value'>Задаваемое значение.</param>
    void setInt(const std::string &name, int value) const;
    
    ///<summary>Задаёт (передаёт) значение переменной типа float в шейдере по имени.</summary>
    ///<param name = 'name'>Имя переменной в шейдере.</param>
    ///<param name = 'value'>Задаваемое значение.</param>
    void setFloat(const std::string &name, float value) const;

    ///<summary>Задаёт (передаёт) значение переменной типа mat4 в шейдере по имени.</summary>
    ///<param name = 'name'>Имя переменной в шейдере.</param>
    ///<param name = 'mat'>Задаваемое значение.</param>
    void setMat4(const std::string &name, const glm::mat4 mat) const;
};