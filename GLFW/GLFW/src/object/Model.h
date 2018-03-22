#pragma once
#include "..\includes\Includes.h"
#include "Mesh.h"

///<summary>������� 3D-������, ��������� �� ���������� �������. ��������� �����.</summary>
class Model
{
	friend class Object;
private: 
	///<summary>��� ������.</summary>
    std::string modelName;
 
	///<summary>���������� � ������� ����� ������.</summary>
    std::string dir;

	///<summary>����, �� ������� ������� ������.</summary>
    std::vector<Mesh> meshes;
 
	///<summary>��� ����������� � ������ ��������.</summary>
    std::vector<QTexture> loaded_textures;    
 
	///<summary>������� ������������.</summary>
    glm::mat4 translationMatrix;
 
	///<summary>������� ��������.</summary>
    glm::mat4 rotationMatrix;

	///<summary>������� �������.</summary>
    glm::mat4 scaleMatrix;
 
	///<summary>������� ������.</summary>
    glm::vec3 position;
 
	///<summary>���� ��������.</summary>
    double rotationAngle;
 
	///<summary>��� ��������.</summary>
    glm::vec3 rotationAxis;
 
	///<summary>����������� ������� �� ������ �� ����.</summary>
    glm::vec3 scale;

    ///<summary>��������� ���� ������.</summary>
    ///<param name ='node'>���� assimp.</param>
    ///<param name ='scene'>����� assimp.</param>
    void handleNode(const aiNode *node, const aiScene *scene);

    ///<summary>��������� ���� ������.</summary>
    ///<param name ='mesh'>��� assimp.</param>
    ///<param name ='scene'>����� assimp.</param>
    Mesh handleMesh(const aiMesh *mesh, const aiScene *scene);

    ///<summary>�������� ������� ������.</summary>
    ///<param name ='material'>�������� assimp.</param>
    ///<param name ='type'>��� �������� assimp.</param>
    ///<param name ='textureType'>��� �������� � �������.</param>
    std::vector<QTexture> loadMaterialTextures(const aiMaterial *material, const aiTextureType type, const QTextureType textureType);	

	///<summary>��������� ������.</summary>
	///<param name ='shader'>������.</param>
	void drawModel(const Shader shader);

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

public:
    ///<summary>�����������.</summary>
    ///<param name ='path'>���� � ������.</param>
    Model(std::string path);  

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
    void setTestTexture(const QTexture texture); 

	///<summary>����� ������� ������.</summary>
	///<param name = 'position'>�������.</param>
	void setPosition(const glm::vec3 position);

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
    glm::mat4 getModelMatrix() const;

    ///<summary>���������� ������� ������.</summary>
    glm::vec3 getPosition() const;
};