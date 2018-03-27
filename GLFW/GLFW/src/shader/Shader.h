#pragma once

#include "..\includes\Includes.h"
 
///<summary>Класс шейдер.</summary>
class Shader
{
private: 
	///<summary>Идентификатор шейдера.</summary>
    unsigned int id_;

    ///<summary>Проверка на ошибки компиляции шейдера.</summary>
    ///<param name = 'id'>Идентификатор шейдера.</param>
    ///<param name = 'type'>Тип шейдера.</param>
    void checkCompilationErrors(const unsigned int id, const std::string type) const;

public:
    ///<summary>Конструктор.</summary>
    ///<param name = 'vs_path'>Путь к вершинному шейдеру.</param>
    ///<param name = 'fs_path'>Путь к фрагментному шейдеру.</param>
    Shader(std::string vs_path, std::string fs_path);

    ///<summary>Возвращает идентификатор шейдера.</summary>
    unsigned int getID() const;

    ///<summary>Активация шейдера.</summary>
    void activate() const;

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
    ///<param name = 'projection_matrix'>Матрица проекции.</param>
    void setProjectionMatrix(const glm::mat4 projection_matrix) const;

    ///<summary>Задаёт (передаёт) матрицу вида.</summary>
    ///<param name = 'view_matrix'>Матрица вида.</param>
    void setViewMatrix(const glm::mat4 view_matrix) const;

    ///<summary>Задаёт (передаёт) матрицу модели.</summary>
    ///<param name = 'model_matrix'>Матрица модели.</param>
    void setModelMatrix(const glm::mat4 model_matrix) const;

    ///<summary>Задаёт (передаёт) матрицы проекции, вида и модели.</summary>
    ///<param name = 'projection_matrix'>Матрица проекции.</param>
    ///<param name = 'view_matrix'>Матрица вида.</param>
    ///<param name = 'model_matrix'>Матрица модели.</param>
    void setProjectionViewModelMatrices(const glm::mat4 projection_matrix, const glm::mat4 view_matrix, const glm::mat4 model_matrix) const;
};