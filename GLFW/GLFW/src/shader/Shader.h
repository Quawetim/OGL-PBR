#pragma once

#include "..\includes\Includes.h"

// ����� ������
class Shader
{
private:
    // ������������� �������
    unsigned int ID;

    ///<summary>�������� �� ������ ���������� �������.</summary>
    ///<param name = 'id'>������������� �������.</param>
    ///<param name = 'type'>��� �������.</param>
    void checkCompileErrors(unsigned int id, std::string type);

public:
    ///<summary>�����������.</summary>
    ///<param name = 'vs_path'>���� � ���������� �������.</param>
    ///<param name = 'fs_path'>���� � ������������ �������.</param>
    Shader(const std::string vs_path, const std::string fs_path);

    ///<summary>���������� ������������� �������.</summary>
    unsigned int getID();

    ///<summary>��������� �������.</summary>
    void activate();

    ///<summary>����� (�������) �������� ���������� ���� bool � ������� �� �����.</summary>
    ///<param name = 'name'>��� ���������� � �������.</param>
    ///<param name = 'value'>���������� ��������.</param>
    void setBool(const std::string &name, bool value) const;
    
    ///<summary>����� (�������) �������� ���������� ���� int � ������� �� �����.</summary>
    ///<param name = 'name'>��� ���������� � �������.</param>
    ///<param name = 'value'>���������� ��������.</param>
    void setInt(const std::string &name, int value) const;
    
    ///<summary>����� (�������) �������� ���������� ���� float � ������� �� �����.</summary>
    ///<param name = 'name'>��� ���������� � �������.</param>
    ///<param name = 'value'>���������� ��������.</param>
    void setFloat(const std::string &name, float value) const;

    ///<summary>����� (�������) �������� ���������� ���� mat4 � ������� �� �����.</summary>
    ///<param name = 'name'>��� ���������� � �������.</param>
    ///<param name = 'mat'>���������� ��������.</param>
    void setMat4(const std::string &name, const glm::mat4 mat) const;
};