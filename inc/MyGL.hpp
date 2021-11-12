#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include <vector>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace mygl {
using namespace glm;

template<typename T> struct GLTypeTrait {
};

template<> struct GLTypeTrait<float> {
    static constexpr GLenum gl_type = GL_FLOAT;
    static void uniform(int location, float v) { glUniform1f(location, v); }
    static void uniform_array(int location, size_t count, const float* v) { glUniform1fv(location, count, v); }
};

template<> struct GLTypeTrait<vec2> {
    static void uniform(int location, vec2 v) { glUniform2f(location, v.x, v.y); }
    static void uniform_array(int location, size_t count, const vec2* v)
    {
        glUniform2fv(location, count, (const float*)v);
    }
};

template<> struct GLTypeTrait<vec3> {
    static void uniform(int location, vec3 v) { glUniform3f(location, v.x, v.y, v.z); }
    static void uniform_array(int location, size_t count, const vec3* v)
    {
        glUniform3fv(location, count, (const float*)v);
    }
};

template<> struct GLTypeTrait<vec4> {
    static void uniform(int location, vec4 v) { glUniform4f(location, v.x, v.y, v.z, v.w); }
    static void uniform_array(int location, size_t count, const vec4* v)
    {
        glUniform4fv(location, count, (const float*)v);
    }
};

template<> struct GLTypeTrait<double> {
    static constexpr GLenum gl_type = GL_DOUBLE;
    static void uniform(int location, double v) { glUniform1d(location, v); }
    static void uniform_array(int location, size_t count, const double* v) { glUniform1dv(location, count, v); }
};

template<> struct GLTypeTrait<dvec2> {
    static void uniform(int location, dvec2 v) { glUniform2d(location, v.x, v.y); }
    static void uniform_array(int location, size_t count, const dvec2* v)
    {
        glUniform2dv(location, count, (const double*)v);
    }
};

template<> struct GLTypeTrait<dvec3> {
    static void uniform(int location, dvec3 v) { glUniform3d(location, v.x, v.y, v.z); }
    static void uniform_array(int location, size_t count, const dvec3* v)
    {
        glUniform3dv(location, count, (const double*)v);
    }
};

template<> struct GLTypeTrait<dvec4> {
    static void uniform(int location, dvec4 v) { glUniform4d(location, v.x, v.y, v.z, v.w); }
    static void uniform_array(int location, size_t count, const dvec4* v)
    {
        glUniform4dv(location, count, (const double*)v);
    }
};

template<> struct GLTypeTrait<int> {
    static constexpr GLenum gl_type = GL_INT;
    static void uniform(int location, int v) { glUniform1i(location, v); }
    static void uniform_array(int location, size_t count, const int* v) { glUniform1iv(location, count, v); }
};

template<> struct GLTypeTrait<ivec2> {
    static void uniform(int location, ivec2 v) { glUniform2i(location, v.x, v.y); }
    static void uniform_array(int location, size_t count, const ivec2* v)
    {
        glUniform2iv(location, count, (const int*)v);
    }
};

template<> struct GLTypeTrait<ivec3> {
    static void uniform(int location, ivec3 v) { glUniform3i(location, v.x, v.y, v.z); }
    static void uniform_array(int location, size_t count, const ivec3* v)
    {
        glUniform3iv(location, count, (const int*)v);
    }
};

template<> struct GLTypeTrait<ivec4> {
    static void uniform(int location, ivec4 v) { glUniform4i(location, v.x, v.y, v.z, v.w); }
    static void uniform_array(int location, size_t count, const ivec4* v)
    {
        glUniform4iv(location, count, (const int*)v);
    }
};

template<> struct GLTypeTrait<unsigned int> {
    static constexpr GLenum gl_type = GL_UNSIGNED_INT;
    static void uniform(int location, unsigned int v) { glUniform1ui(location, v); }
    static void uniform_array(int location, size_t count, const unsigned int* v) { glUniform1uiv(location, count, v); }
};

template<> struct GLTypeTrait<uvec2> {
    static void uniform(int location, uvec2 v) { glUniform2ui(location, v.x, v.y); }
    static void uniform_array(int location, size_t count, const uvec2* v)
    {
        glUniform2uiv(location, count, (const unsigned int*)v);
    }
};

template<> struct GLTypeTrait<uvec3> {
    static void uniform(int location, uvec3 v) { glUniform3ui(location, v.x, v.y, v.z); }
    static void uniform_array(int location, size_t count, const uvec3* v)
    {
        glUniform3uiv(location, count, (const unsigned int*)v);
    }
};

template<> struct GLTypeTrait<uvec4> {
    static void uniform(int location, uvec4 v) { glUniform4ui(location, v.x, v.y, v.z, v.w); }
    static void uniform_array(int location, size_t count, const uvec4* v)
    {
        glUniform4uiv(location, count, (const unsigned int*)v);
    }
};

class ShaderBuilder;

class VertexBuffer {

public:
    VertexBuffer() { glGenBuffers(1, &m_id); }

    VertexBuffer(const void* data, size_t size)
    {
        glGenBuffers(1, &m_id);
        set_data(data, size);
    }

    void set_data(const void* data, size_t size)
    {
        bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    void bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_id); }

    void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

private:
    uint32_t m_id;
};

struct VertexBufferElement {
    uint32_t count;
    uint32_t type;
    uint32_t total_size_in_bytes;
    uint32_t normalized;
};

class VertexLayout {

public:
    VertexLayout() = default;

