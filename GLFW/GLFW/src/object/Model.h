#pragma once
#include "..\includes\Includes.h"
#include "Mesh.h"

class Model
{
private:
    // ��� ������
    std::string name;

    // ���������� � ������� ����� ������
    std::string dir;

    // ����, �� ������� ������� ������.
    std::vector<Mesh> meshes;

    // ��� ����������� � ������ ��������
    std::vector<QTexture> loaded_textures;    

    // ������� ������
    glm::mat4 modelMatrix;

    // ������� ������������
    glm::mat4 translationMatrix;

    // ������� ��������
    glm::mat4 rotationMatrix;

    // ������� �������
    glm::mat4 scaleMatrix;

    // ������� ������� ����
    glm::vec3 position;

    // ���� ��������
    double rotationAngle;

    // ��� ��������
    glm::vec3 rotationAxis;

    // ����������� ������� �� ������ �� ����
    glm::vec3 scale;

    ///<summary>��������� ���� ������.</summary>
    ///<param name ='node'>���� assimp.</param>
    ///<param name ='scene'>����� assimp.</param>
    void handleNode(aiNode *node, const aiScene *scene);

    ///<summary>��������� ���� ������.</summary>
    ///<param name ='mesh'>��� assimp.</param>
    ///<param name ='scene'>����� assimp.</param>
    Mesh handleMesh(aiMesh *mesh, const aiScene *scene);

    ///<summary>�������� ������� ������.</summary>
    ///<param name ='material'>�������� assimp.</param>
    ///<param name ='type'>��� �������� assimp.</param>
    ///<param name ='textureType'>��� �������� � �������.</param>
    std::vector<QTexture> loadMaterialTextures(aiMaterial *material, aiTextureType type, QTextureType textureType);

public:
    ///<summary>�����������.</summary>
    ///<param name ='path'>���� � ������.</param>
    Model(std::string path);

    ///<summary>��������� ������.</summary>
    ///<param name ='shader'>������.</param>
    void drawModel(Shader shader);

    ///<summary>������� ������ � ����������� ��� � �������� ���������.</summary>
    ///<param name = 'velocityX'>�������� �� ��� x.</param>
    ///<param name = 'velocityY'>�������� �� ��� y.</param>
    ///<param name = 'velocityZ'>�������� �� ��� z.</param>
    void moveModel(const float velocityX, const float velocityY, const float velocityZ);

    ///<summary>������� ������ � �������� ���������.</summary>
    ///<param name = 'angle'>�������� �������� � ��������.</param>
    ///<param name = 'axis'>��� ��������.</param>
    void rotateModel(const double angle, const glm::vec3 axis);

    ///<summary>�������� ������ ������ � �������� ���������.</summary>
    ///<param name = 'scaleXYZ'>�������� ��������� ������� �� ���� ����.</param>
    void scaleModel(const float scaleXYZ);

    ///<summary>�������� ������ ������ � �������� ���������.</summary>
    ///<param name = 'scaleX'>�������� ��������� ������� �� X.</param>
    ///<param name = 'scaleY'>�������� ��������� ������� �� Y.</param>
    ///<param name = 'scaleZ'>�������� ��������� ������� �� Z.</param>
    void scaleModel(const float scaleX, const float scaleY, const float scaleZ);

    ///<summary>����� ambient ���� ���� ����� ������ � RGB �������.</summary>
    ///<param name = 'red'>������� ���������� �����.</param>
    ///<param name = 'green'>������ ���������� �����.</param>
    ///<param name = 'blue'>����� ���������� �����.</param>
    void setAmbientColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>����� diffuse ���� ���� ����� ������ � RGB �������.</summary>
    ///<param name = 'red'>������� ���������� �����.</param>
    ///<param name = 'green'>������ ���������� �����.</param>
    ///<param name = 'blue'>����� ���������� �����.</param>
    void setDiffuseColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>����� specular ���� ���� ����� ������ � RGB �������.</summary>
    ///<param name = 'red'>������� ���������� �����.</param>
    ///<param name = 'green'>������ ���������� �����.</param>
    ///<param name = 'blue'>����� ���������� �����.</param>
    void setSpecularColor(const unsigned char red, const unsigned char green, const unsigned char blue);

    ///<summary>����� ���� (�������) ����� ���� ����� ������.</summary>
    ///<param name = 'value'>��������.</param>
    void setShinePower(const float value);

    ///<summary>����� ���� ������������� �������� ���� name.</summary>
    ///<param name = 'mesh_name'>��� ����.</param>
    ///<param name = 'texture_type'>��� �������� �����.</param>
    ///<param name = 'use'>������������ �������� ��� ���.</param>
    void setTextureFlag(const std::string mesh_name, const QTextureType texture_type, const bool use);

    ///<summary>����� ���� ������������� �������� ���� ����� ������.</summary>
    ///<param name = 'type'>��� ��������.</param>
    ///<param name = 'use'>������������ �������� ��� ���.</param>
    void setTextureFlag(const QTextureType type, const bool use);

    ///<summary>����� ���� ����� �������� ��������.</summary>
    ///<param name = 'texture'>��������.</param>
    void setTestTexture(QTexture texture);

    ///<summary>����� ������� ������.</summary>
    ///<param name = 'position'>�������.</param>
    void setPosition(glm::vec3 position);

    ///<summary>����� ������� ������.</summary>
    ///<param name = 'angle'>���� �������� � ��������.</param>
    ///<param name = 'axis'>��� ��������.</param>
    void setRotation(const double angle, const glm::vec3 axis);

    ///<summary>����� ������ ������ �� ���������.</summary>
    ///<param name = 'scale'>����������� �������.</param>
    void setScale(const glm::vec3 scale);

    ///<summary>���������� ��� ������.</summary>
    std::string getName() const;

    ///<summary>���������� ������� ������.</summary>
    glm::mat4 getModelMatrix();

    ///<summary>���������� ������� ������.</summary>
    glm::vec3 getPosition() const;
};