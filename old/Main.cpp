#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>

#include <string>
#include <complex>
#include <vector>
#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class ShaderBuilder;

template<typename T> struct Point {
    T x;
    T y;
};

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

template<typename T> struct OpenGLTypeTraits {
};

template<> struct OpenGLTypeTraits<float> {
    static constexpr uint32_t gl_type = GL_FLOAT;
    static constexpr uint32_t size_in_bytes = sizeof(float);
};

template<> struct OpenGLTypeTraits<uint32_t> {
    static constexpr uint32_t gl_type = GL_UNSIGNED_INT;
    static constexpr uint32_t size_in_bytes = sizeof(uint32_t);
};

class VertexLayout {

public:
    VertexLayout() = default;

    template<typename T> void push(uint32_t count, bool normalized = false)
    {
        using TraitType = OpenGLTypeTraits<T>;

        uint32_t norm = normalized ? GL_TRUE : GL_FALSE;

        m_elements.push_back(VertexBufferElement { count, TraitType::gl_type, TraitType::size_in_bytes * count, norm });
        m_stride += TraitType::size_in_bytes * count;
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

    int get_uniform_location(const std::string& name) const
    {
        if (auto it = uniform_chache.find(name); it != uniform_chache.end()) {
            return (*it).second;
        }

        int loc;

        loc = glGetUniformLocation(m_id, name.c_str());

        if (loc == -1) {
            // TODO: error handling
        }

        uniform_chache[name] = loc;
        return loc;
    }

private:
    Shader(uint32_t id) { m_id = id; }

private:
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

        if (num_read != length) {
            perror(path.c_str());
            return false;
        }

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
    uint32_t m_program_id;
    std::unordered_map<uint32_t, uint32_t> m_shader_ids;
};

class GLApplication {

public:
    GLApplication() = default;

    bool init(int width, int height)
    {
        glfwSetErrorCallback(glfw_error_callback);

        if (!glfwInit()) {
            return false;
        }

        glfw_window = glfwCreateWindow(640, 480, "fett", nullptr, nullptr);

        if (!glfw_window) {
            return false;
        }

        glfwMakeContextCurrent(glfw_window);

        if (glewInit() != GLEW_OK) {
            return false;
        }

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(gl_error_callback, this);

        return true;
    }

    void run()
    {
        results.resize(640 * 480);

        float vertices[] = {
            -1.0f, +1.0f, // Top Left
            -1.0f, -1.0f, // Bottom Left
            +1.0f, -1.0f, // Bottom Right

            -1.0f, +1.0f, // Top Left
            +1.0f, +1.0f, // Top Right
            +1.0f, -1.0f, // Bottom Right
        };

        VertexBuffer buffer(vertices, sizeof(vertices));
        VertexLayout layout;
        layout.push<float>(2);

        VertexArray va;
        va.add_buffer(buffer, layout);

        ShaderBuilder builder;
        builder.add_shader(GL_VERTEX_SHADER, "res/vertex.glsl");
        builder.add_shader(GL_FRAGMENT_SHADER, "res/fragment.glsl");

        builder.compile_and_link();

        Shader shader = builder.finish();

        buffer.bind();
        va.bind();
        shader.bind();

        GLclampf bg = 51.0f / 255.0f;
        glClearColor(bg, bg, bg, 1.0f);

        int uniform_loc = shader.get_uniform_location("iterations");

        while (!glfwWindowShouldClose(glfw_window)) {
            glClear(GL_COLOR_BUFFER_BIT);

            Point<int> pix_tl { 0, 0 };
            Point<int> pix_br { 640 , 480 };

            Point<double> frac_tl { -2.0, -1.0 };
            Point<double> frac_br { 1.0 , 1.0 };

            calculate_fractal_basic(pix_tl, pix_br, frac_tl, frac_br);

            glUniform1iv(uniform_loc, results.size(), results.data());

            glDrawArrays(GL_TRIANGLES, 0, 12);

            glfwSwapBuffers(glfw_window);
            glfwPollEvents();
        }
    }

    void calculate_fractal_basic(Point<int> pix_tl, Point<int> pix_br, Point<double> frac_tl, Point<double> frac_br)
    {
        double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
        double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

        for (int y = pix_tl.y; y < pix_br.y; y++) {
            for (int x = pix_tl.x; x < pix_br.x; x++) {
                std::complex z(0.0, 0.0);
                std::complex<double> c(x * x_scale + frac_tl.x, y * y_scale + frac_tl.y);

                int n = 0;

                while (abs(z) < 4.0 && n < 500) {
                    z = (z * z) + c;
                    n++;
                }

                results[y * 480 + x] = n;
            }
        }
    }

    ~GLApplication()
    {
        if (glfw_window) {
            glfwDestroyWindow(glfw_window);
        }
        glfwTerminate();
    }

private:
    static void glfw_error_callback(int error, const char* description)
    {
        fprintf(stderr, "GLFW Error: %s\n", description);
    }

    static APIENTRY void gl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
        const GLchar* message, const void* this_ptr)
    {
        fprintf(stderr, "OpenGL Info: %s\n", message);
    }

protected:
    std::string name { "OpenGL App" };

private:
    GLFWwindow* glfw_window { nullptr };

    std::vector<int> results;
};

int main()
{

    GLApplication app;

    if (app.init(640, 480)) {
        app.run();
    }

    return 0;
}
