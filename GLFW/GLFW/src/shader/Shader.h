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
    void checkCompilationErrors(unsigned int id, std::string type);

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
    void setBool(const std::string &name, const bool value) const;
    
    ///<summary>Задаёт (передаёт) значение переменной типа int в шейдере по имени.</summary>
    ///<param name = 'name'>Имя переменной в шейдере.</param>
    ///<param name = 'value'>Задаваемое значение.</param>
    void setInt(const std::string &name, const int value) const;
    
    ///<summary>Задаёт (передаёт) значение переменной типа float в шейдере по имени.</summary>
    ///<param name = 'name'>Имя переменной в шейдере.</param>
    ///<param name = 'value'>Задаваемое значение.</param>
    void setFloat(const std::string &name, const float value) const;

    ///<summary>Задаёт (передаёт) значение переменной типа vec3 в шейдере по имени.</summary>
    ///<param name = 'name'>Имя переменной в шейдере.</param>
    ///<param name = 'value'>Задаваемое значение.</param>
    void setVec3(const std::string &name, const glm::vec3 &value) const;

    ///<summary>Задаёт (передаёт) значение переменной типа mat4 в шейдере по имени.</summary>
    ///<param name = 'name'>Имя переменной в шейдере.</param>
    ///<param name = 'mat'>Задаваемое значение.</param>
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

    ///<summary>Задаёт (передаёт) матрицу проекции.</summary>
    ///<param name = 'projectionMatrix'>Матрица проекции.</param>
    void setProjectionMatrix(const glm::mat4 projectionMatrix) const;

    ///<summary>Задаёт (передаёт) матрицу вида.</summary>
    ///<param name = 'viewMatrix'>Матрица вида.</param>
    void setViewMatrix(const glm::mat4 viewMatrix) const;

    ///<summary>Задаёт (передаёт) матрицу модели.</summary>
    ///<param name = 'modelMatrix'>Матрица модели.</param>
    void setModelMatrix(const glm::mat4 modelMatrix) const;

    ///<summary>Задаёт (передаёт) матрицы проекции, вида и модели.</summary>
    ///<param name = 'projectionMatrix'>Матрица проекции.</param>
    ///<param name = 'viewMatrix'>Матрица вида.</param>
    ///<param name = 'modelMatrix'>Матрица модели.</param>
    void setProjectionViewModelMatrices(const glm::mat4 projectionMatrix, const glm::mat4 viewMatrix, const glm::mat4 modelMatrix) const;
};