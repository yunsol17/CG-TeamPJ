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
#include "헤더.h"

struct AABB {
    glm::vec3 min; // 충돌박스의 최소 좌표 (x, y, z)
    glm::vec3 max; // 충돌박스의 최대 좌표 (x, y, z)
};

AABB block1Box, block2Box, block3Box, robotBox, cubeFaceBoxes[6];

GLuint vaoBottom, vaoArrowAndPillar, vaoEndPoint, vaoPoint, vaoCharacter1Body, vaoCharacter1BackPattern, vaoCharacter1Blusher, vaoCharacter1Eye, vaoCharacter1Face, 
vaoCharacter1LeftArm, vaoCharacter1RightArm, vaoCharacter1LeftLeg, vaoCharacter1RightLeg, vaoCharacter2;
GLuint vboBottom[2], vboArrowAndPillar[2], vboEndPoint[2], vboPoint[2], vboCharacter1Body[2], vboCharacter1BackPattern[2], vboCharacter1Blusher[2], vboCharacter1Eye[2], vboCharacter1Face[2], 
vboCharacter1LeftArm[2], vboCharacter1RightArm[2], vboCharacter1LeftLeg[2], vboCharacter1RightLeg[2], vboCharacter2[2];
GLuint shaderProgramID;
GLuint vertexShader;
GLuint fragmentShader;
Model modelBottom, modelArrowAndPillar, modelEndPoint, modelPoint,modelCharacter1Body, modelCharacter1BackPattern, modelCharacter1Blusher, modelCharacter1Eye, modelCharacter1Face, 
modelCharacter1LeftArm, modelCharacter1RightArm, modelCharacter1LeftLeg, modelCharacter1RightLeg,modelCharacter2;

GLuint vaoCharacter2Acc, vboCharacter2Acc[2], vaoCharacter2Body, vboCharacter2Body[2], vaoCharacter2Clothes, vboCharacter2Clothes[2], vaoCharacter2Hair, vboCharacter2Hair[2], vaoCharacter2LeftLeg, vboCharacter2LeftLeg[2], vaoCharacter2RightLeg, vboCharacter2RightLeg[2], vaoCharacter2LeftArm, vboCharacter2LeftArm[2], vaoCharacter2RightArm, vboCharacter2RightArm[2];
Model modelCharacter2Acc, modelCharacter2Body, modelCharacter2Hair,modelCharacter2Clothes, modelCharacter2LeftLeg, modelCharacter2RightLeg, modelCharacter2LeftArm, modelCharacter2RightArm;


GLfloat cameraX = 0.0f;
GLfloat cameraY = 0.0f;
GLfloat cameraZ = 30.0f;

bool isCameraXmove = false;
bool isCameraXmoveReverse = false;
bool isCameraYmove = false;
bool isCameraYmoveReverse = false;
bool isCameraZmove = false;
bool isCameraZmoveReverse = false;

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

void InitBottom();
void InitArrowAndPillar();
void InitEndPoint();
void InitPoint();

void InitCharacter1Body();
void InitCharacter1BackPattern();
void InitCharacter1Blusher();
void InitCharacter1Eye();
void InitCharacter1Face();
void InitCharacter1LeftArm();
void InitCharacter1RightArm();
void InitCharacter1LeftLeg();
void InitCharacter1RightLeg();

GLuint make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Timer(int value);

int window_Width = 800;
int window_Height = 600;

void InitBuffer(GLuint& vao, GLuint* vbo, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(2, vbo);

    // 정점버퍼 (위치와 색상)
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // 버퍼 요소들 (인덱스)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // 정점을 사용하게 함
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);  // Position attribute
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));  // Color attribute
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);  // VAO 바인드 해제
}

void InitBottom() {
    read_obj_file("Map/bottom.obj", modelBottom);  // OBJ 파일 읽기

    std::vector<Vertex> expandedVertices;
    std::vector<unsigned int> indices;

    glm::vec3 color = glm::vec3(0.0f, 0.0f, 1.0f);

    for (size_t i = 0; i < modelBottom.faces.size(); ++i) {
        Vertex v1 = { modelBottom.vertices[modelBottom.faces[i].v1].x,
                     modelBottom.vertices[modelBottom.faces[i].v1].y,
                     modelBottom.vertices[modelBottom.faces[i].v1].z,
                     color };

        Vertex v2 = { modelBottom.vertices[modelBottom.faces[i].v2].x,
                     modelBottom.vertices[modelBottom.faces[i].v2].y,
                     modelBottom.vertices[modelBottom.faces[i].v2].z,
                     color };

        Vertex v3 = { modelBottom.vertices[modelBottom.faces[i].v3].x,
                     modelBottom.vertices[modelBottom.faces[i].v3].y,
                     modelBottom.vertices[modelBottom.faces[i].v3].z,
                     color };

        expandedVertices.push_back(v1);
        expandedVertices.push_back(v2);
        expandedVertices.push_back(v3);

        indices.push_back(expandedVertices.size() - 3);
        indices.push_back(expandedVertices.size() - 2);
        indices.push_back(expandedVertices.size() - 1);
    }

    InitBuffer(vaoBottom, vboBottom, expandedVertices, indices);  // InitBuffer 호출
}

