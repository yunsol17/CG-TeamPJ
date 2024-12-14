#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <set>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include "���.h"

struct AABB {
    glm::vec3 min; // �浹�ڽ��� �ּ� ��ǥ (x, y, z)
    glm::vec3 max; // �浹�ڽ��� �ִ� ��ǥ (x, y, z)

    // AABB ���� �޼���
    void update(const glm::vec3& position, const glm::vec3& offsetMin, const glm::vec3& offsetMax) {
        min = position + offsetMin;
        max = position + offsetMax;
    }
};

// ��
GLuint vaoBottom, vaoArrowAndPillar, vaoEndPoint, vaoPoint;
GLuint vboBottom[2], vboArrowAndPillar[2], vboEndPoint[2], vboPoint[2];
Model modelBottom, modelArrowAndPillar, modelEndPoint, modelPoint;

// ĳ����1
GLuint vaoCharacter1Body, vaoCharacter1BackPattern, vaoCharacter1Blusher, vaoCharacter1Eye, vaoCharacter1Face, vaoCharacter1LeftArm, vaoCharacter1RightArm, vaoCharacter1LeftLeg, vaoCharacter1RightLeg;
GLuint vboCharacter1Body[2], vboCharacter1BackPattern[2], vboCharacter1Blusher[2], vboCharacter1Eye[2], vboCharacter1Face[2], vboCharacter1LeftArm[2], vboCharacter1RightArm[2], vboCharacter1LeftLeg[2],
vboCharacter1RightLeg[2], vboCharacter2[2];
Model modelCharacter1Body, modelCharacter1BackPattern, modelCharacter1Blusher, modelCharacter1Eye, modelCharacter1Face, modelCharacter1LeftArm, modelCharacter1RightArm, modelCharacter1LeftLeg, modelCharacter1RightLeg;

// ĳ����2
GLuint vaoCharacter2Acc, vaoCharacter2Body, vaoCharacter2Clothes, vaoCharacter2Hair, vaoCharacter2LeftLeg, vaoCharacter2RightLeg, vaoCharacter2LeftArm, vaoCharacter2RightArm, vaoCharacter2Eye, vaoCharacter2Face;
GLuint vboCharacter2Acc[2], vboCharacter2Body[2], vboCharacter2Clothes[2], vboCharacter2Hair[2], vboCharacter2LeftLeg[2], vboCharacter2RightLeg[2], vboCharacter2LeftArm[2], vboCharacter2RightArm[2], vboCharacter2Eye[2], vboCharacter2Face[2];
Model modelCharacter2Acc, modelCharacter2Body, modelCharacter2Hair, modelCharacter2Clothes, modelCharacter2LeftLeg, modelCharacter2RightLeg, modelCharacter2LeftArm, modelCharacter2RightArm, modelCharacter2Eye, modelCharacter2Face;

//��ֹ�
GLuint vaoBong1, vaoBong2, vaoHorizontalFanPink, vaoHorizontalFanPurple, vaoDoorOut, vaoLeftdoor, vaoRightdoor, vaoJumpBarCenter, vaoJumpBarbargroup1, vaoJumpBarbargroup2;
GLuint vboBong1[2], vboBong2[2], vboHorizontalFanPink[2], vboHorizontalFanPurple[2], vboDoorOut[2], vboLeftdoor[2], vboRightdoor[2],vboJumpBarCenter[2], vboJumpBarbargroup1[2], vboJumpBarbargroup2[2];
Model modelBong1, modelBong2, modelHorizontalFanPink, modelHorizontalFanPurple, modelDoorOut, modelLeftdoor, modelRightdoor, modelJumpBarCenter, modelJumpBarbargroup1, modelJumpBarbargroup2;

//checkbox
GLuint vaoCheckBoxMap1, vboCheckBoxMap1[2], vaoCheckBoxMap2, vboCheckBoxMap2[2], vaoCheckBoxMap3, vboCheckBoxMap3[2], vaoCheckBoxMap4, vboCheckBoxMap4[2], vaoCheckBoxMap5, vboCheckBoxMap5[2];
GLuint vaoCharacter1CheckBox, vboCharacter1CheckBox[2], vaoCharacter2CheckBox, vboCharacter2CheckBox[2];

GLuint shaderProgramID;
GLuint vertexShader;
GLuint fragmentShader;

GLfloat moveSpeed = 0.5f;
GLfloat character1RotationAngle = 0.0f;
GLfloat character2RotationAngle = 0.0f;
GLfloat character1ArmLegSwingAngle = 0.0f;
GLfloat character2ArmLegSwingAngle = 0.0f;
GLfloat maxSwingAngle = 30.0f;
GLfloat character1JumpSpeed = 0.3f;
GLfloat character2JumpSpeed = 0.3f;
GLfloat gravity = 0.01f;
GLfloat realGravity = 0.1f;
GLfloat BongMove = 0.1f; // �̵� �ӵ�
GLfloat MaxBongMove = 1.6f; // �ִ� �̵� �Ÿ�
GLfloat obstacleRotation = 0.0f;
GLfloat DoorMove = 0.05f;
GLfloat MaxDoorMove = 1.7f;

glm::mat4 character1ModelMatrix = glm::mat4(1.0f);
glm::vec3 character1Direction = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 character1Position = glm::vec3(-5.0f, 0.0f, -5.0f);
glm::vec3 character1InitialPosition = glm::vec3(0.0f, 0.0f, 0.0f);

glm::mat4 character2ModelMatrix = glm::mat4(1.0f);
glm::vec3 character2Direction = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 character2Position = glm::vec3(5.0f, 0.0f, -5.0f);
glm::vec3 character2InitialPosition = glm::vec3(0.0f, 0.0f, 0.0f);

glm::mat4 bong1ModelMatrix = glm::mat4(1.0f);
glm::mat4 bong2ModelMatrix = glm::mat4(1.0f);
glm::vec3 BongGroup1Position = glm::vec3(0.0f, 0.0f, 0.0f); // �ʱ� ��ġ
glm::vec3 BongGroup1Direction = glm::vec3(1.0f, 0.0f, 0.0f); // �ʱ� �̵� ���� (������)
glm::vec3 BongGroup2Position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 BongGroup2Direction = glm::vec3(-1.0f, 0.0f, 0.0f);

glm::mat4 LeftdoorModelMatrix = glm::mat4(1.0f);
glm::mat4 RightdoorModelMatrix = glm::mat4(1.0f);
glm::vec3 LeftdoorGroupPosition = glm::vec3(0.0f, 0.0f, 0.0f); // �ʱ� ��ġ
glm::vec3 LeftdoorGroupDirection = glm::vec3(-1.0f, 0.0f, 0.0f); // �ʱ� �̵� ���� (����)
glm::vec3 RightdoorGroupPosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 RightdoorGroupDirection = glm::vec3(1.0f, 0.0f, 0.0f);

bool isCharacter1Swing = false;
bool isCharacter2Swing = false;
bool isCharacter1Jumping = false;
bool isCharacter2Jumping = false;
bool isCharacter1OnMap = false;
bool isCharacter2OnMap = false;
bool moveKeyStates[256] = { false }; // �̵� Ű ����
bool arrowKeyStates[256] = { false };
bool commandKeyStates[256] = { false }; // ��� Ű ����
bool isObstacleRotate = true;

int character1SwingDirection = 1;
int character2SwingDirection = 1;

bool checkCollision(const AABB& box1, const AABB& box2) {
    return (box1.max.x > box2.min.x && box1.min.x < box2.max.x &&
        box1.max.y > box2.min.y && box1.min.y < box2.max.y &&
        box1.max.z > box2.min.z && box1.min.z < box2.max.z);
}

char* filetobuf(const char* file) {
    FILE* fptr;
    long length;
    char* buf;

    fptr = fopen(file, "rb");
    if (!fptr)
        return NULL;
    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr);
    buf = (char*)malloc(length + 1);
    fseek(fptr, 0, SEEK_SET);
    fread(buf, length, 1, fptr);
    fclose(fptr);
    buf[length] = 0;
    return buf;
}

void make_vertexShaders();
void make_fragmentShaders();
void InitBuffer();

// ��
void InitBottom();
void InitArrowAndPillar();
void InitEndPoint();
void InitPoint();

// ĳ����1
void InitCharacter1Body();
void InitCharacter1BackPattern();
void InitCharacter1Blusher();
void InitCharacter1Eye();
void InitCharacter1Face();
void InitCharacter1LeftArm();
void InitCharacter1RightArm();
void InitCharacter1LeftLeg();
void InitCharacter1RightLeg();