    template<typename T> void push(uint32_t count, bool normalized = false)
    {
        using TraitType = GLTypeTrait<T>;

        m_elements.push_back(
            VertexBufferElement { count, TraitType::gl_type, (uint32_t)sizeof(T) * count, normalized });
        m_stride += sizeof(T) * count;
    }

    uint32_t stride() const { return m_stride; }

    const std::vector<VertexBufferElement>& elements() const { return m_elements; }

private:
    uint32_t m_stride { 0 };
    std::vector<VertexBufferElement> m_elements;
};

class VertexArray {

public:
    VertexArray() { glGenVertexArrays(1, &m_id); }

    ~VertexArray() { glDeleteVertexArrays(1, &m_id); }

    void bind() const { glBindVertexArray(m_id); }

    void unbind() const { glBindVertexArray(0); }

    void add_buffer(const VertexBuffer& buffer, const VertexLayout& layout)
    {
        bind();
        buffer.bind();

        const auto& elements = layout.elements();

        uint32_t offset = 0;

        for (size_t i = 0; i < elements.size(); i++) {
            const auto& elem = elements[i];

            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, elem.count, elem.type, elem.normalized, layout.stride(), (const void*)offset);

            offset += elem.total_size_in_bytes;
        }
    }

private:
    uint32_t m_id { 0 };
};

class Shader {
    friend class ShaderBuilder;

public:
    void bind() const { glUseProgram(m_id); }

    void unbind() const { glUseProgram(0); }

    template<typename T> void set_uniform(const std::string& name, T value)
    {
        using TraitType = GLTypeTrait<T>;

        int loc = get_uniform_location(name);
        TraitType::uniform(loc, value);
    }

    template<typename T> void set_uniform_array(const std::string& name, size_t count, const T* values)
    {
        using TraitType = GLTypeTrait<T>;

        int loc = get_uniform_location(name);
        TraitType::uniform_array(loc, count, values);
    }

    int get_uniform_location(const std::string& name) const
    {
        if (auto it = uniform_chache.find(name); it != uniform_chache.end()) {
            return (*it).second;
        }

        int loc;
        loc = glGetUniformLocation(m_id, name.c_str());

        // TODO: maybe do something diffrent if -1 is returnd?

        uniform_chache[name] = loc;
        return loc;
    }

private:
    Shader(uint32_t id) { m_id = id; }

public:
    uint32_t m_id { 0 };
    mutable std::unordered_map<std::string, int> uniform_chache;
};

class ShaderBuilder {

public:
    ShaderBuilder() = default;
    ~ShaderBuilder() = default;

    bool add_shader(uint32_t shader_type, std::string path)
    {
        assert(
            shader_type == GL_VERTEX_SHADER || shader_type == GL_GEOMETRY_SHADER || shader_type == GL_FRAGMENT_SHADER);

        #pragma warning(suppress : 4996)
        auto* file = fopen(path.c_str(), "r");

        if (!file) {
            perror(path.c_str());
            return false;
        }

        fseek(file, 0, SEEK_END);
        size_t length = ftell(file);
        fseek(file, 0, SEEK_SET);

        std::string contents;
        // TODO: what if resize throws ????
        contents.resize(length);

        size_t num_read = fread(&contents[0], sizeof(char), length, file);

        fclose(file);

        #ifndef _WIN32
        if (num_read != length) {
            perror(path.c_str());
            return false;
        }
        #endif

        uint32_t shader_id = glCreateShader(shader_type);
        m_shader_ids[shader_type] = shader_id;

        const char* str = contents.c_str();
        int size = static_cast<int>(length);
        glShaderSource(shader_id, 1, &str, &size);

        return true;
    }

    bool compile_and_link()
    {
        m_program_id = glCreateProgram();

        for (auto pair : m_shader_ids) {
            auto shader_id = pair.second;
            auto shader_type = pair.first;
            glCompileShader(shader_id);
            glAttachShader(m_program_id, shader_id);
        }

        glLinkProgram(m_program_id);
        glValidateProgram(m_program_id);

        for (auto pair : m_shader_ids) {
            auto shader_id = pair.second;
            glDetachShader(m_program_id, shader_id);
            glDeleteShader(shader_id);
        }

        return true; // TODO: compile and link error checks
    }

    Shader finish() { return Shader(m_program_id); }

private:
    bool check_compile_error(uint32_t shader_id, uint32_t shader_type)
    {
        int result;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);

        if (result == GL_FALSE) {
            int len;
            glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);

            std::string msg;
            msg.resize(len);
            glGetShaderInfoLog(shader_id, len, &len, &msg[0]);

            std::string shader_name;
            switch (shader_type) {
            case GL_VERTEX_SHADER:
                shader_name = "vertex shader";
                break;
            case GL_GEOMETRY_SHADER:
                shader_name = "geometry shader";
                break;
            case GL_FRAGMENT_SHADER:
                shader_name = "fragment shader";
                break;
            default:
                shader_name = "unknown shader";
                break;
            }

            fprintf(stderr, "Failed to compile %s: %s\n", shader_name.c_str(), msg.c_str());

            return false;
        }

        return true;
    }

    bool check_link_error()
    {
        int result;
        glGetProgramiv(m_program_id, GL_LINK_STATUS, &result);

        if (result == GL_FALSE) {
            int len;
            glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &len);

            std::string msg;
            msg.resize(len);

            glGetProgramInfoLog(m_program_id, len, &len, &msg[0]);

            fprintf(stderr, "Shader failed to link: %s\n", msg.c_str());

            return false;
        }

        return true;
    }

private:
    uint32_t m_program_id;
    std::unordered_map<uint32_t, uint32_t> m_shader_ids;
};

}