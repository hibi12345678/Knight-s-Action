//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "Shader.h"   


///////////////////////////////////////////////////////////////////////////////
//Vertex Struct
///////////////////////////////////////////////////////////////////////////////
struct Vertex {
    glm::vec3 position; //位置
    glm::vec2 uv; //UV
    glm::vec3 normal; //法線
};

///////////////////////////////////////////////////////////////////////////////
//Terrain class
///////////////////////////////////////////////////////////////////////////////
class Terrain
{
public:
    //=========================================================================
    // public methods.
    //========================================================================
    //コンストラクタ
	Terrain(Shader* shader, const Matrix4& view, const Matrix4& proj);

    //Getter,Setter
    glm::vec3 GetTranslate() const { return pos; }
    void SetAngle(float angle) { mAngle = angle; }
    void SetTranslate(glm::vec3 translate) { pos = translate; }
    void SetScale(glm::vec3 scale) { mScale = scale; }
    void SetTex(bool flag) { mflag = flag; }

    void GenerateTerrain(const Matrix4& view, const Matrix4& proj);
    GLuint loadTerrainTexture(const char* filePath);
    std::vector<float> generateHeightMap(int width, int height, float scale, float heightscle);
    glm::mat4 ConvertToGLM(const Matrix4& matrix);
    std::vector<float> loadHeightMap(const char* filePath, int& width, int& height);
    std::vector<Vertex> generateControlPoints(int gridWidth, int gridHeight, const std::vector<float>& heightMap);
    glm::vec3 CalculateNormal(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
    float fade(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }
    float lerp(float t, float a, float b) { return a + t * (b - a); }
    float grad(int hash, float x, float y);
    float perlin(float x, float y);

private:
    //=========================================================================
    // protected variables.
    //=========================================================================
    int p[512] = { 
    227, 167, 197, 127, 151, 7, 90, 148, 160, 94, 86, 28, 223, 93, 105, 122,
    46, 36, 63, 141, 96, 220, 74, 173, 72, 69, 111, 77, 146, 44, 171, 67,
    128, 91, 84, 195, 144, 132, 208, 149, 33, 206, 161, 82, 134, 210, 87, 23,
    137, 31, 188, 154, 25, 19, 79, 191, 174, 150, 119, 60, 61, 4, 193, 76,
    243, 136, 246, 241, 22, 47, 140, 104, 26, 17, 71, 21, 166, 199, 85, 165,
    110, 95, 109, 68, 236, 153, 179, 247, 6, 5, 202, 114, 66, 152, 232, 239,
    64, 216, 121, 186, 249, 209, 252, 178, 157, 215, 89, 254, 57, 143, 92, 183,
    230, 162, 40, 42, 16, 181, 29, 190, 175, 238, 182, 41, 172, 78, 250, 228,
    145, 81, 224, 38, 101, 117, 30, 35, 100, 53, 237, 138, 231, 212, 213, 0,
    170, 205, 135, 52, 15, 55, 129, 27, 124, 83, 130, 222, 115, 131, 126, 34,
    75, 107, 118, 113, 3, 244, 164, 176, 198, 225, 45, 204, 49, 10, 242, 200,
    180, 229, 98, 24, 70, 12, 106, 158, 48, 54, 73, 185, 147, 226, 218, 37,
    234, 112, 251, 133, 2, 159, 80, 50, 235, 219, 139, 221, 192, 125, 142, 248,
    8, 189, 103, 211, 11, 245, 9, 116, 32, 120, 214, 65, 169, 88, 217, 1,
    58, 56, 59, 99, 253, 163, 168, 14, 51, 233, 196, 255, 20, 13, 194, 62,
    201, 187, 240, 156, 207, 43, 18, 39, 102, 203, 155, 97, 177, 184, 123, 108,
    227, 167, 197, 127, 151, 7, 90, 148, 160, 94, 86, 28, 223, 93, 105, 122,
    46, 36, 63, 141, 96, 220, 74, 173, 72, 69, 111, 77, 146, 44, 171, 67,
    128, 91, 84, 195, 144, 132, 208, 149, 33, 206, 161, 82, 134, 210, 87, 23,
    137, 31, 188, 154, 25, 19, 79, 191, 174, 150, 119, 60, 61, 4, 193, 76,
    243, 136, 246, 241, 22, 47, 140, 104, 26, 17, 71, 21, 166, 199, 85, 165,
    110, 95, 109, 68, 236, 153, 179, 247, 6, 5, 202, 114, 66, 152, 232, 239,
    64, 216, 121, 186, 249, 209, 252, 178, 157, 215, 89, 254, 57, 143, 92, 183,
    230, 162, 40, 42, 16, 181, 29, 190, 175, 238, 182, 41, 172, 78, 250, 228,
    145, 81, 224, 38, 101, 117, 30, 35, 100, 53, 237, 138, 231, 212, 213, 0,
    170, 205, 135, 52, 15, 55, 129, 27, 124, 83, 130, 222, 115, 131, 126, 34,
    75, 107, 118, 113, 3, 244, 164, 176, 198, 225, 45, 204, 49, 10, 242, 200,
    180, 229, 98, 24, 70, 12, 106, 158, 48, 54, 73, 185, 147, 226, 218, 37,
    234, 112, 251, 133, 2, 159, 80, 50, 235, 219, 139, 221, 192, 125, 142, 248,
    8, 189, 103, 211, 11, 245, 9, 116, 32, 120, 214, 65, 169, 88, 217, 1,
    58, 56, 59, 99, 253, 163, 168, 14, 51, 233, 196, 255, 20, 13, 194, 62,
    201, 187, 240, 156, 207, 43, 18, 39, 102, 203, 155, 97, 177, 184, 123, 108 }; //HeightMapのランダム生成用

    bool mflag;
    float mAngle;
    glm::vec3 mScale;

    GLuint VBO; //Terrain用VBO
    GLuint VAO; //Terrain用VAO
    GLuint textureID; //草テクスチャ
    GLuint rockTextureID; //岩tテクスチャ
    GLuint soilTextureID; //土テクスチャ
    GLuint snowTextureID; //雪テクスチャ
    GLuint numIndices; //インデックスバッファのサイズ 
    glm::vec3 pos;

    Shader* mShader; //Terrain用シェーダー
    std::vector<Vertex> controlPoint; //頂点情報
};