//üũ�ڽ� �߰�
void InitCharacter1CheckBox();
void InitCharacter2CheckBox();

// ĳ����2
void InitCharacter2Acc();
void InitCharacter2Body();
void InitCharacter2Clothes();
void InitCharacter2Hair();
void InitCharacter2LeftLeg();
void InitCharacter2LeftArm();
void InitCharacter2RightLeg();
void InitCharacter2RightArm();
void InitCharacter2Eye();
void InitCharacter2Face();

// ��
void InitBong1();
void InitBong2();
// ������
void InitHorizontalFanPink();
void InitHorizontalFanPurple();
// ������
void InitJumpbarCenter();
void InitJumpbarbargroup1();
void InitJumpbarbargroup2();


GLuint make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid KeyboardUp(unsigned char key, int x, int y);
void SpecialKey(int key, int x, int y);
void SpecialKeyUp(int key, int x, int y);
GLvoid Timer(int value);

int window_Width = 800;
int window_Height = 600;

void InitBuffer(GLuint& vao, GLuint* vbo, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(2, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void InitPart(const std::string& filePath, Model& model, GLuint& vao, GLuint* vbo, const glm::vec3& color) {
    read_obj_file(filePath, model);

    std::vector<Vertex> expandedVertices;
    std::vector<unsigned int> indices;

    for (size_t i = 0; i < model.faces.size(); ++i) {
        Vertex v1 = { model.vertices[model.faces[i].v1].x,
                      model.vertices[model.faces[i].v1].y,
                      model.vertices[model.faces[i].v1].z,
                      color };
        Vertex v2 = { model.vertices[model.faces[i].v2].x,
                      model.vertices[model.faces[i].v2].y,
                      model.vertices[model.faces[i].v2].z,
                      color };
        Vertex v3 = { model.vertices[model.faces[i].v3].x,
                      model.vertices[model.faces[i].v3].y,
                      model.vertices[model.faces[i].v3].z,
                      color };

        expandedVertices.push_back(v1);
        expandedVertices.push_back(v2);
        expandedVertices.push_back(v3);

        indices.push_back(expandedVertices.size() - 3);
        indices.push_back(expandedVertices.size() - 2);
        indices.push_back(expandedVertices.size() - 1);
    }

    InitBuffer(vao, vbo, expandedVertices, indices);
}

// ��
void InitBottom() {
    InitPart("Map/bottom.obj", modelBottom, vaoBottom, vboBottom, glm::vec3(0.0f, 0.0f, 1.0f));
}
void InitArrowAndPillar() {
    InitPart("Map/arrowAndPillar.obj", modelArrowAndPillar, vaoArrowAndPillar, vboArrowAndPillar, glm::vec3(0.0f, 1.0f, 0.0f));
}
void InitEndPoint() {
    InitPart("Map/endPoint.obj", modelEndPoint, vaoEndPoint, vboEndPoint, glm::vec3(1.0f, 0.0f, 1.0f));
}
void InitPoint() {
    InitPart("Map/point.obj", modelPoint, vaoPoint, vboPoint, glm::vec3(1.0f, 0.0f, 0.0f));
}

// ��
AABB map1 = {
    glm::vec3(-22.5f, 0.0f, -80.0f), // min
    glm::vec3(22.5f,  0.0f,  3.0f)   // max
};
AABB map2 = {
    glm::vec3(-18.0f, -2.3f, -121.0f), // min
    glm::vec3(18.0f, -0.2f, -79.0f)    // max
};
AABB map3 = {
    glm::vec3(-14.0f, -2.0f, -146.0f), // min
    glm::vec3(14.0f, -0.3f, -120.0f)   // max
};
AABB map4 = {
    glm::vec3(-11.6f, -2.0f, -165.0f), // min
    glm::vec3(11.6f, -0.4f, -143.0f)   // max
};
AABB map5 = {
    glm::vec3(-10.6f, -28.5f, -245.0f), // min
    glm::vec3(10.6f, -26.5f, -165.0f)   // max
};

// ��
void InitBong1() {
    InitPart("bong/bonggroup1.obj", modelBong1, vaoBong1, vboBong1, glm::vec3(1.0f, 0.078f, 0.576f));
}
void InitBong2() {
    InitPart("bong/bonggroup2.obj", modelBong2, vaoBong2, vboBong2, glm::vec3(1.0f, 0.078f, 0.576f));
}
// ������
void InitHorizontalFanPink() {
    InitPart("horizontalFan/pink.obj", modelHorizontalFanPink, vaoHorizontalFanPink, vboHorizontalFanPink, glm::vec3(1.0f, 0.7f, 0.75f));
}
void InitHorizontalFanPurple() {
    InitPart("horizontalFan/purple.obj", modelHorizontalFanPurple, vaoHorizontalFanPurple, vboHorizontalFanPurple, glm::vec3(0.5f, 0.0f, 0.5f));
}
// ��������
void InitDoorOut() {
    InitPart("frogDoor/outsidegroup.obj", modelDoorOut, vaoDoorOut, vboDoorOut, glm::vec3(0.576f, 0.078f, 1.0f));
}
void InitDoorLeft() {
    InitPart("frogDoor/leftdoorgroup.obj", modelLeftdoor, vaoLeftdoor, vboLeftdoor, glm::vec3(1.0f, 0.078f, 0.576f));
}
void InitDoorRight() {
    InitPart("frogDoor/rightdoorgroup.obj", modelRightdoor, vaoRightdoor, vboRightdoor, glm::vec3(1.0f, 0.078f, 0.576f));
}
// ������
void InitJumpbarCenter() {
    InitPart("jumpBong/centergroup.obj", modelJumpBarCenter, vaoJumpBarCenter, vboJumpBarCenter, glm::vec3(0.576f, 0.078f, 1.0f));
}
void InitJumpbarbargroup1() {
    InitPart("jumpBong/bargroup1.obj", modelJumpBarbargroup1, vaoJumpBarbargroup1, vboJumpBarbargroup1, glm::vec3(0.576f, 0.078f, 1.0f));
}
void InitJumpbarbargroup2() {
    InitPart("jumpBong/bargroup2.obj", modelJumpBarbargroup2, vaoJumpBarbargroup2, vboJumpBarbargroup2, glm::vec3(0.576f, 0.078f, 1.0f));
}

// ��
AABB bong1 = {
    glm::vec3(-15.74f + BongGroup1Position.x, 0.0f, -33.25f), // min
    glm::vec3(-13.74f + BongGroup1Position.x,  3.6f,  -31.25f)  // max
};
AABB bong2 = {
    glm::vec3(-9.47f + BongGroup2Position.x, 0.0f, -33.25f), // min
    glm::vec3(-7.47f + BongGroup2Position.x,  3.6f,  -31.25f)  // max
};
AABB bong3 = {
    glm::vec3(-3.169f + BongGroup1Position.x, 0.0f, -33.25f), // min
    glm::vec3(-1.169f + BongGroup1Position.x,  3.6f,  -31.25f)  // max
};
AABB bong4 = {
    glm::vec3(3.045f + BongGroup2Position.x, 0.0f, -33.25f), // min
    glm::vec3(5.045f + BongGroup2Position.x,  3.6f,  -31.25f)  // max
};
AABB bong5 = {
    glm::vec3(9.27f + BongGroup1Position.x, 0.0f, -33.25f), // min
    glm::vec3(11.27f + BongGroup1Position.x,  3.6f,  -31.25f)  // max
};
AABB bong6 = {
    glm::vec3(14.945f + BongGroup2Position.x, 0.0f, -33.25f), // min
    glm::vec3(16.945f + BongGroup2Position.x,  3.6f,  -31.25f)  // max
};

// ��
AABB leftdoor1 = {
    glm::vec3(-8.475f, -0.76f, -159.129f), // min
    glm::vec3(-6.4f,  2.4f,  -158.53f)   // max
};
AABB leftdoor2 = {
    glm::vec3(-2.168f, -0.76f, -159.129f), // min
    glm::vec3(-0.09f,  2.4f,  -158.53f)   // max
};
AABB leftdoor3 = {
    glm::vec3(4.227f, -0.76f, -159.129f), // min
    glm::vec3(6.297f,  2.4f,  -158.53f)   // max
};
AABB rightdoor1 = {
    glm::vec3(6.408f, -0.76f, -159.129f), // min
    glm::vec3(4.38f,  2.4f,  -158.53f)   // max
};
AABB rightdoor2 = {
    glm::vec3(-0.1f, -0.76f, -159.129f), // min
    glm::vec3(1.926f,  2.4f,  -158.53f)   // max
};
AABB rightdoor3 = {
    glm::vec3(6.294f, -0.76f, -159.129f), // min
    glm::vec3(8.322f,  2.4f,  -158.53f)   // max
};

// ĳ����1
void InitCharacter1Body() {
    InitPart("Character1/body.obj", modelCharacter1Body, vaoCharacter1Body, vboCharacter1Body, glm::vec3(1.0f, 1.0f, 0.0f));
}
void InitCharacter1BackPattern() {
    InitPart("Character1/backPattern.obj", modelCharacter1BackPattern, vaoCharacter1BackPattern, vboCharacter1BackPattern, glm::vec3(0.0f, 0.0f, 0.0f));
}
void InitCharacter1Blusher() {
    InitPart("Character1/blusher.obj", modelCharacter1Blusher, vaoCharacter1Blusher, vboCharacter1Blusher, glm::vec3(1.0f, 0.0f, 0.0f));
}
void InitCharacter1Eye() {
    InitPart("Character1/eye.obj", modelCharacter1Eye, vaoCharacter1Eye, vboCharacter1Eye, glm::vec3(0.0f, 0.0f, 0.0f));
}
void InitCharacter1Face() {
    InitPart("Character1/face.obj", modelCharacter1Face, vaoCharacter1Face, vboCharacter1Face, glm::vec3(1.0f, 1.0f, 1.0f));
}
void InitCharacter1LeftArm() {
    InitPart("Character1/leftArm.obj", modelCharacter1LeftArm, vaoCharacter1LeftArm, vboCharacter1LeftArm, glm::vec3(1.0f, 1.0f, 0.0f));
}
void InitCharacter1RightArm() {
    InitPart("Character1/rightArm.obj", modelCharacter1RightArm, vaoCharacter1RightArm, vboCharacter1RightArm, glm::vec3(1.0f, 1.0f, 0.0f));
}
void InitCharacter1LeftLeg() {
    InitPart("Character1/leftLeg.obj", modelCharacter1LeftLeg, vaoCharacter1LeftLeg, vboCharacter1LeftLeg, glm::vec3(1.0f, 1.0f, 0.0f));
}
void InitCharacter1RightLeg() {
    InitPart("Character1/rightLeg.obj", modelCharacter1RightLeg, vaoCharacter1RightLeg, vboCharacter1RightLeg, glm::vec3(1.0f, 1.0f, 0.0f));
}

//üũ�ڽ�
std::vector<float> Character1CheckBox = {
    // Bottom
 -0.47f, 0.f, 0.42f,
 -0.47f, 0.f, -0.48f,
 0.47f, 0.f, -0.48f,
 0.47f, 0.f, 0.42f,

 // Top
 -0.47f, 1.84f, 0.42f,
 -0.47f, 1.84f, -0.48f,
 0.47f, 1.84f, -0.48f,
 0.47f, 1.84f, 0.42f,

 // Front
 -0.47f, 0.f, -0.48f,
 -0.47f, 1.84f, -0.48f,
 0.47f, 1.84f, -0.48f,
 0.47f, 0.f, -0.48f,

 // Back face
 -0.47f, 0.f, 0.42f,
 -0.47f, 1.84f, 0.42f,
 0.47f, 1.84f, 0.42f,
 0.47f, 0.f, 0.42f,

 // Left face
 -0.47f, 0.f, 0.42f,
 -0.47f, 0.f, -0.48f,
 -0.47f, 1.84f, -0.48f,
 -0.47f, 1.84f, 0.42f,

 // Right face
 0.47f, 0.f, 0.42f,
 0.47f, 0.f, -0.48f,
 0.47f, 1.84f, -0.48f,
 0.47f, 1.84f, 0.42f,

};
void InitCharacter1CheckBox() {
    glGenVertexArrays(1, &vaoCharacter1CheckBox);
    glBindVertexArray(vaoCharacter1CheckBox);

    glGenBuffers(1, vboCharacter1CheckBox);

    glBindBuffer(GL_ARRAY_BUFFER, vboCharacter1CheckBox[0]);
    glBufferData(GL_ARRAY_BUFFER, Character1CheckBox.size() * sizeof(float), Character1CheckBox.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
// ĳ����
AABB character1 = {
    character1Position + glm::vec3(-0.70f, 0.0f, -0.72f),
    character1Position + glm::vec3(0.70f, 1.84f, 0.63f)
};

// ĳ����2
void InitCharacter2Acc() {
    InitPart("Character2/accessories.obj", modelCharacter2Acc, vaoCharacter2Acc, vboCharacter2Acc, glm::vec3(1.0f, 0.078f, 0.576f));
}
void InitCharacter2Body() {
    InitPart("Character2/body.obj", modelCharacter2Body, vaoCharacter2Body, vboCharacter2Body, glm::vec3(1.0f, 0.714f, 0.757f));
}
void InitCharacter2Clothes() {
    InitPart("Character2/clothes.obj", modelCharacter2Clothes, vaoCharacter2Clothes, vboCharacter2Clothes, glm::vec3(0.0f, 0.0f, 0.0f));
}
void InitCharacter2Hair() {
    InitPart("Character2/hair.obj", modelCharacter2Hair, vaoCharacter2Hair, vboCharacter2Hair, glm::vec3(1.0f, 1.0f, 0.4f));
}
void InitCharacter2LeftLeg() {
    InitPart("Character2/leftLeg.obj", modelCharacter2LeftLeg, vaoCharacter2LeftLeg, vboCharacter2LeftLeg, glm::vec3(1.0f, 0.714f, 0.757f));
}
void InitCharacter2LeftArm() {
    InitPart("Character2/leftArm.obj", modelCharacter2LeftArm, vaoCharacter2LeftArm, vboCharacter2LeftArm, glm::vec3(1.0f, 0.714f, 0.757f));
}
void InitCharacter2RightLeg() {
    InitPart("Character2/rightLeg.obj", modelCharacter2RightLeg, vaoCharacter2RightLeg, vboCharacter2RightLeg, glm::vec3(1.0f, 0.714f, 0.757f));
}
void InitCharacter2RightArm() {
    InitPart("Character2/rightArm.obj", modelCharacter2RightArm, vaoCharacter2RightArm, vboCharacter2RightArm, glm::vec3(1.0f, 0.714f, 0.757f));
}
void InitCharacter2Eye() {
    InitPart("Character2/eye.obj", modelCharacter2Eye, vaoCharacter2Eye, vboCharacter2Eye, glm::vec3(0.0f, 0.0f, 0.0f));
}
void InitCharacter2Face() {
    InitPart("Character2/face.obj", modelCharacter2Face, vaoCharacter2Face, vboCharacter2Face, glm::vec3(1.0f, 1.0, 0.941f));
}

//üũ�ڽ�
std::vector<float> Character2CheckBox = {
    // Bottom
-0.47f, 0.f, 0.42f,
-0.47f, 0.f, -0.48f,
0.47f, 0.f, -0.48f,
0.47f, 0.f, 0.42f,

// Top
-0.47f, 1.84f, 0.42f,
-0.47f, 1.84f, -0.48f,
0.47f, 1.84f, -0.48f,
0.47f, 1.84f, 0.42f,

// Front
-0.47f, 0.f, -0.48f,
-0.47f, 1.84f, -0.48f,
0.47f, 1.84f, -0.48f,
0.47f, 0.f, -0.48f,

// Back face
-0.47f, 0.f, 0.42f,
-0.47f, 1.84f, 0.42f,
0.47f, 1.84f, 0.42f,
0.47f, 0.f, 0.42f,

// Left face
-0.47f, 0.f, 0.42f,
-0.47f, 0.f, -0.48f,
-0.47f, 1.84f, -0.48f,
-0.47f, 1.84f, 0.42f,

// Right face
0.47f, 0.f, 0.42f,
0.47f, 0.f, -0.48f,
0.47f, 1.84f, -0.48f,
0.47f, 1.84f, 0.42f,
};
void InitCharacter2CheckBox() {
    glGenVertexArrays(1, &vaoCharacter2CheckBox);
    glBindVertexArray(vaoCharacter2CheckBox);

    glGenBuffers(1, vboCharacter2CheckBox);

    glBindBuffer(GL_ARRAY_BUFFER, vboCharacter2CheckBox[0]);
    glBufferData(GL_ARRAY_BUFFER, Character2CheckBox.size() * sizeof(float), Character2CheckBox.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
AABB character2 = {
    character2Position + glm::vec3(-0.70f, 0.0f, -0.72f),
    character2Position + glm::vec3(0.70f, 1.84f, 0.63f)
};

// �� �浹�ڽ�
std::vector<float> CheckBoxVerticesMap1 = {
    // Bottom
    -22.5f, -2.0f,  0.0f,
    -22.5f,  0.0f,  0.0f,
    -22.5f, -2.0f, -80.0f,
    -22.5f,  0.0f, -80.0f,

    // Top
    22.5f, -2.0f,  0.0f,
    22.5f,  0.0f,  0.0f,
    22.5f, -2.0f, -80.0f,
    22.5f,  0.0f, -80.0f,

    // Front
    -22.5f, -2.0f, -80.0f,
    22.5f, -2.0f, -80.0f,
    22.5f,  0.0f, -80.0f,
    -22.5f,  0.0f, -80.0f,

    // Back face
    -22.5f, -2.0f,  0.0f,
    22.5f, -2.0f,  0.0f,
    22.5f,  0.0f,  0.0f,
    -22.5f,  0.0f,  0.0f,

    // Left face
    -22.5f, -2.0f,  0.0f,
    -22.5f, -2.0f, -80.0f,
    -22.5f,  0.0f, -80.0f,
    -22.5f,  0.0f,  0.0f,

    // Right face
    22.5f, -2.0f,  0.0f,
    22.5f, -2.0f, -80.0f,
    22.5f,  0.0f, -80.0f,
    22.5f,  0.0f,  0.0f
};
std::vector<float> CheckBoxVerticesMap2 = {
    // Bottom
    -18.0f, -2.3f, -79.0f,
    -18.0f, -0.3f, -79.0f,
    -18.0f, -2.3f, -121.0f,
    -18.0f, -0.3f, -121.0f,

    // Top
    18.0f, -2.3f, -79.0f,
    18.0f, -0.3f, -79.0f,
    18.0f, -2.3f, -121.0f,
    18.0f, -0.3f, -121.0f,

    // Front
    -18.0f, -2.3f, -121.0f,
    18.0f, -2.3f, -121.0f,
    18.0f, -0.3f, -121.0f,
    -18.0f, -0.3f, -121.0f,

    // Back face
    -18.0f, -2.3f, -79.0f,
    18.0f, -2.3f, -79.0f,
    18.0f, -0.3f, -79.0f,
    -18.0f, -0.3f, -79.0f,

    // Left face
    -18.0f, -2.3f, -79.0f,
    -18.0f, -2.3f, -121.0f,
    -18.0f, -0.3f, -121.0f,
    -18.0f, -0.3f, -79.0f,

    // Right face
    18.0f, -2.3f, -79.0f,
    18.0f, -2.3f, -121.0f,
    18.0f, -0.3f, -121.0f,
    18.0f, -0.3f, -79.0f
};
std::vector<float> CheckBoxVerticesMap3 = {
    // Bottom
    -14.0f, -2.6f, -120.0f,
    -14.0f, -0.6f, -120.0f,
    -14.0f, -2.6f, -146.0f,
    -14.0f, -0.6f, -146.0f,

    // Top
    14.0f, -2.6f, -120.0f,
    14.0f, -0.6f, -120.0f,
    14.0f, -2.6f, -146.0f,
    14.0f, -0.6f, -146.0f,

    // Front
    -14.0f, -2.6f, -146.0f,
    14.0f, -2.6f, -146.0f,
    14.0f, -0.6f, -146.0f,
    -14.0f, -0.6f, -146.0f,

    // Back face
    -14.0f, -2.6f, -120.0f,
    14.0f, -2.6f, -120.0f,
    14.0f, -0.6f, -120.0f,
    -14.0f, -0.6f, -120.0f,

    // Left face
    -14.0f, -2.6f, -120.0f,
    -14.0f, -2.6f, -146.0f,
    -14.0f, -0.6f, -146.0f,
    -14.0f, -0.6f, -120.0f,

    // Right face
    14.0f, -2.6f, -120.0f,
    14.0f, -2.6f, -146.0f,
    14.0f, -0.6f, -146.0f,
    14.0f, -0.6f, -120.0f
};
std::vector<float> CheckBoxVerticesMap4 = {
    // Bottom
    -11.6f, -2.8f, -143.0f,
    -11.6f, -0.8f, -143.0f,
    -11.6f, -2.8f, -165.0f,
    -11.6f, -0.8f, -165.0f,

    // Top
    11.6f, -2.8f, -143.0f,
    11.6f, -0.8f, -143.0f,
    11.6f, -2.8f, -165.0f,
    11.6f, -0.8f, -165.0f,

    // Front
    -11.6f, -2.8f, -165.0f,
    11.6f, -2.8f, -165.0f,
    11.6f, -0.8f, -165.0f,
    -11.6f, -0.8f, -165.0f,

    // Back face
    -11.6f, -2.8f, -143.0f,
    11.6f, -2.8f, -143.0f,
    11.6f, -0.8f, -143.0f,
    -11.6f, -0.8f, -143.0f,

    // Left face
    -11.6f, -2.8f, -143.0f,
    -11.6f, -2.8f, -165.0f,
    -11.6f, -0.8f, -165.0f,
    -11.6f, -0.8f, -143.0f,

    // Right face
    11.6f, -2.8f, -143.0f,
    11.6f, -2.8f, -165.0f,
    11.6f, -0.8f, -165.0f,
    11.6f, -0.8f, -143.0f
};
std::vector<float> CheckBoxVerticesMap5 = {
    // Bottom
     -10.6f, -28.5f, -165.0f,
     -10.6f, -26.5f, -165.0f,
     -10.6f, -28.5f, -245.0f,
     -10.6f, -26.5f, -245.0f,

     // Top
     10.6f, -28.5f, -165.0f,
     10.6f, -26.5f, -165.0f,
     10.6f, -28.5f, -245.0f,
     10.6f, -26.5f, -245.0f,

     // Front
     -10.6f, -28.5f, -245.0f,
     10.6f, -28.5f, -245.0f,
     10.6f, -26.5f, -245.0f,
     -10.6f, -26.5f, -245.0f,

     // Back face
     -10.6f, -28.5f, -165.0f,
     10.6f, -28.5f, -165.0f,
     10.6f, -26.5f, -165.0f,
     -10.6f, -26.5f, -165.0f,

     // Left face
     -10.6f, -28.5f, -165.0f,
     -10.6f, -28.5f, -245.0f,
     -10.6f, -26.5f, -245.0f,
     -10.6f, -26.5f, -165.0f,

     // Right face
     10.6f, -28.5f, -165.0f,
     10.6f, -28.5f, -245.0f,
     10.6f, -26.5f, -245.0f,
     10.6f, -26.5f, -165.0f
};

void InitCheckBoxMap1() {
    glGenVertexArrays(1, &vaoCheckBoxMap1);
    glBindVertexArray(vaoCheckBoxMap1);

    glGenBuffers(1, vboCheckBoxMap1);

    glBindBuffer(GL_ARRAY_BUFFER, vboCheckBoxMap1[0]);
    glBufferData(GL_ARRAY_BUFFER, CheckBoxVerticesMap1.size() * sizeof(float), CheckBoxVerticesMap1.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
void InitCheckBoxMap2() {
    glGenVertexArrays(1, &vaoCheckBoxMap2);
    glBindVertexArray(vaoCheckBoxMap2);

    glGenBuffers(1, vboCheckBoxMap2);

    glBindBuffer(GL_ARRAY_BUFFER, vboCheckBoxMap2[0]);
    glBufferData(GL_ARRAY_BUFFER, CheckBoxVerticesMap2.size() * sizeof(float), CheckBoxVerticesMap2.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
void InitCheckBoxMap3() {
    glGenVertexArrays(1, &vaoCheckBoxMap3);
    glBindVertexArray(vaoCheckBoxMap3);

    glGenBuffers(1, vboCheckBoxMap3);

    glBindBuffer(GL_ARRAY_BUFFER, vboCheckBoxMap3[0]);
    glBufferData(GL_ARRAY_BUFFER, CheckBoxVerticesMap3.size() * sizeof(float), CheckBoxVerticesMap3.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
void InitCheckBoxMap4() {
    glGenVertexArrays(1, &vaoCheckBoxMap4);
    glBindVertexArray(vaoCheckBoxMap4);

    glGenBuffers(1, vboCheckBoxMap4);

    glBindBuffer(GL_ARRAY_BUFFER, vboCheckBoxMap4[0]);
    glBufferData(GL_ARRAY_BUFFER, CheckBoxVerticesMap4.size() * sizeof(float), CheckBoxVerticesMap4.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
void InitCheckBoxMap5() {
    glGenVertexArrays(1, &vaoCheckBoxMap5);
    glBindVertexArray(vaoCheckBoxMap5);

    glGenBuffers(1, vboCheckBoxMap5);

    glBindBuffer(GL_ARRAY_BUFFER, vboCheckBoxMap5[0]);
    glBufferData(GL_ARRAY_BUFFER, CheckBoxVerticesMap5.size() * sizeof(float), CheckBoxVerticesMap5.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

// �� �׸���
void DrawMap(GLuint shaderPRogramID, GLint modelMatrixLocation) {
    // �ٴ�
    glm::mat4 bottomModelMatrix = glm::mat4(1.0f);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(bottomModelMatrix));

    glBindVertexArray(vaoBottom);
    glDrawElements(GL_TRIANGLES, modelBottom.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // ȭ��ǥ�� ���
    glm::mat4 arrowAndPillarModelMatrix = glm::mat4(1.0f);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(arrowAndPillarModelMatrix));

    glBindVertexArray(vaoArrowAndPillar);
    glDrawElements(GL_TRIANGLES, modelArrowAndPillar.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // �����
    glm::mat4 endPointModelMatrix = glm::mat4(1.0f);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(endPointModelMatrix));

    glBindVertexArray(vaoEndPoint);
    glDrawElements(GL_TRIANGLES, modelEndPoint.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // ����Ʈ
    glm::mat4 pointModelMatrix = glm::mat4(1.0f);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(pointModelMatrix));

    glBindVertexArray(vaoPoint);
    glDrawElements(GL_TRIANGLES, modelPoint.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// ĳ����1 �׸���
void DrawCharacter1(GLuint shaderProgramID, GLint modelMatrixLocation) {
    glm::mat4 finalCharacter1ModelMatrix = character1ModelMatrix;

    // ��
    glm::mat4 Character1BodyModelMatrix = finalCharacter1ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character1BodyModelMatrix));
    glBindVertexArray(vaoCharacter1Body);
    glDrawElements(GL_TRIANGLES, modelCharacter1Body.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // � ���� ��
    glm::mat4 Character1BackPatternModelMatrix = finalCharacter1ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character1BackPatternModelMatrix));
    glBindVertexArray(vaoCharacter1BackPattern);
    glDrawElements(GL_TRIANGLES, modelCharacter1BackPattern.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // ����
    glm::mat4 Character1BlusherModelMatrix = finalCharacter1ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character1BlusherModelMatrix));
    glBindVertexArray(vaoCharacter1Blusher);
    glDrawElements(GL_TRIANGLES, modelCharacter1Blusher.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // ��
    glm::mat4 Character1EyeModelMatrix = finalCharacter1ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character1EyeModelMatrix));
    glBindVertexArray(vaoCharacter1Eye);
    glDrawElements(GL_TRIANGLES, modelCharacter1Eye.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // ��
    glm::mat4 Character1FaceModelMatrix = finalCharacter1ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character1FaceModelMatrix));
    glBindVertexArray(vaoCharacter1Face);
    glDrawElements(GL_TRIANGLES, modelCharacter1Face.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // ����
    glm::mat4 Character1LeftArmModelMatrix = finalCharacter1ModelMatrix;
    Character1LeftArmModelMatrix = glm::translate(Character1LeftArmModelMatrix, glm::vec3(0.0f, 1.0f, 0.0f));
    Character1LeftArmModelMatrix = glm::rotate(Character1LeftArmModelMatrix, glm::radians(character1ArmLegSwingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    Character1LeftArmModelMatrix = glm::translate(Character1LeftArmModelMatrix, glm::vec3(0.0f, -1.0f, 0.0f));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character1LeftArmModelMatrix));
    glBindVertexArray(vaoCharacter1LeftArm);
    glDrawElements(GL_TRIANGLES, modelCharacter1LeftArm.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // ������
    glm::mat4 Character1RightArmModelMatrix = finalCharacter1ModelMatrix;
    Character1RightArmModelMatrix = glm::translate(Character1RightArmModelMatrix, glm::vec3(0.0f, 1.0f, 0.0f));
    Character1RightArmModelMatrix = glm::rotate(Character1RightArmModelMatrix, glm::radians(-character1ArmLegSwingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    Character1RightArmModelMatrix = glm::translate(Character1RightArmModelMatrix, glm::vec3(0.0f, -1.0f, 0.0f));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character1RightArmModelMatrix));
    glBindVertexArray(vaoCharacter1RightArm);
    glDrawElements(GL_TRIANGLES, modelCharacter1RightArm.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // �޴ٸ�
    glm::mat4 Character1LeftLegModelMatrix = finalCharacter1ModelMatrix;
    Character1LeftLegModelMatrix = glm::translate(Character1LeftLegModelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
    Character1LeftLegModelMatrix = glm::rotate(Character1LeftLegModelMatrix, glm::radians(-character1ArmLegSwingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    Character1LeftLegModelMatrix = glm::translate(Character1LeftLegModelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character1LeftLegModelMatrix));
    glBindVertexArray(vaoCharacter1LeftLeg);
    glDrawElements(GL_TRIANGLES, modelCharacter1LeftLeg.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // �����ٸ�
    glm::mat4 Character1RightLegModelMatrix = finalCharacter1ModelMatrix;
    Character1RightLegModelMatrix = glm::translate(Character1RightLegModelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
    Character1RightLegModelMatrix = glm::rotate(Character1RightLegModelMatrix, glm::radians(character1ArmLegSwingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    Character1RightLegModelMatrix = glm::translate(Character1RightLegModelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character1RightLegModelMatrix));
    glBindVertexArray(vaoCharacter1RightLeg);
    glDrawElements(GL_TRIANGLES, modelCharacter1RightLeg.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //checkbox
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glm::mat4 Character1CheckBox = finalCharacter1ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character1CheckBox));
    glBindVertexArray(vaoCharacter1CheckBox);
    glDrawArrays(GL_QUADS, 0, 24);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// ĳ����2 �׸���
void DrawCharacter2(GLuint shaderProgramID, GLint modelMatrixLocation) {
    glm::mat4 finalCharacter2ModelMatrix = character2ModelMatrix;

    //acc
    glm::mat4 Character2AccModelMatrix = finalCharacter2ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character2AccModelMatrix));
    glBindVertexArray(vaoCharacter2Acc);
    glDrawElements(GL_TRIANGLES, modelCharacter2Acc.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //body
    glm::mat4 Character2BodyModelMatrix = finalCharacter2ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character2BodyModelMatrix));
    glBindVertexArray(vaoCharacter2Body);
    glDrawElements(GL_TRIANGLES, modelCharacter2Body.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //hair
    glm::mat4 Character2HairModelMatrix = finalCharacter2ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character2HairModelMatrix));
    glBindVertexArray(vaoCharacter2Hair);
    glDrawElements(GL_TRIANGLES, modelCharacter2Hair.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //leftLeg
    glm::mat4 Character2LeftLegModelMatrix = finalCharacter2ModelMatrix;
    Character2LeftLegModelMatrix = glm::translate(Character2LeftLegModelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
    Character2LeftLegModelMatrix = glm::rotate(Character2LeftLegModelMatrix, glm::radians(-character2ArmLegSwingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    Character2LeftLegModelMatrix = glm::translate(Character2LeftLegModelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character2LeftLegModelMatrix));
    glBindVertexArray(vaoCharacter2LeftLeg);
    glDrawElements(GL_TRIANGLES, modelCharacter2LeftLeg.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //leftArm
    glm::mat4 Character2LeftArmModelMatrix = finalCharacter2ModelMatrix;
    Character2LeftArmModelMatrix = glm::translate(Character2LeftArmModelMatrix, glm::vec3(0.0f, 1.0f, 0.0f));
    Character2LeftArmModelMatrix = glm::rotate(Character2LeftArmModelMatrix, glm::radians(character2ArmLegSwingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    Character2LeftArmModelMatrix = glm::translate(Character2LeftArmModelMatrix, glm::vec3(0.0f, -1.0f, 0.0f));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character2LeftArmModelMatrix));
    glBindVertexArray(vaoCharacter2LeftArm);
    glDrawElements(GL_TRIANGLES, modelCharacter2LeftArm.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //RightLeg
    glm::mat4 Character2RightLegModelMatrix = finalCharacter2ModelMatrix;
    Character2RightLegModelMatrix = glm::translate(Character2RightLegModelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
    Character2RightLegModelMatrix = glm::rotate(Character2RightLegModelMatrix, glm::radians(character2ArmLegSwingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    Character2RightLegModelMatrix = glm::translate(Character2RightLegModelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character2RightLegModelMatrix));
    glBindVertexArray(vaoCharacter2RightLeg);
    glDrawElements(GL_TRIANGLES, modelCharacter2RightLeg.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //RightArm
    glm::mat4 Character2RightArmModelMatrix = finalCharacter2ModelMatrix;
    Character2RightArmModelMatrix = glm::translate(Character2RightArmModelMatrix, glm::vec3(0.0f, 1.0f, 0.0f));
    Character2RightArmModelMatrix = glm::rotate(Character2RightArmModelMatrix, glm::radians(-character2ArmLegSwingAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    Character2RightArmModelMatrix = glm::translate(Character2RightArmModelMatrix, glm::vec3(0.0f, -1.0f, 0.0f));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character2RightArmModelMatrix));
    glBindVertexArray(vaoCharacter2RightArm);
    glDrawElements(GL_TRIANGLES, modelCharacter2RightArm.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //Clothes
    glm::mat4 Character2ClothesModelMatrix = finalCharacter2ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character2ClothesModelMatrix));
    glBindVertexArray(vaoCharacter2Clothes);
    glDrawElements(GL_TRIANGLES, modelCharacter2Clothes.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //Eye
    glm::mat4 Character2EyeModelMatrix = finalCharacter2ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character2EyeModelMatrix));
    glBindVertexArray(vaoCharacter2Eye);
    glDrawElements(GL_TRIANGLES, modelCharacter2Eye.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //Face
    glm::mat4 Character2FaceModelMatrix = finalCharacter2ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character2FaceModelMatrix));
    glBindVertexArray(vaoCharacter2Face);
    glDrawElements(GL_TRIANGLES, modelCharacter2Face.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //checkbox
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glm::mat4 Character2CheckBox = finalCharacter2ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character2CheckBox));
    glBindVertexArray(vaoCharacter2CheckBox);
    glDrawArrays(GL_QUADS, 0, 24);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// üũ�ڽ� �׸���
void DrawMapCheckBox(GLuint shaderProgramID, GLint modelMatrixLocation) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glm::mat4 checkBoxModelMatrix1 = glm::mat4(1.0f);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(checkBoxModelMatrix1));
    glBindVertexArray(vaoCheckBoxMap1);
    glDrawArrays(GL_QUADS, 0, 24);
    glBindVertexArray(0);

    glm::mat4 checkBoxModelMatrix2 = glm::mat4(1.0f);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(checkBoxModelMatrix2));
    glBindVertexArray(vaoCheckBoxMap2);
    glDrawArrays(GL_QUADS, 0, 24);
    glBindVertexArray(0);

    glm::mat4 checkBoxModelMatrix3 = glm::mat4(1.0f);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(checkBoxModelMatrix3));
    glBindVertexArray(vaoCheckBoxMap3);
    glDrawArrays(GL_QUADS, 0, 24);
    glBindVertexArray(0);

    glm::mat4 checkBoxModelMatrix4 = glm::mat4(1.0f);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(checkBoxModelMatrix4));
    glBindVertexArray(vaoCheckBoxMap4);
    glDrawArrays(GL_QUADS, 0, 24);
    glBindVertexArray(0);

    glm::mat4 checkBoxModelMatrix5 = glm::mat4(1.0f);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(checkBoxModelMatrix5));
    glBindVertexArray(vaoCheckBoxMap5);
    glDrawArrays(GL_QUADS, 0, 24);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// ��ֹ� �׸���
void DrawObstacleBong(GLuint shaderPRogramID, GLint modelMatrixLocation) {
    glm::mat4 finalBong1ModelMatrix = bong1ModelMatrix;
    bong1ModelMatrix = glm::translate(glm::mat4(1.0f), BongGroup1Position);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(finalBong1ModelMatrix));

    glBindVertexArray(vaoBong1);
    glDrawElements(GL_TRIANGLES, modelBong1.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glm::mat4 finalBong2ModelMatrix = bong2ModelMatrix;
    bong2ModelMatrix = glm::translate(glm::mat4(1.0f), BongGroup2Position);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(finalBong2ModelMatrix));

    glBindVertexArray(vaoBong2);
    glDrawElements(GL_TRIANGLES, modelBong2.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
void DrawObstacleHorizontalFan(GLuint shaderPRogramID, GLint modelMatrixLocation) {
    glm::mat4 HorizontalFanPink1ModelMatrix = glm::mat4(1.0f);
    HorizontalFanPink1ModelMatrix = glm::translate(HorizontalFanPink1ModelMatrix, glm::vec3(0.0f, 0.0f, -140.0f));
    HorizontalFanPink1ModelMatrix = glm::rotate(HorizontalFanPink1ModelMatrix, glm::radians(obstacleRotation), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(HorizontalFanPink1ModelMatrix));

    glBindVertexArray(vaoHorizontalFanPink);
    glDrawElements(GL_TRIANGLES, modelHorizontalFanPink.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glm::mat4 HorizontalFanPurple1ModelMatrix = glm::mat4(1.0f);
    HorizontalFanPurple1ModelMatrix = glm::translate(HorizontalFanPurple1ModelMatrix, glm::vec3(0.0f, 0.0f, -140.0f));
    HorizontalFanPurple1ModelMatrix = glm::rotate(HorizontalFanPurple1ModelMatrix, glm::radians(obstacleRotation), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(HorizontalFanPurple1ModelMatrix));

    glBindVertexArray(vaoHorizontalFanPurple);
    glDrawElements(GL_TRIANGLES, modelHorizontalFanPurple.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glm::mat4 HorizontalFanPink2ModelMatrix = glm::mat4(1.0f);
    HorizontalFanPink2ModelMatrix = glm::translate(HorizontalFanPink2ModelMatrix, glm::vec3(7.0f, 0.0f, -115.0f));
    HorizontalFanPink2ModelMatrix = glm::rotate(HorizontalFanPink2ModelMatrix, glm::radians(-obstacleRotation), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(HorizontalFanPink2ModelMatrix));

    glBindVertexArray(vaoHorizontalFanPink);
    glDrawElements(GL_TRIANGLES, modelHorizontalFanPink.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glm::mat4 HorizontalFanPurple2ModelMatrix = glm::mat4(1.0f);
    HorizontalFanPurple2ModelMatrix = glm::translate(HorizontalFanPurple2ModelMatrix, glm::vec3(7.0f, 0.0f, -115.0f));
    HorizontalFanPurple2ModelMatrix = glm::rotate(HorizontalFanPurple2ModelMatrix, glm::radians(-obstacleRotation), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(HorizontalFanPurple2ModelMatrix));

    glBindVertexArray(vaoHorizontalFanPurple);
    glDrawElements(GL_TRIANGLES, modelHorizontalFanPurple.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glm::mat4 HorizontalFanPink3ModelMatrix = glm::mat4(1.0f);
    HorizontalFanPink3ModelMatrix = glm::translate(HorizontalFanPink3ModelMatrix, glm::vec3(-7.0f, 0.0f, -115.0f));
    HorizontalFanPink3ModelMatrix = glm::rotate(HorizontalFanPink3ModelMatrix, glm::radians(obstacleRotation), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(HorizontalFanPink3ModelMatrix));

    glBindVertexArray(vaoHorizontalFanPink);
    glDrawElements(GL_TRIANGLES, modelHorizontalFanPink.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glm::mat4 HorizontalFanPurple3ModelMatrix = glm::mat4(1.0f);
    HorizontalFanPurple3ModelMatrix = glm::translate(HorizontalFanPurple3ModelMatrix, glm::vec3(-7.0f, 0.0f, -115.0f));
    HorizontalFanPurple3ModelMatrix = glm::rotate(HorizontalFanPurple3ModelMatrix, glm::radians(obstacleRotation), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(HorizontalFanPurple3ModelMatrix));

    glBindVertexArray(vaoHorizontalFanPurple);
    glDrawElements(GL_TRIANGLES, modelHorizontalFanPurple.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
void DrawObstacleDoor(GLuint shaderPRogramID, GLint modelMatrixLocation) {
    glm::mat4 DooroutModelMatrix = glm::mat4(1.0f);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(DooroutModelMatrix));

    glBindVertexArray(vaoDoorOut);
    glDrawElements(GL_TRIANGLES, modelDoorOut.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glm::mat4 finalLeftdoorModelMatrix = LeftdoorModelMatrix;
    LeftdoorModelMatrix = glm::translate(glm::mat4(1.0f), LeftdoorGroupPosition);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(finalLeftdoorModelMatrix));

    glBindVertexArray(vaoLeftdoor);
    glDrawElements(GL_TRIANGLES, modelLeftdoor.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glm::mat4 finalRightdoorModelMatrix = RightdoorModelMatrix;
    RightdoorModelMatrix = glm::translate(glm::mat4(1.0f), RightdoorGroupPosition);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(finalRightdoorModelMatrix));

    glBindVertexArray(vaoRightdoor);
    glDrawElements(GL_TRIANGLES, modelRightdoor.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
void DrawObstacleJumpbar(GLuint shaderPRogramID, GLint modelMatrixLocation) {
    glm::mat4 JumpBarCenterModelMatrix = glm::mat4(1.0f);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(JumpBarCenterModelMatrix));

    glBindVertexArray(vaoJumpBarCenter);
    glDrawElements(GL_TRIANGLES, modelJumpBarCenter.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glm::mat4 JumpBarbargroup1ModelMatrix = glm::mat4(1.0f);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(JumpBarbargroup1ModelMatrix));

    glBindVertexArray(vaoJumpBarbargroup1);
    glDrawElements(GL_TRIANGLES, modelJumpBarbargroup1.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glm::mat4 JumpBarbargroup2ModelMatrix = glm::mat4(1.0f);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(JumpBarbargroup2ModelMatrix));

    glBindVertexArray(vaoJumpBarbargroup2);
    glDrawElements(GL_TRIANGLES, modelJumpBarbargroup2.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(window_Width, window_Height);
    glutCreateWindow("team_project");

    glewExperimental = GL_TRUE;
    glewInit();

    make_shaderProgram();

    // ��
    InitBottom();
    InitArrowAndPillar();
    InitEndPoint();
    InitPoint();

    // ĳ����1
    InitCharacter1Body();
    InitCharacter1BackPattern();
    InitCharacter1Blusher();
    InitCharacter1Eye();
    InitCharacter1Face();
    InitCharacter1LeftArm();
    InitCharacter1RightArm();
    InitCharacter1LeftLeg();
    InitCharacter1RightLeg();

    // ĳ����2
    InitCharacter2Acc();
    InitCharacter2Body();
    InitCharacter2Clothes();
    InitCharacter2Hair();
    InitCharacter2LeftLeg();
    InitCharacter2LeftArm();
    InitCharacter2RightLeg();
    InitCharacter2RightArm();
    InitCharacter2Eye();
    InitCharacter2Face();
    InitCharacter1CheckBox();
    InitCharacter2CheckBox();

    //mapcheckbox
    InitCheckBoxMap1();
    InitCheckBoxMap2();
    InitCheckBoxMap3();
    InitCheckBoxMap4();
    InitCheckBoxMap5();

    //��ֹ�
    InitBong1();
    InitBong2();
    InitHorizontalFanPink();
    InitHorizontalFanPurple();
    InitDoorOut();
    InitDoorLeft();
    InitDoorRight();
    InitJumpbarCenter();
    InitJumpbarbargroup1();
    InitJumpbarbargroup2();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutKeyboardUpFunc(KeyboardUp);
    glutKeyboardUpFunc(KeyboardUp);
    glutSpecialFunc(SpecialKey);
    glutSpecialUpFunc(SpecialKeyUp);
    glutTimerFunc(16, Timer, 0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDisable(GL_CULL_FACE);

    glutMainLoop();
}

void make_vertexShaders() {
    GLchar* vertexSource;

    vertexSource = filetobuf("vertex.glsl");

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
    glCompileShader(vertexShader);

    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
        return;
    }
}
void make_fragmentShaders() {
    GLchar* fragmentSource;

    fragmentSource = filetobuf("fragment.glsl");

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
    glCompileShader(fragmentShader);

    GLint result;
    GLchar errorLog[512];

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        std::cerr << "ERROR: frag_shader ������ ����\n" << errorLog << std::endl;
        return;
    }
}
GLuint make_shaderProgram() {
    make_vertexShaders();
    make_fragmentShaders();

    shaderProgramID = glCreateProgram();

    glAttachShader(shaderProgramID, vertexShader);
    glAttachShader(shaderProgramID, fragmentShader);
    glLinkProgram(shaderProgramID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgramID);
    return shaderProgramID;
}

GLvoid drawScene() {
    glClearColor(1.0, 1.0, 1.0, 1.0f);  // ����� ������� ����
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgramID);

    // ī�޶� ��ġ ���
    glm::vec3 cameraPosition = character1Position + glm::vec3(0.0f, 10.0f, 15.0f);
    glm::vec3 cameraTarget = character1Position;

    // �� ��ȯ
    glm::mat4 viewMatrix = glm::lookAt(
        cameraPosition,  // ī�޶� ��ġ
        cameraTarget,    // ī�޶� �ٶ󺸴� ����
        glm::vec3(0.0f, 1.0f, 0.0f) // ī�޶� ���� ����
    );

    // �� ����� ���̴��� ����
    GLint viewMatrixLocation = glGetUniformLocation(shaderProgramID, "viewTransform");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // ���� ��ȯ
    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(45.0f),
        (float)window_Width / (float)window_Height,
        0.1f,
        10000.0f
    );

    GLint projMatrixLocation = glGetUniformLocation(shaderProgramID, "projectionTransform");
    glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    GLint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelTransform");

    DrawMap(shaderProgramID, modelMatrixLocation);
    DrawObstacleBong(shaderProgramID, modelMatrixLocation);
    DrawCharacter1(shaderProgramID, modelMatrixLocation);
    DrawCharacter2(shaderProgramID, modelMatrixLocation);
    DrawMapCheckBox(shaderProgramID, modelMatrixLocation);
    DrawObstacleHorizontalFan(shaderProgramID, modelMatrixLocation);
    DrawObstacleDoor(shaderProgramID, modelMatrixLocation);
    DrawObstacleJumpbar(shaderProgramID, modelMatrixLocation);

    glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'a' || key == 's' || key == 'd') {
        moveKeyStates[key] = true;
    }
    else {
        switch (key) {
        case 'q':
            glutLeaveMainLoop();
            break;
        case ' ':
            if (!isCharacter1Jumping) {
                isCharacter1Jumping = true;
            }
            break;
        case 'j':
            if (!isCharacter2Jumping) {
                isCharacter2Jumping = true;
            }
            break;
        }
    }
    glutPostRedisplay();
}

void KeyboardUp(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'a' || key == 's' || key == 'd') {
        moveKeyStates[key] = false;
    }
}

void SpecialKey(int key, int x, int y) {
    if (key == GLUT_KEY_UP || key == GLUT_KEY_DOWN || key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT) {
        arrowKeyStates[key] = true;
    }
}

void SpecialKeyUp(int key, int x, int y) {
    if (key == GLUT_KEY_UP || key == GLUT_KEY_DOWN || key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT) {
        arrowKeyStates[key] = false;
    }
}

GLvoid Timer(int value) {
    // ĳ����1 �̵� ó��
    if (moveKeyStates['w']) {
        character1Direction = glm::vec3(0.0f, 0.0f, -moveSpeed);
        character1RotationAngle = 0.0f;
        isCharacter1Swing = true;
    }
    else if (moveKeyStates['s']) {
        character1Direction = glm::vec3(0.0f, 0.0f, moveSpeed);
        character1RotationAngle = 180.0f;
        isCharacter1Swing = true;
    }
    else if (moveKeyStates['a']) {
        character1Direction = glm::vec3(-moveSpeed, 0.0f, 0.0f);
        character1RotationAngle = 90.0f;
        isCharacter1Swing = true;
    }
    else if (moveKeyStates['d']) {
        character1Direction = glm::vec3(moveSpeed, 0.0f, 0.0f);
        character1RotationAngle = -90.0f;
        isCharacter1Swing = true;
    }
    else {
        character1Direction = glm::vec3(0.0f, 0.0f, 0.0f);
        isCharacter1Swing = false;
    }

    AABB maps[] = { map1, map2, map3, map4, map5 };
    isCharacter1OnMap = false;
    for (const auto& map : maps) {
        if (checkCollision(character1, map)) {
            isCharacter1OnMap = true;
            break;
        }
    }

    // ���� ����
    if (isCharacter1Jumping) {
        character1Position.y += character1JumpSpeed;
        character1JumpSpeed -= gravity;

        if (character1JumpSpeed <= 0.0f && isCharacter1OnMap) {
            isCharacter1Jumping = false;
            character1JumpSpeed = 0.3f;
        }
    }
    else if (!isCharacter1OnMap) {
        character1Position.y -= realGravity;
    }

    // ĳ����2 �̵� ó��
    if (arrowKeyStates[GLUT_KEY_UP]) {
        character2Direction = glm::vec3(0.0f, 0.0f, -moveSpeed);
        character2RotationAngle = 0.0f;
        isCharacter2Swing = true;
    }
    else if (arrowKeyStates[GLUT_KEY_DOWN]) {
        character2Direction = glm::vec3(0.0f, 0.0f, moveSpeed);
        character2RotationAngle = 180.0f;
        isCharacter2Swing = true;
    }
    else if (arrowKeyStates[GLUT_KEY_LEFT]) {
        character2Direction = glm::vec3(-moveSpeed, 0.0f, 0.0f);
        character2RotationAngle = 90.0f;
        isCharacter2Swing = true;
    }
    else if (arrowKeyStates[GLUT_KEY_RIGHT]) {
        character2Direction = glm::vec3(moveSpeed, 0.0f, 0.0f);
        character2RotationAngle = -90.0f;
        isCharacter2Swing = true;
    }
    else {
        character2Direction = glm::vec3(0.0f, 0.0f, 0.0f);
        isCharacter2Swing = false;
    }

    isCharacter2OnMap = false;
    for (const auto& map : maps) {
        if (checkCollision(character2, map)) {
            isCharacter2OnMap = true;
            break;
        }
    }

    // ���� ����
    if (isCharacter2Jumping) {
        character2Position.y += character2JumpSpeed;
        character2JumpSpeed -= gravity;

        if (character2JumpSpeed <= 0.0f && isCharacter2OnMap) {
            isCharacter2Jumping = false;
            character2JumpSpeed = 0.3f;
        }
    }
    else if (!isCharacter2OnMap) {
        character2Position.y -= realGravity;
    }

    // ĳ����1 �� ��Ʈ���� ������Ʈ
    character1ModelMatrix = glm::translate(glm::mat4(1.0f), character1Position);
    character1ModelMatrix = glm::rotate(character1ModelMatrix, glm::radians(character1RotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));

    // AABB ������Ʈ
    character1.update(character1Position, glm::vec3(-0.7f, 0.0f, -0.72f), glm::vec3(0.7f, 1.84f, 0.63f));

    // ĳ����2 �� ��Ʈ���� ������Ʈ
    character2ModelMatrix = glm::translate(glm::mat4(1.0f), character2Position);
    character2ModelMatrix = glm::rotate(character2ModelMatrix, glm::radians(character2RotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));

    // AABB ������Ʈ
    character2.update(character2Position, glm::vec3(-0.70f, 0.0f, -0.72f), glm::vec3(0.70f, 1.84f, 0.63f));

    // �� ��鸲 ������Ʈ
    if (isCharacter1Swing) {
        character1ArmLegSwingAngle += character1SwingDirection * 2.0f;
        if (character1ArmLegSwingAngle >= maxSwingAngle) {
            character1SwingDirection = -1; // ���� ����
        }
        else if (character1ArmLegSwingAngle <= -maxSwingAngle) {
            character1SwingDirection = 1; // ���� ����
        }
    }
    else {
        // ��鸲 ��Ȱ��ȭ �� �ʱ� ���·� ����
        if (character1ArmLegSwingAngle > 0.0f) {
            character1ArmLegSwingAngle -= 2.0f;
            if (character1ArmLegSwingAngle < 0.0f) character1ArmLegSwingAngle = 0.0f;
        }
        else if (character1ArmLegSwingAngle < 0.0f) {
            character1ArmLegSwingAngle += 2.0f;
            if (character1ArmLegSwingAngle > 0.0f) character1ArmLegSwingAngle = 0.0f;
        }
    }

    if (isCharacter2Swing) {
        character2ArmLegSwingAngle += character2SwingDirection * 2.0f;
        if (character2ArmLegSwingAngle >= maxSwingAngle) {
            character2SwingDirection = -1;
        }
        else if (character2ArmLegSwingAngle <= -maxSwingAngle) {
            character2SwingDirection = 1;
        }
    }
    else {
        if (character2ArmLegSwingAngle > 0.0f) {
            character2ArmLegSwingAngle -= 2.0f;
            if (character2ArmLegSwingAngle < 0.0f) character2ArmLegSwingAngle = 0.0f;
        }
        else if (character2ArmLegSwingAngle < 0.0f) {
            character2ArmLegSwingAngle += 2.0f;
            if (character2ArmLegSwingAngle > 0.0f) character2ArmLegSwingAngle = 0.0f;
        }
    }

    // ĳ���� �� �浹 �˻�
    if (checkCollision(character1, character2)) {
        float overlapX = std::min(character1.max.x, character2.max.x) - std::max(character1.min.x, character2.min.x);
        float overlapZ = std::min(character1.max.z, character2.max.z) - std::max(character1.min.z, character2.min.z);

        if (overlapX < overlapZ) {
            if (character1Direction.x > 0.0f && character1.max.x > character2.min.x) {
                character1Direction.x = 0.0f;
            }
            else if (character1Direction.x < 0.0f && character1.min.x < character2.max.x) {
                character1Direction.x = 0.0f;
            }

            if (character2Direction.x > 0.0f && character2.max.x > character1.min.x) {
                character2Direction.x = 0.0f;
            }
            else if (character2Direction.x < 0.0f && character2.min.x < character1.max.x) {
                character2Direction.x = 0.0f;
            }
        }
        else {
            if (character1Direction.z > 0.0f && character1.max.z > character2.min.z) {
                character1Direction.z = 0.0f;
            }
            else if (character1Direction.z < 0.0f && character1.min.z < character2.max.z) {
                character1Direction.z = 0.0f;
            }

            if (character2Direction.z > 0.0f && character2.max.z > character1.min.z) {
                character2Direction.z = 0.0f;
            }
            else if (character2Direction.z < 0.0f && character2.min.z < character1.max.z) {
                character2Direction.z = 0.0f;
            }
        }
    }

    // �̵� ó��
    character1Position += character1Direction;
    character2Position += character2Direction;

    //�� �����̱�
    BongGroup1Position.x += BongGroup1Direction.x * BongMove;

    if (BongGroup1Position.x >= MaxBongMove) {
        BongGroup1Direction.x = -1; // �������� �̵�
    }
    else if (BongGroup1Position.x <= -MaxBongMove) {
        BongGroup1Direction.x = 1;  // ���������� �̵�
    }

    // �� �׷� 2 �����̱� (�ݴ� ����)
    BongGroup2Position.x += BongGroup2Direction.x * BongMove;
    if (BongGroup2Position.x >= MaxBongMove) {
        BongGroup2Direction.x = -1;
    }
    else if (BongGroup2Position.x <= -MaxBongMove) {
        BongGroup2Direction.x = 1;
    }

    if (isObstacleRotate) {
        obstacleRotation += 2.0f;
    }

    // ��¦ �����̱�
    LeftdoorGroupPosition.x += LeftdoorGroupDirection.x * DoorMove;

    if (LeftdoorGroupPosition.x >= 0.0) {
        LeftdoorGroupDirection.x = -1; // �������� �̵�
    }
    else if (LeftdoorGroupPosition.x <= -MaxDoorMove) {
        LeftdoorGroupDirection.x = 1;  // ���������� �̵�
    }

    RightdoorGroupPosition.x += RightdoorGroupDirection.x * DoorMove;
    if (RightdoorGroupPosition.x >= MaxDoorMove) {
        RightdoorGroupDirection.x = -1;
    }
    else if (RightdoorGroupPosition.x <= -0.0) {
        RightdoorGroupDirection.x = 1;
    }

    // ȭ�� ����
    glutPostRedisplay();
    glutTimerFunc(16, Timer, 0);
}