#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include "헤더.h" // 필요한 경우에만 포함

// 캐릭터 초기화 함수 선언
void InitCharacter2Acc();
void InitCharacter2Body();
void InitCharacter2Clothes();
void InitCharacter2Hair();
void InitCharacter2LeftLeg();
void InitCharacter2LeftArm();
void InitCharacter2RightLeg();
void InitCharacter2RightArm();

// 캐릭터 렌더링 함수 선언
void DrawCharacter2(GLuint shaderProgramID, GLint modelMatrixLocation);

#endif // CHARACTER_H