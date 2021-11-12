
#include <MyGL.hpp>
#include <GLFWApplication.hpp>

using namespace mygl;

constexpr size_t NUM_SHADERS = 3;

class MyApp : public GLFWApplication {

public:
    MyApp() { m_name = "Mandelbrot"; }

    void run() override
    {
        offset = -screen_to_world(window_size() / 2);

        float vertices[] {
            -1.0f, 1.0f, //
            1.0f, 1.0f,  //
            1.0f, -1.0f, //

            -1.0f, 1.0f,  //
            -1.0f, -1.0f, //
            1.0f, -1.0f,  //
        };

        VertexBuffer buffer(vertices, sizeof(vertices));

        VertexLayout layout;
        layout.push<float>(2);

        VertexArray varray;
        varray.add_buffer(buffer, layout);

        Shader shader1 = load_shader("res/shader1");
        Shader shader2 = load_shader("res/shader2");
        Shader shader3 = load_shader("res/shader3");

        shaders[0] = &shader1;
        shaders[1] = &shader2;
        shaders[2] = &shader3;

        varray.bind();

        shader1.bind();
        shader1.set_uniform<int>("u_max_it", max_iterations);
        shader2.bind();
        shader2.set_uniform<int>("u_max_it", max_iterations);
        shader3.bind();
        shader3.set_uniform<int>("u_max_it", max_iterations);

        redraw();

        while (!glfwWindowShouldClose(m_window)) {
            glfwPollEvents();
        }
    }

    void redraw()
    {
        current_shader()->bind();
        current_shader()->set_uniform("u_one_over_scale", 1.0 / scale);
        current_shader()->set_uniform("u_offset", offset);

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(m_window);
    }

    void mouse_event(MouseEvent event) override
    {
        if (event.action == MouseAction::Press) {
            drag_pos = mouse_pos();
            dragging = true;
        } else if (event.action == MouseAction::Release) {
            dragging = false;
        }
    }

    void cursor_event(dvec2 pos) override
    {
        if (dragging) {
            offset -= (pos - drag_pos) / scale;
            drag_pos = pos;
            redraw();
        }
    }

    void key_event(KeyEvent event) override
    {
        if (event.action == KeyAction::Press) {
            if (event.key == Key::KeyRight) {
                shader_idx = (shader_idx + 1) % NUM_SHADERS;
                redraw();
            } else if (event.key == Key::KeyLeft) {
                shader_idx = (shader_idx - 1) % NUM_SHADERS;
                redraw();
            } else if (event.key == Key::KeyUp) {
                max_iterations += 500;
                current_shader()->set_uniform("u_max_it", max_iterations);
                redraw();
                printf("max_iterations: %d\n", max_iterations);
            } else if (event.key == Key::KeyDown) {
                max_iterations -= 500;
                current_shader()->set_uniform("u_max_it", max_iterations);
                redraw();
                printf("max_iterations: %d\n", max_iterations);
            }
        } else if (event.action == KeyAction::Repeat) {
            if (event.key == Key::KeyA) {
                zoom(0.9);
            } else if (event.key == Key::KeyQ) {
                zoom(1.1);
            }
        } else if (event.action == KeyAction::Release) {
        }
    }

    void zoom(double factor)
    {
        dvec2 mouse = mouse_pos();
        dvec2 pre_mouse = screen_to_world(mouse);

        scale *= factor;

        dvec2 post_mouse = screen_to_world(mouse);
        offset += pre_mouse - post_mouse;

        redraw();
    }

    void scroll_event(dvec2 off) override
    {
        zoom(off.y < 0.0 ? 0.9 : 1.1);
    }

    void resize_event(ivec2 size) override
    {
        glViewport(0, 0, size.x, size.y);
        redraw();
    }

    Shader load_shader(std::string folder_path)
    {
        ShaderBuilder builder;
        builder.add_shader(GL_VERTEX_SHADER, folder_path + "/vertex.glsl");
        builder.add_shader(GL_FRAGMENT_SHADER, folder_path + "/fragment.glsl");
        builder.compile_and_link();
        return builder.finish();
    }

    Shader* current_shader() { return shaders[shader_idx]; }

    dvec2 screen_to_world(dvec2 screen)
    {
        dvec2 res;
        res.x = screen.x / scale.x + offset.x;
        res.y = screen.y / scale.y + offset.y;
        return res;
    }

    dvec2 screen_to_world(ivec2 screen)
    {
        dvec2 res;
        res.x = double(screen.x) / scale.x + offset.x;
        res.y = double(screen.y) / scale.y + offset.y;
        return res;
    }

private:
    dvec2 scale { 200.0, 200.0 };
    dvec2 offset { 0.0, 0.0 };

    dvec2 drag_pos { 0, 0 };
    bool dragging = false;

    int max_iterations = 1000;

    Shader* shaders[NUM_SHADERS];
    size_t shader_idx = 0;

    int* m_iterations { nullptr };
};

int main()
{
    MyApp app;

    if (app.init(1280, 960)) {
        app.run();
        return 0;
    }

    return -1;
}