void InitArrowAndPillar() {
    read_obj_file("Map/arrowAndPillar.obj", modelArrowAndPillar);  // OBJ 파일 읽기

    std::vector<Vertex> expandedVertices;
    std::vector<unsigned int> indices;

    glm::vec3 color = glm::vec3(1.0f, 1.0f, 0.0f);

    for (size_t i = 0; i < modelArrowAndPillar.faces.size(); ++i) {
        Vertex v1 = { modelArrowAndPillar.vertices[modelArrowAndPillar.faces[i].v1].x,
                     modelArrowAndPillar.vertices[modelArrowAndPillar.faces[i].v1].y,
                     modelArrowAndPillar.vertices[modelArrowAndPillar.faces[i].v1].z,
                     color };

        Vertex v2 = { modelArrowAndPillar.vertices[modelArrowAndPillar.faces[i].v2].x,
                     modelArrowAndPillar.vertices[modelArrowAndPillar.faces[i].v2].y,
                     modelArrowAndPillar.vertices[modelArrowAndPillar.faces[i].v2].z,
                     color };

        Vertex v3 = { modelArrowAndPillar.vertices[modelArrowAndPillar.faces[i].v3].x,
                     modelArrowAndPillar.vertices[modelArrowAndPillar.faces[i].v3].y,
                     modelArrowAndPillar.vertices[modelArrowAndPillar.faces[i].v3].z,
                     color };

        expandedVertices.push_back(v1);
        expandedVertices.push_back(v2);
        expandedVertices.push_back(v3);

        indices.push_back(expandedVertices.size() - 3);
        indices.push_back(expandedVertices.size() - 2);
        indices.push_back(expandedVertices.size() - 1);
    }

    InitBuffer(vaoArrowAndPillar, vboArrowAndPillar, expandedVertices, indices);  // InitBuffer 호출
}

void InitEndPoint() {
    read_obj_file("Map/endPoint.obj", modelEndPoint);  // OBJ 파일 읽기

    std::vector<Vertex> expandedVertices;
    std::vector<unsigned int> indices;

    glm::vec3 color = glm::vec3(1.0f, 0.0f, 1.0f);

    for (size_t i = 0; i < modelEndPoint.faces.size(); ++i) {
        Vertex v1 = { modelEndPoint.vertices[modelEndPoint.faces[i].v1].x,
                     modelEndPoint.vertices[modelEndPoint.faces[i].v1].y,
                     modelEndPoint.vertices[modelEndPoint.faces[i].v1].z,
                     color };

        Vertex v2 = { modelEndPoint.vertices[modelEndPoint.faces[i].v2].x,
                     modelEndPoint.vertices[modelEndPoint.faces[i].v2].y,
                     modelEndPoint.vertices[modelEndPoint.faces[i].v2].z,
                     color };

        Vertex v3 = { modelEndPoint.vertices[modelEndPoint.faces[i].v3].x,
                     modelEndPoint.vertices[modelEndPoint.faces[i].v3].y,
                     modelEndPoint.vertices[modelEndPoint.faces[i].v3].z,
                     color };

        expandedVertices.push_back(v1);
        expandedVertices.push_back(v2);
        expandedVertices.push_back(v3);

        indices.push_back(expandedVertices.size() - 3);
        indices.push_back(expandedVertices.size() - 2);
        indices.push_back(expandedVertices.size() - 1);
    }

    InitBuffer(vaoEndPoint, vboEndPoint, expandedVertices, indices);  // InitBuffer 호출
}

void InitPoint() {
    read_obj_file("Map/point.obj", modelPoint);  // OBJ 파일 읽기

    std::vector<Vertex> expandedVertices;
    std::vector<unsigned int> indices;

    glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

    for (size_t i = 0; i < modelPoint.faces.size(); ++i) {
        Vertex v1 = { modelPoint.vertices[modelPoint.faces[i].v1].x,
                     modelPoint.vertices[modelPoint.faces[i].v1].y,
                     modelPoint.vertices[modelPoint.faces[i].v1].z,
                     color };

        Vertex v2 = { modelPoint.vertices[modelPoint.faces[i].v2].x,
                     modelPoint.vertices[modelPoint.faces[i].v2].y,
                     modelPoint.vertices[modelPoint.faces[i].v2].z,
                     color };

        Vertex v3 = { modelPoint.vertices[modelPoint.faces[i].v3].x,
                     modelPoint.vertices[modelPoint.faces[i].v3].y,
                     modelPoint.vertices[modelPoint.faces[i].v3].z,
                     color };

        expandedVertices.push_back(v1);
        expandedVertices.push_back(v2);
        expandedVertices.push_back(v3);

        indices.push_back(expandedVertices.size() - 3);
        indices.push_back(expandedVertices.size() - 2);
        indices.push_back(expandedVertices.size() - 1);
    }

    InitBuffer(vaoPoint, vboPoint, expandedVertices, indices);  // InitBuffer 호출
}

