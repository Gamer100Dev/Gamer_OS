#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>
#include <string>
#include <map>
#include <array>

inline std::array<float, 16> makeOrtho(float left, float right, float bottom, float top)
{
    std::array<float, 16> m = {};
    m[0] = 2.0f / (right - left);
    m[5] = 2.0f / (top - bottom);
    m[10] = -1.0f;
    m[12] = -(right + left) / (right - left);
    m[13] = -(top + bottom) / (top - bottom);
    m[15] = 1.0f;
    return m;
}

struct Character {
    GLuint TextureID;
    int Width;
    int Height;
    int BearingX;
    int BearingY;
    GLuint Advance;
};

class Text
{
public:
    Text(const char *fontPath, int fontSize)
    {
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            throw std::runtime_error("Could not init FreeType Library");
        }
        FT_Face face;
        if (FT_New_Face(ft, fontPath, 0, &face)) {
            throw std::runtime_error("Failed to load font");
        }
        FT_Set_Pixel_Sizes(face, 0, fontSize);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

        for (unsigned char c = 32; c < 127; ++c) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                continue; // skip glyphs we can't load
            }
            GLuint tex;
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Character ch = {
                tex,
                static_cast<int>(face->glyph->bitmap.width),
                static_cast<int>(face->glyph->bitmap.rows),
                face->glyph->bitmap_left,
                face->glyph->bitmap_top,
                static_cast<GLuint>(face->glyph->advance.x)
            };
            mChars.insert({c, ch});
        }
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        compileShaders();

        glGenVertexArrays(1, &mVAO);
        glGenBuffers(1, &mVBO);
        glBindVertexArray(mVAO);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        mColor[0] = mColor[1] = mColor[2] = 1.0f;
    }

    ~Text()
    {
        for (auto &it : mChars) {
            glDeleteTextures(1, &it.second.TextureID);
        }
        glDeleteBuffers(1, &mVBO);
        glDeleteVertexArrays(1, &mVAO);
        glDeleteProgram(mProgram);
    }

    void setScreenSize(int w, int h)
    {
        mScreenW = w;
        mScreenH = h;
    }

    void setColor(float r, float g, float b)
    {
        mColor[0] = r; mColor[1] = g; mColor[2] = b;
    }

    // Render text at (x, y) without a background (no framebuffer)
    void render(const std::string &text, float x, float y, float scale = 1.0f)
    {
        glUseProgram(mProgram);
        auto proj = makeOrtho(0.0f, (float)mScreenW, 0.0f, (float)mScreenH);
        glUniformMatrix4fv(glGetUniformLocation(mProgram, "uProj"), 1, GL_FALSE, proj.data());
        glUniform3fv(glGetUniformLocation(mProgram, "uTextColor"), 1, mColor);

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(mVAO);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (char c : text)
        {
            Character ch = mChars[c];
            float xpos = x + ch.BearingX * scale;
            float ypos = y - (ch.Height - ch.BearingY) * scale;
            float w = ch.Width * scale;
            float h = ch.Height * scale;

            float verts[6][4] = {
                { xpos, ypos + h, 0.0f, 0.0f },
                { xpos, ypos, 0.0f, 1.0f },
                { xpos + w, ypos, 1.0f, 1.0f },

                { xpos, ypos + h, 0.0f, 0.0f },
                { xpos + w, ypos, 1.0f, 1.0f },
                { xpos + w, ypos + h, 1.0f, 0.0f }
            };
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            glBindBuffer(GL_ARRAY_BUFFER, mVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            x += (ch.Advance >> 6) * scale;
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_BLEND);
    }

private:
    void compileShaders()
    {
        const char *vs = R"glsl(
            #version 330 core
            layout(location = 0) in vec4 vPos;
            out vec2 TexCoord;
            uniform mat4 uProj;
            void main() {
                gl_Position = uProj * vec4(vPos.xy, 0.0, 1.0);
                TexCoord = vPos.zw;
            }
        )glsl";

        const char *fs = R"glsl(
            #version 330 core
            in vec2 TexCoord;
            out vec4 FragColor;
            uniform sampler2D uText;
            uniform vec3 uTextColor;
            void main() {
                float alpha = texture(uText, TexCoord).r;
                FragColor = vec4(uTextColor, alpha);
            }
        )glsl";

        GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vsID, 1, &vs, nullptr);
        glCompileShader(vsID);
        checkCompile(vsID, "VERTEX");

        GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fsID, 1, &fs, nullptr);
        glCompileShader(fsID);
        checkCompile(fsID, "FRAGMENT");

        mProgram = glCreateProgram();
        glAttachShader(mProgram, vsID);
        glAttachShader(mProgram, fsID);
        glLinkProgram(mProgram);
        checkCompile(mProgram, "PROGRAM");

        glDeleteShader(vsID);
        glDeleteShader(fsID);

        glUseProgram(mProgram);
        glUniform1i(glGetUniformLocation(mProgram, "uText"), 0);
    }

    void checkCompile(GLuint id, const char *type)
    {
        GLint success;
        GLchar info[1024];
        if (std::string(type) != "PROGRAM") {
            glGetShaderiv(id, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(id, 1024, nullptr, info);
                throw std::runtime_error(std::string("Shader compile error (") + type + "): " + info);
            }
        } else {
            glGetProgramiv(id, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(id, 1024, nullptr, info);
                throw std::runtime_error(std::string("Program link error: ") + info);
            }
        }
    }

    std::map<char, Character> mChars;
    GLuint mVAO = 0, mVBO = 0;
    GLuint mProgram = 0;
    int mScreenW = 800, mScreenH = 600;
    float mColor[3];
};
