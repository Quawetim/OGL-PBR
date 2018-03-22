#pragma once

#include "..\includes\Includes.h"
 
///<summary>����� ������.</summary>
class Shader
{
private: 
	///<summary>������������� �������.</summary>
    unsigned int ID;

    ///<summary>�������� �� ������ ���������� �������.</summary>
    ///<param name = 'id'>������������� �������.</param>
    ///<param name = 'type'>��� �������.</param>
    void checkCompilationErrors(const unsigned int id, const std::string type) const;

public:
    ///<summary>�����������.</summary>
    ///<param name = 'vs_path'>���� � ���������� �������.</param>
    ///<param name = 'fs_path'>���� � ������������ �������.</param>
    Shader(std::string vs_path, std::string fs_path);

    ///<summary>���������� ������������� �������.</summary>
    unsigned int getID() const;

    ///<summary>��������� �������.</summary>
    void activate() const;

    ///<summary>����� (�������) �������� ���������� ���� bool � ������� �� �����.</summary>
    ///<param name = 'name'>��� ���������� � �������.</param>
    ///<param name = 'value'>���������� ��������.</param>
    void setBool(const std::string &name, const bool value) const;
    
    ///<summary>����� (�������) �������� ���������� ���� int � ������� �� �����.</summary>
    ///<param name = 'name'>��� ���������� � �������.</param>
    ///<param name = 'value'>���������� ��������.</param>
    void setInt(const std::string &name, const int value) const;
    
    ///<summary>����� (�������) �������� ���������� ���� float � ������� �� �����.</summary>
    ///<param name = 'name'>��� ���������� � �������.</param>
    ///<param name = 'value'>���������� ��������.</param>
    void setFloat(const std::string &name, const float value) const;

    ///<summary>����� (�������) �������� ���������� ���� vec3 � ������� �� �����.</summary>
    ///<param name = 'name'>��� ���������� � �������.</param>
    ///<param name = 'value'>���������� ��������.</param>
    void setVec3(const std::string &name, const glm::vec3 &value) const;

    ///<summary>����� (�������) �������� ���������� ���� mat4 � ������� �� �����.</summary>
    ///<param name = 'name'>��� ���������� � �������.</param>
    ///<param name = 'mat'>���������� ��������.</param>
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

    ///<summary>����� (�������) ������� ��������.</summary>
    ///<param name = 'projectionMatrix'>������� ��������.</param>
    void setProjectionMatrix(const glm::mat4 projectionMatrix) const;

    ///<summary>����� (�������) ������� ����.</summary>
    ///<param name = 'viewMatrix'>������� ����.</param>
    void setViewMatrix(const glm::mat4 viewMatrix) const;

    ///<summary>����� (�������) ������� ������.</summary>
    ///<param name = 'modelMatrix'>������� ������.</param>
    void setModelMatrix(const glm::mat4 modelMatrix) const;

    ///<summary>����� (�������) ������� ��������, ���� � ������.</summary>
    ///<param name = 'projectionMatrix'>������� ��������.</param>
    ///<param name = 'viewMatrix'>������� ����.</param>
    ///<param name = 'modelMatrix'>������� ������.</param>
    void setProjectionViewModelMatrices(const glm::mat4 projectionMatrix, const glm::mat4 viewMatrix, const glm::mat4 modelMatrix) const;
};