<<<<<<< HEAD
// 캐릭터1
void InitCharacter1Body() {
    read_obj_file("Character1/body.obj", modelCharacter1Body);  // OBJ 파일 읽기
=======
// Character2
void InitCharacter2Acc() {
    read_obj_file("Character2/accessories.obj", modelCharacter2Acc);  // OBJ 파일 읽기
>>>>>>> e8146b6c0d6b32c452fe4eed2786a6a46ba9c286

    std::vector<Vertex> expandedVertices;
    std::vector<unsigned int> indices;

<<<<<<< HEAD
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 0.0f);

    for (size_t i = 0; i < modelCharacter1Body.faces.size(); ++i) {
        Vertex v1 = { modelCharacter1Body.vertices[modelCharacter1Body.faces[i].v1].x,
                     modelCharacter1Body.vertices[modelCharacter1Body.faces[i].v1].y,
                     modelCharacter1Body.vertices[modelCharacter1Body.faces[i].v1].z,
                     color };

        Vertex v2 = { modelCharacter1Body.vertices[modelCharacter1Body.faces[i].v2].x,
                     modelCharacter1Body.vertices[modelCharacter1Body.faces[i].v2].y,
                     modelCharacter1Body.vertices[modelCharacter1Body.faces[i].v2].z,
                     color };

        Vertex v3 = { modelCharacter1Body.vertices[modelCharacter1Body.faces[i].v3].x,
                     modelCharacter1Body.vertices[modelCharacter1Body.faces[i].v3].y,
                     modelCharacter1Body.vertices[modelCharacter1Body.faces[i].v3].z,
=======
    glm::vec3 color = glm::vec3(1.0f, 0.078f, 0.576f);

    for (size_t i = 0; i < modelCharacter2Acc.faces.size(); ++i) {
        Vertex v1 = { modelCharacter2Acc.vertices[modelCharacter2Acc.faces[i].v1].x,
                     modelCharacter2Acc.vertices[modelCharacter2Acc.faces[i].v1].y,
                     modelCharacter2Acc.vertices[modelCharacter2Acc.faces[i].v1].z,
                     color };

        Vertex v2 = { modelCharacter2Acc.vertices[modelCharacter2Acc.faces[i].v2].x,
                     modelCharacter2Acc.vertices[modelCharacter2Acc.faces[i].v2].y,
                     modelCharacter2Acc.vertices[modelCharacter2Acc.faces[i].v2].z,
                     color };

        Vertex v3 = { modelCharacter2Acc.vertices[modelCharacter2Acc.faces[i].v3].x,
                     modelCharacter2Acc.vertices[modelCharacter2Acc.faces[i].v3].y,
                     modelCharacter2Acc.vertices[modelCharacter2Acc.faces[i].v3].z,
>>>>>>> e8146b6c0d6b32c452fe4eed2786a6a46ba9c286
                     color };

        expandedVertices.push_back(v1);
        expandedVertices.push_back(v2);
        expandedVertices.push_back(v3);

        indices.push_back(expandedVertices.size() - 3);
        indices.push_back(expandedVertices.size() - 2);
        indices.push_back(expandedVertices.size() - 1);
    }

<<<<<<< HEAD
    InitBuffer(vaoCharacter1Body, vboCharacter1Body, expandedVertices, indices);  // InitBuffer 호출
}

void InitCharacter1BackPattern() {
    read_obj_file("Character1/backPattern.obj", modelCharacter1BackPattern);  // OBJ 파일 읽기
=======
    InitBuffer(vaoCharacter2Acc, vboCharacter2Acc, expandedVertices, indices);  // InitBuffer 호출
}
void InitCharacter2Body() {
    read_obj_file("Character2/body.obj", modelCharacter2Body);  // OBJ 파일 읽기

    std::vector<Vertex> expandedVertices;
    std::vector<unsigned int> indices;

    glm::vec3 color = glm::vec3(1.0f, 1.0f, 0.941f);

    for (size_t i = 0; i < modelCharacter2Body.faces.size(); ++i) {
        Vertex v1 = { modelCharacter2Body.vertices[modelCharacter2Body.faces[i].v1].x,
                     modelCharacter2Body.vertices[modelCharacter2Body.faces[i].v1].y,
                     modelCharacter2Body.vertices[modelCharacter2Body.faces[i].v1].z,
                     color };

        Vertex v2 = { modelCharacter2Body.vertices[modelCharacter2Body.faces[i].v2].x,
                     modelCharacter2Body.vertices[modelCharacter2Body.faces[i].v2].y,
                     modelCharacter2Body.vertices[modelCharacter2Body.faces[i].v2].z,
                     color };

        Vertex v3 = { modelCharacter2Body.vertices[modelCharacter2Body.faces[i].v3].x,
                     modelCharacter2Body.vertices[modelCharacter2Body.faces[i].v3].y,
                     modelCharacter2Body.vertices[modelCharacter2Body.faces[i].v3].z,
                     color };

        expandedVertices.push_back(v1);
        expandedVertices.push_back(v2);
        expandedVertices.push_back(v3);

        indices.push_back(expandedVertices.size() - 3);
        indices.push_back(expandedVertices.size() - 2);
        indices.push_back(expandedVertices.size() - 1);
    }

    InitBuffer(vaoCharacter2Body, vboCharacter2Body, expandedVertices, indices);  // InitBuffer 호출
}
void InitCharacter2Clothes() {
    read_obj_file("Character2/clothes.obj", modelCharacter2Clothes);  // OBJ 파일 읽기
>>>>>>> e8146b6c0d6b32c452fe4eed2786a6a46ba9c286

    std::vector<Vertex> expandedVertices;
    std::vector<unsigned int> indices;

    glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);

<<<<<<< HEAD
    for (size_t i = 0; i < modelCharacter1BackPattern.faces.size(); ++i) {
        Vertex v1 = { modelCharacter1BackPattern.vertices[modelCharacter1BackPattern.faces[i].v1].x,
                     modelCharacter1BackPattern.vertices[modelCharacter1BackPattern.faces[i].v1].y,
                     modelCharacter1BackPattern.vertices[modelCharacter1BackPattern.faces[i].v1].z,
                     color };

        Vertex v2 = { modelCharacter1BackPattern.vertices[modelCharacter1BackPattern.faces[i].v2].x,
                     modelCharacter1BackPattern.vertices[modelCharacter1BackPattern.faces[i].v2].y,
                     modelCharacter1BackPattern.vertices[modelCharacter1BackPattern.faces[i].v2].z,
                     color };

        Vertex v3 = { modelCharacter1BackPattern.vertices[modelCharacter1BackPattern.faces[i].v3].x,
                     modelCharacter1BackPattern.vertices[modelCharacter1BackPattern.faces[i].v3].y,
                     modelCharacter1BackPattern.vertices[modelCharacter1BackPattern.faces[i].v3].z,
=======
    for (size_t i = 0; i < modelCharacter2Clothes.faces.size(); ++i) {
        Vertex v1 = { modelCharacter2Clothes.vertices[modelCharacter2Clothes.faces[i].v1].x,
                     modelCharacter2Clothes.vertices[modelCharacter2Clothes.faces[i].v1].y,
                     modelCharacter2Clothes.vertices[modelCharacter2Clothes.faces[i].v1].z,
                     color };

        Vertex v2 = { modelCharacter2Clothes.vertices[modelCharacter2Clothes.faces[i].v2].x,
                     modelCharacter2Clothes.vertices[modelCharacter2Clothes.faces[i].v2].y,
                     modelCharacter2Clothes.vertices[modelCharacter2Clothes.faces[i].v2].z,
                     color };

        Vertex v3 = { modelCharacter2Clothes.vertices[modelCharacter2Clothes.faces[i].v3].x,
                     modelCharacter2Clothes.vertices[modelCharacter2Clothes.faces[i].v3].y,
                     modelCharacter2Clothes.vertices[modelCharacter2Clothes.faces[i].v3].z,
>>>>>>> e8146b6c0d6b32c452fe4eed2786a6a46ba9c286
                     color };

        expandedVertices.push_back(v1);
        expandedVertices.push_back(v2);
        expandedVertices.push_back(v3);

        indices.push_back(expandedVertices.size() - 3);
        indices.push_back(expandedVertices.size() - 2);
        indices.push_back(expandedVertices.size() - 1);
    }

<<<<<<< HEAD
    InitBuffer(vaoCharacter1BackPattern, vboCharacter1BackPattern, expandedVertices, indices);  // InitBuffer 호출
}

void InitCharacter1Blusher() {
    read_obj_file("Character1/blusher.obj", modelCharacter1Blusher);  // OBJ 파일 읽기
=======
    InitBuffer(vaoCharacter2Clothes, vboCharacter2Clothes, expandedVertices, indices);  // InitBuffer 호출
}
void InitCharacter2Hair() {
    read_obj_file("Character2/hair.obj", modelCharacter2Hair);  // OBJ 파일 읽기
>>>>>>> e8146b6c0d6b32c452fe4eed2786a6a46ba9c286

    std::vector<Vertex> expandedVertices;
    std::vector<unsigned int> indices;

<<<<<<< HEAD
    glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

    for (size_t i = 0; i < modelCharacter1Blusher.faces.size(); ++i) {
        Vertex v1 = { modelCharacter1Blusher.vertices[modelCharacter1Blusher.faces[i].v1].x,
                     modelCharacter1Blusher.vertices[modelCharacter1Blusher.faces[i].v1].y,
                     modelCharacter1Blusher.vertices[modelCharacter1Blusher.faces[i].v1].z,
                     color };

        Vertex v2 = { modelCharacter1Blusher.vertices[modelCharacter1Blusher.faces[i].v2].x,
                     modelCharacter1Blusher.vertices[modelCharacter1Blusher.faces[i].v2].y,
                     modelCharacter1Blusher.vertices[modelCharacter1Blusher.faces[i].v2].z,
                     color };

        Vertex v3 = { modelCharacter1Blusher.vertices[modelCharacter1Blusher.faces[i].v3].x,
                     modelCharacter1Blusher.vertices[modelCharacter1Blusher.faces[i].v3].y,
                     modelCharacter1Blusher.vertices[modelCharacter1Blusher.faces[i].v3].z,
=======
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 0.4f);

    for (size_t i = 0; i < modelCharacter2Hair.faces.size(); ++i) {
        Vertex v1 = { modelCharacter2Hair.vertices[modelCharacter2Hair.faces[i].v1].x,
                     modelCharacter2Hair.vertices[modelCharacter2Hair.faces[i].v1].y,
                     modelCharacter2Hair.vertices[modelCharacter2Hair.faces[i].v1].z,
                     color };

        Vertex v2 = { modelCharacter2Hair.vertices[modelCharacter2Hair.faces[i].v2].x,
                     modelCharacter2Hair.vertices[modelCharacter2Hair.faces[i].v2].y,
                     modelCharacter2Hair.vertices[modelCharacter2Hair.faces[i].v2].z,
                     color };

        Vertex v3 = { modelCharacter2Hair.vertices[modelCharacter2Hair.faces[i].v3].x,
                     modelCharacter2Hair.vertices[modelCharacter2Hair.faces[i].v3].y,
                     modelCharacter2Hair.vertices[modelCharacter2Hair.faces[i].v3].z,
>>>>>>> e8146b6c0d6b32c452fe4eed2786a6a46ba9c286
                     color };

        expandedVertices.push_back(v1);
        expandedVertices.push_back(v2);
        expandedVertices.push_back(v3);

        indices.push_back(expandedVertices.size() - 3);
        indices.push_back(expandedVertices.size() - 2);
        indices.push_back(expandedVertices.size() - 1);
    }

<<<<<<< HEAD
    InitBuffer(vaoCharacter1Blusher, vboCharacter1Blusher, expandedVertices, indices);  // InitBuffer 호출
}

void InitCharacter1Eye() {
    read_obj_file("Character1/eye.obj", modelCharacter1Eye);  // OBJ 파일 읽기
=======
    InitBuffer(vaoCharacter2Hair, vboCharacter2Hair, expandedVertices, indices);  // InitBuffer 호출
}
void InitCharacter2LeftLeg() {
    read_obj_file("Character2/leftLeg.obj", modelCharacter2LeftLeg);  // OBJ 파일 읽기
>>>>>>> e8146b6c0d6b32c452fe4eed2786a6a46ba9c286

    std::vector<Vertex> expandedVertices;
    std::vector<unsigned int> indices;

<<<<<<< HEAD
    glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);

    for (size_t i = 0; i < modelCharacter1Eye.faces.size(); ++i) {
        Vertex v1 = { modelCharacter1Eye.vertices[modelCharacter1Eye.faces[i].v1].x,
                     modelCharacter1Eye.vertices[modelCharacter1Eye.faces[i].v1].y,
                     modelCharacter1Eye.vertices[modelCharacter1Eye.faces[i].v1].z,
                     color };

        Vertex v2 = { modelCharacter1Eye.vertices[modelCharacter1Eye.faces[i].v2].x,
                     modelCharacter1Eye.vertices[modelCharacter1Eye.faces[i].v2].y,
                     modelCharacter1Eye.vertices[modelCharacter1Eye.faces[i].v2].z,
                     color };

        Vertex v3 = { modelCharacter1Eye.vertices[modelCharacter1Eye.faces[i].v3].x,
                     modelCharacter1Eye.vertices[modelCharacter1Eye.faces[i].v3].y,
                     modelCharacter1Eye.vertices[modelCharacter1Eye.faces[i].v3].z,
=======
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 0.941f);

    for (size_t i = 0; i < modelCharacter2LeftLeg.faces.size(); ++i) {
        Vertex v1 = { modelCharacter2LeftLeg.vertices[modelCharacter2LeftLeg.faces[i].v1].x,
                     modelCharacter2LeftLeg.vertices[modelCharacter2LeftLeg.faces[i].v1].y,
                     modelCharacter2LeftLeg.vertices[modelCharacter2LeftLeg.faces[i].v1].z,
                     color };

        Vertex v2 = { modelCharacter2LeftLeg.vertices[modelCharacter2LeftLeg.faces[i].v2].x,
                     modelCharacter2LeftLeg.vertices[modelCharacter2LeftLeg.faces[i].v2].y,
                     modelCharacter2LeftLeg.vertices[modelCharacter2LeftLeg.faces[i].v2].z,
                     color };

        Vertex v3 = { modelCharacter2LeftLeg.vertices[modelCharacter2LeftLeg.faces[i].v3].x,
                     modelCharacter2LeftLeg.vertices[modelCharacter2LeftLeg.faces[i].v3].y,
                     modelCharacter2LeftLeg.vertices[modelCharacter2LeftLeg.faces[i].v3].z,
>>>>>>> e8146b6c0d6b32c452fe4eed2786a6a46ba9c286
                     color };

        expandedVertices.push_back(v1);
        expandedVertices.push_back(v2);
        expandedVertices.push_back(v3);

        indices.push_back(expandedVertices.size() - 3);
        indices.push_back(expandedVertices.size() - 2);
        indices.push_back(expandedVertices.size() - 1);
    }

<<<<<<< HEAD
    InitBuffer(vaoCharacter1Eye, vboCharacter1Eye, expandedVertices, indices);  // InitBuffer 호출
}

void InitCharacter1Face() {
    read_obj_file("Character1/face.obj", modelCharacter1Face);  // OBJ 파일 읽기

    std::vector<Vertex> expandedVertices;
    std::vector<unsigned int> indices;

    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

    for (size_t i = 0; i < modelCharacter1Face.faces.size(); ++i) {
        Vertex v1 = { modelCharacter1Face.vertices[modelCharacter1Face.faces[i].v1].x,
                     modelCharacter1Face.vertices[modelCharacter1Face.faces[i].v1].y,
                     modelCharacter1Face.vertices[modelCharacter1Face.faces[i].v1].z,
                     color };

        Vertex v2 = { modelCharacter1Face.vertices[modelCharacter1Face.faces[i].v2].x,
                     modelCharacter1Face.vertices[modelCharacter1Face.faces[i].v2].y,
                     modelCharacter1Face.vertices[modelCharacter1Face.faces[i].v2].z,
                     color };

        Vertex v3 = { modelCharacter1Face.vertices[modelCharacter1Face.faces[i].v3].x,
                     modelCharacter1Face.vertices[modelCharacter1Face.faces[i].v3].y,
                     modelCharacter1Face.vertices[modelCharacter1Face.faces[i].v3].z,
                     color };

        expandedVertices.push_back(v1);
        expandedVertices.push_back(v2);
        expandedVertices.push_back(v3);

        indices.push_back(expandedVertices.size() - 3);
        indices.push_back(expandedVertices.size() - 2);
        indices.push_back(expandedVertices.size() - 1);
    }

    InitBuffer(vaoCharacter1Face, vboCharacter1Face, expandedVertices, indices);  // InitBuffer 호출
}

void InitCharacter1LeftArm() {
    read_obj_file("Character1/leftArm.obj", modelCharacter1LeftArm);  // OBJ 파일 읽기

    std::vector<Vertex> expandedVertices;
    std::vector<unsigned int> indices;

    glm::vec3 color = glm::vec3(1.0f, 1.0f, 0.0f);

    for (size_t i = 0; i < modelCharacter1LeftArm.faces.size(); ++i) {
        Vertex v1 = { modelCharacter1LeftArm.vertices[modelCharacter1LeftArm.faces[i].v1].x,
                     modelCharacter1LeftArm.vertices[modelCharacter1LeftArm.faces[i].v1].y,
                     modelCharacter1LeftArm.vertices[modelCharacter1LeftArm.faces[i].v1].z,
                     color };

        Vertex v2 = { modelCharacter1LeftArm.vertices[modelCharacter1LeftArm.faces[i].v2].x,
                     modelCharacter1LeftArm.vertices[modelCharacter1LeftArm.faces[i].v2].y,
                     modelCharacter1LeftArm.vertices[modelCharacter1LeftArm.faces[i].v2].z,
                     color };

        Vertex v3 = { modelCharacter1LeftArm.vertices[modelCharacter1LeftArm.faces[i].v3].x,
                     modelCharacter1LeftArm.vertices[modelCharacter1LeftArm.faces[i].v3].y,
                     modelCharacter1LeftArm.vertices[modelCharacter1LeftArm.faces[i].v3].z,
                     color };

        expandedVertices.push_back(v1);
        expandedVertices.push_back(v2);
        expandedVertices.push_back(v3);

        indices.push_back(expandedVertices.size() - 3);
        indices.push_back(expandedVertices.size() - 2);
        indices.push_back(expandedVertices.size() - 1);
    }

    InitBuffer(vaoCharacter1LeftArm, vboCharacter1LeftArm, expandedVertices, indices);  // InitBuffer 호출
}

void InitCharacter1RightArm() {
    read_obj_file("Character1/rightArm.obj", modelCharacter1RightArm);  // OBJ 파일 읽기

    std::vector<Vertex> expandedVertices;
    std::vector<unsigned int> indices;

    glm::vec3 color = glm::vec3(1.0f, 1.0f, 0.0f);

    for (size_t i = 0; i < modelCharacter1RightArm.faces.size(); ++i) {
        Vertex v1 = { modelCharacter1RightArm.vertices[modelCharacter1RightArm.faces[i].v1].x,
                     modelCharacter1RightArm.vertices[modelCharacter1RightArm.faces[i].v1].y,
                     modelCharacter1RightArm.vertices[modelCharacter1RightArm.faces[i].v1].z,
                     color };

        Vertex v2 = { modelCharacter1RightArm.vertices[modelCharacter1RightArm.faces[i].v2].x,
                     modelCharacter1RightArm.vertices[modelCharacter1RightArm.faces[i].v2].y,
                     modelCharacter1RightArm.vertices[modelCharacter1RightArm.faces[i].v2].z,
                     color };

        Vertex v3 = { modelCharacter1RightArm.vertices[modelCharacter1RightArm.faces[i].v3].x,
                     modelCharacter1RightArm.vertices[modelCharacter1RightArm.faces[i].v3].y,
                     modelCharacter1RightArm.vertices[modelCharacter1RightArm.faces[i].v3].z,
                     color };

        expandedVertices.push_back(v1);
        expandedVertices.push_back(v2);
        expandedVertices.push_back(v3);

        indices.push_back(expandedVertices.size() - 3);
        indices.push_back(expandedVertices.size() - 2);
        indices.push_back(expandedVertices.size() - 1);
    }

    InitBuffer(vaoCharacter1RightArm, vboCharacter1RightArm, expandedVertices, indices);  // InitBuffer 호출
}

void InitCharacter1LeftLeg() {
    read_obj_file("Character1/leftLeg.obj", modelCharacter1LeftLeg);  // OBJ 파일 읽기

    std::vector<Vertex> expandedVertices;
    std::vector<unsigned int> indices;

    glm::vec3 color = glm::vec3(1.0f, 1.0f, 0.0f);

    for (size_t i = 0; i < modelCharacter1LeftLeg.faces.size(); ++i) {
        Vertex v1 = { modelCharacter1LeftLeg.vertices[modelCharacter1LeftLeg.faces[i].v1].x,
                     modelCharacter1LeftLeg.vertices[modelCharacter1LeftLeg.faces[i].v1].y,
                     modelCharacter1LeftLeg.vertices[modelCharacter1LeftLeg.faces[i].v1].z,
                     color };

        Vertex v2 = { modelCharacter1LeftLeg.vertices[modelCharacter1LeftLeg.faces[i].v2].x,
                     modelCharacter1LeftLeg.vertices[modelCharacter1LeftLeg.faces[i].v2].y,
                     modelCharacter1LeftLeg.vertices[modelCharacter1LeftLeg.faces[i].v2].z,
                     color };

        Vertex v3 = { modelCharacter1LeftLeg.vertices[modelCharacter1LeftLeg.faces[i].v3].x,
                     modelCharacter1LeftLeg.vertices[modelCharacter1LeftLeg.faces[i].v3].y,
                     modelCharacter1LeftLeg.vertices[modelCharacter1LeftLeg.faces[i].v3].z,
                     color };

        expandedVertices.push_back(v1);
        expandedVertices.push_back(v2);
        expandedVertices.push_back(v3);

        indices.push_back(expandedVertices.size() - 3);
        indices.push_back(expandedVertices.size() - 2);
        indices.push_back(expandedVertices.size() - 1);
    }

    InitBuffer(vaoCharacter1LeftLeg, vboCharacter1LeftLeg, expandedVertices, indices);  // InitBuffer 호출
}

void InitCharacter1RightLeg() {
    read_obj_file("Character1/rightLeg.obj", modelCharacter1RightLeg);  // OBJ 파일 읽기

    std::vector<Vertex> expandedVertices;
    std::vector<unsigned int> indices;

    glm::vec3 color = glm::vec3(1.0f, 1.0f, 0.0f);

    for (size_t i = 0; i < modelCharacter1RightLeg.faces.size(); ++i) {
        Vertex v1 = { modelCharacter1RightLeg.vertices[modelCharacter1RightLeg.faces[i].v1].x,
                     modelCharacter1RightLeg.vertices[modelCharacter1RightLeg.faces[i].v1].y,
                     modelCharacter1RightLeg.vertices[modelCharacter1RightLeg.faces[i].v1].z,
                     color };

        Vertex v2 = { modelCharacter1RightLeg.vertices[modelCharacter1RightLeg.faces[i].v2].x,
                     modelCharacter1RightLeg.vertices[modelCharacter1RightLeg.faces[i].v2].y,
                     modelCharacter1RightLeg.vertices[modelCharacter1RightLeg.faces[i].v2].z,
                     color };

        Vertex v3 = { modelCharacter1RightLeg.vertices[modelCharacter1RightLeg.faces[i].v3].x,
                     modelCharacter1RightLeg.vertices[modelCharacter1RightLeg.faces[i].v3].y,
                     modelCharacter1RightLeg.vertices[modelCharacter1RightLeg.faces[i].v3].z,
                     color };

        expandedVertices.push_back(v1);
        expandedVertices.push_back(v2);
        expandedVertices.push_back(v3);

        indices.push_back(expandedVertices.size() - 3);
        indices.push_back(expandedVertices.size() - 2);
        indices.push_back(expandedVertices.size() - 1);
    }

    InitBuffer(vaoCharacter1RightLeg, vboCharacter1RightLeg, expandedVertices, indices);  // InitBuffer 호출
=======
    InitBuffer(vaoCharacter2LeftLeg, vboCharacter2LeftLeg, expandedVertices, indices);  // InitBuffer 호출
>>>>>>> e8146b6c0d6b32c452fe4eed2786a6a46ba9c286
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
    InitBottom();
    InitArrowAndPillar();
    InitEndPoint();
    InitPoint();
    InitCharacter1Body();
    InitCharacter1BackPattern();
    InitCharacter1Blusher();
    InitCharacter1Eye();
    InitCharacter1Face();
    InitCharacter1LeftArm();
    InitCharacter1RightArm();
    InitCharacter1LeftLeg();
    InitCharacter1RightLeg();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
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
        std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
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
        std::cerr << "ERROR: frag_shader 컴파일 실패\n" << errorLog << std::endl;
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
    glClearColor(1.0, 1.0, 1.0, 1.0f);  // 배경을 흰색으로 설정
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgramID);

    // 뷰 변환
    glm::mat4 viewMatrix = glm::lookAt(
        glm::vec3(cameraX, cameraY, cameraZ),  // 카메라 위치
        glm::vec3(0.0f, 0.0f, 0.0f),   // 카메라가 바라보는 지점
        glm::vec3(0.0f, 1.0f, 0.0f)  // 위쪽 방향
    );

    // 뷰 행렬을 셰이더에 전달
    GLint viewMatrixLocation = glGetUniformLocation(shaderProgramID, "viewTransform");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // 투영 변환
    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(45.0f),
        (float)window_Width / (float)window_Height,
        0.1f,
        10000.0f
    );

    GLint projMatrixLocation = glGetUniformLocation(shaderProgramID, "projectionTransform");
    glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // 맵    ---------------------------------------------------------------------------------------------
    // 베이스 변환
    glm::mat4 baseMapModelMatrix = glm::mat4(1.0f);
    GLint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
    baseMapModelMatrix = glm::translate(baseMapModelMatrix, glm::vec3(0.0f, 1.7f, 5.0f));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(baseMapModelMatrix));

    // 바닥
    glm::mat4 bottomModelMatrix = baseMapModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(bottomModelMatrix));

    glBindVertexArray(vaoBottom);
    glDrawElements(GL_TRIANGLES, modelBottom.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // 화살표와 기둥
    glm::mat4 arrowAndPillarModelMatrix = baseMapModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(arrowAndPillarModelMatrix));

    glBindVertexArray(vaoArrowAndPillar);
    glDrawElements(GL_TRIANGLES, modelArrowAndPillar.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // 결승점
    glm::mat4 endPointModelMatrix = baseMapModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(endPointModelMatrix));

    glBindVertexArray(vaoEndPoint);
    glDrawElements(GL_TRIANGLES, modelEndPoint.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // 포인트
    glm::mat4 pointModelMatrix = baseMapModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(pointModelMatrix));

    glBindVertexArray(vaoPoint);
    glDrawElements(GL_TRIANGLES, modelPoint.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    // ------------------------------------------------------------------------------------------------------
    // 피카츄
    // 베이스 변환
    glm::mat4 baseCharacter1ModelMatrix = glm::mat4(1.0f);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(baseCharacter1ModelMatrix));

    // 몸
    glm::mat4 character1BodyModelMatrix = baseCharacter1ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(character1BodyModelMatrix));

    glBindVertexArray(vaoCharacter1Body);
    glDrawElements(GL_TRIANGLES, modelCharacter1Body.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // 뒤에 패턴
    glm::mat4 character1BackPatternModelMatrix = baseCharacter1ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(character1BackPatternModelMatrix));

    glBindVertexArray(vaoCharacter1BackPattern);
    glDrawElements(GL_TRIANGLES, modelCharacter1BackPattern.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // 블러셔
    glm::mat4 character1BlusherModelMatrix = baseCharacter1ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(character1BlusherModelMatrix));

    glBindVertexArray(vaoCharacter1Blusher);
    glDrawElements(GL_TRIANGLES, modelCharacter1Blusher.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // 눈
    glm::mat4 character1EyeModelMatrix = baseCharacter1ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(character1EyeModelMatrix));

    glBindVertexArray(vaoCharacter1Eye);
    glDrawElements(GL_TRIANGLES, modelCharacter1Eye.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // 얼굴
    glm::mat4 character1FaceModelMatrix = baseCharacter1ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(character1FaceModelMatrix));

    glBindVertexArray(vaoCharacter1Face);
    glDrawElements(GL_TRIANGLES, modelCharacter1Face.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // 왼쪽 팔
    glm::mat4 character1LeftArmModelMatrix = baseCharacter1ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(character1LeftArmModelMatrix));

    glBindVertexArray(vaoCharacter1LeftArm);
    glDrawElements(GL_TRIANGLES, modelCharacter1LeftArm.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // 오른쪽 팔
    glm::mat4 character1RightArmModelMatrix = baseCharacter1ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(character1RightArmModelMatrix));

    glBindVertexArray(vaoCharacter1RightArm);
    glDrawElements(GL_TRIANGLES, modelCharacter1RightArm.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // 왼쪽 다리
    glm::mat4 character1LeftLegModelMatrix = baseCharacter1ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(character1LeftLegModelMatrix));

    glBindVertexArray(vaoCharacter1LeftLeg);
    glDrawElements(GL_TRIANGLES, modelCharacter1LeftLeg.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // 오른쪽 다리
    glm::mat4 character1RightLegModelMatrix = baseCharacter1ModelMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(character1RightLegModelMatrix));

    glBindVertexArray(vaoCharacter1RightLeg);
    glDrawElements(GL_TRIANGLES, modelCharacter1RightLeg.faces.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

<<<<<<< HEAD
    glutSwapBuffers();
=======
    ////소녀
    //glm::mat4 Character2ModelMatrix = glm::mat4(1.0f);
    //Character2ModelMatrix = glm::translate(Character2ModelMatrix, glm::vec3(-4.0, -0.75, -5.0f));
    //Character2ModelMatrix = glm::rotate(Character2ModelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(Character2ModelMatrix));
    //glBindVertexArray(vaoCharacter2);
    //glDrawElements(GL_TRIANGLES, modelCharacter2.faces.size() * 3, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);
    //glutSwapBuffers();
>>>>>>> e8146b6c0d6b32c452fe4eed2786a6a46ba9c286
}

GLvoid Reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'q':
        glutLeaveMainLoop();
        break;
    case 'z':
        isCameraZmove = !isCameraZmove;
        break;
    case 'Z':
        isCameraZmoveReverse = !isCameraZmoveReverse;
        break;
    case 'x':
        isCameraXmove = !isCameraXmove;
        break;
    case 'X':
        isCameraXmoveReverse = !isCameraXmoveReverse;
        break;
    case 'y':
        isCameraYmove = !isCameraYmove;
        break;
    case 'Y':
        isCameraYmoveReverse = !isCameraYmoveReverse;
        break;
    }

    glutPostRedisplay();
}

GLvoid Timer(int value) {
    if (isCameraXmove) {
        if (isCameraXmoveReverse) {
            cameraX -= 0.1f;
        }
        else {
            cameraX += 0.1f;
        }
    }

    if (isCameraYmove) {
        if (isCameraYmoveReverse) {
            cameraY -= 0.1f;
        }
        else {
            cameraY += 0.1f;
        }
    }

    if (isCameraZmove) {
        if (isCameraZmoveReverse) {
            cameraZ -= 0.1f;
        }
        else {
            cameraZ += 0.1f;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, Timer, 0);
}