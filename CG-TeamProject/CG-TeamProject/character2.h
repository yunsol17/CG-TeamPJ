#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include "���.h" // �ʿ��� ��쿡�� ����

// ĳ���� �ʱ�ȭ �Լ� ����
void InitCharacter2Acc();
void InitCharacter2Body();
void InitCharacter2Clothes();
void InitCharacter2Hair();
void InitCharacter2LeftLeg();
void InitCharacter2LeftArm();
void InitCharacter2RightLeg();
void InitCharacter2RightArm();

// ĳ���� ������ �Լ� ����
void DrawCharacter2(GLuint shaderProgramID, GLint modelMatrixLocation);

#endif // CHARACTER_H