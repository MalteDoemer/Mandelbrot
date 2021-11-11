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

enum class Key : int {
    KeySpace = GLFW_KEY_SPACE,
    KeyApostrophe = GLFW_KEY_APOSTROPHE,
    KeyComma = GLFW_KEY_COMMA,
    KeyMinus = GLFW_KEY_MINUS,
    KeyPeriod = GLFW_KEY_PERIOD,
    KeySlash = GLFW_KEY_SLASH,
    Key0 = GLFW_KEY_0,
    Key1 = GLFW_KEY_1,
    Key2 = GLFW_KEY_2,
    Key3 = GLFW_KEY_3,
    Key4 = GLFW_KEY_4,
    Key5 = GLFW_KEY_5,
    Key6 = GLFW_KEY_6,
    Key7 = GLFW_KEY_7,
    Key8 = GLFW_KEY_8,
    Key9 = GLFW_KEY_9,
    KeySemicolon = GLFW_KEY_SEMICOLON,
    KeyEqual = GLFW_KEY_EQUAL,
    KeyA = GLFW_KEY_A,
    KeyB = GLFW_KEY_B,
    KeyC = GLFW_KEY_C,
    KeyD = GLFW_KEY_D,
    KeyE = GLFW_KEY_E,
    KeyF = GLFW_KEY_F,
    KeyG = GLFW_KEY_G,
    KeyH = GLFW_KEY_H,
    KeyI = GLFW_KEY_I,
    KeyJ = GLFW_KEY_J,
    KeyK = GLFW_KEY_K,
    KeyL = GLFW_KEY_L,
    KeyM = GLFW_KEY_M,
    KeyN = GLFW_KEY_N,
    KeyO = GLFW_KEY_O,
    KeyP = GLFW_KEY_P,
    KeyQ = GLFW_KEY_Q,
    KeyR = GLFW_KEY_R,
    KeyS = GLFW_KEY_S,
    KeyT = GLFW_KEY_T,
    KeyU = GLFW_KEY_U,
    KeyV = GLFW_KEY_V,
    KeyW = GLFW_KEY_W,
    KeyX = GLFW_KEY_X,
    KeyY = GLFW_KEY_Y,
    KeyZ = GLFW_KEY_Z,
    KeyLeftBracket = GLFW_KEY_LEFT_BRACKET,
    KeyBackslash = GLFW_KEY_BACKSLASH,
    KeyRightBracket = GLFW_KEY_RIGHT_BRACKET,
    KeyGraveAccent = GLFW_KEY_GRAVE_ACCENT,
    KeyWorld1 = GLFW_KEY_WORLD_1,
    KeyWorld2 = GLFW_KEY_WORLD_2,

    KeyEscape = GLFW_KEY_ESCAPE,
    KeyEnter = GLFW_KEY_ENTER,
    KeyTab = GLFW_KEY_TAB,
    KeyBackspace = GLFW_KEY_BACKSPACE,
    KeyInsert = GLFW_KEY_INSERT,
    KeyDelete = GLFW_KEY_DELETE,
    KeyRight = GLFW_KEY_RIGHT,
    KeyLeft = GLFW_KEY_LEFT,
    KeyDown = GLFW_KEY_DOWN,
    KeyUp = GLFW_KEY_UP,
    KeyPage_up = GLFW_KEY_PAGE_UP,
    KeyPage_down = GLFW_KEY_PAGE_DOWN,
    KeyHome = GLFW_KEY_HOME,
    KeyEnd = GLFW_KEY_END,
    KeyCaps_lock = GLFW_KEY_CAPS_LOCK,
    KeyScroll_lock = GLFW_KEY_SCROLL_LOCK,
    KeyNum_lock = GLFW_KEY_NUM_LOCK,
    KeyPrint_screen = GLFW_KEY_PRINT_SCREEN,
    KeyPause = GLFW_KEY_PAUSE,
    KeyF1 = GLFW_KEY_F1,
    KeyF2 = GLFW_KEY_F2,
    KeyF3 = GLFW_KEY_F3,
    KeyF4 = GLFW_KEY_F4,
    KeyF5 = GLFW_KEY_F5,
    KeyF6 = GLFW_KEY_F6,
    KeyF7 = GLFW_KEY_F7,
    KeyF8 = GLFW_KEY_F8,
    KeyF9 = GLFW_KEY_F9,
    KeyF10 = GLFW_KEY_F10,
    KeyF11 = GLFW_KEY_F11,
    KeyF12 = GLFW_KEY_F12,
    KeyF13 = GLFW_KEY_F13,
    KeyF14 = GLFW_KEY_F14,
    KeyF15 = GLFW_KEY_F15,
    KeyF16 = GLFW_KEY_F16,
    KeyF17 = GLFW_KEY_F17,
    KeyF18 = GLFW_KEY_F18,
    KeyF19 = GLFW_KEY_F19,
    KeyF20 = GLFW_KEY_F20,
    KeyF21 = GLFW_KEY_F21,
    KeyF22 = GLFW_KEY_F22,
    KeyF23 = GLFW_KEY_F23,
    KeyF24 = GLFW_KEY_F24,
    KeyF25 = GLFW_KEY_F25,
    KeyKp0 = GLFW_KEY_KP_0,
    KeyKp1 = GLFW_KEY_KP_1,
    KeyKp2 = GLFW_KEY_KP_2,
    KeyKp3 = GLFW_KEY_KP_3,
    KeyKp4 = GLFW_KEY_KP_4,
    KeyKp5 = GLFW_KEY_KP_5,
    KeyKp6 = GLFW_KEY_KP_6,
    KeyKp7 = GLFW_KEY_KP_7,
    KeyKp8 = GLFW_KEY_KP_8,
    KeyKp9 = GLFW_KEY_KP_9,
    KeyKpDecimal = GLFW_KEY_KP_DECIMAL,
    KeyKpDivide = GLFW_KEY_KP_DIVIDE,
    KeyKpMultiply = GLFW_KEY_KP_MULTIPLY,
    KeyKpSubtract = GLFW_KEY_KP_SUBTRACT,
    KeyKpAdd = GLFW_KEY_KP_ADD,
    KeyKpEnter = GLFW_KEY_KP_ENTER,
    KeyKpEqual = GLFW_KEY_KP_EQUAL,
    KeyLeftShift = GLFW_KEY_LEFT_SHIFT,
    KeyLeftControl = GLFW_KEY_LEFT_CONTROL,
    KeyLeftAlt = GLFW_KEY_LEFT_ALT,
    KeyLeftSuper = GLFW_KEY_LEFT_SUPER,
    KeyRightShift = GLFW_KEY_RIGHT_SHIFT,
    KeyRightControl = GLFW_KEY_RIGHT_CONTROL,
    KeyRightAlt = GLFW_KEY_RIGHT_ALT,
    KeyRightSuper = GLFW_KEY_RIGHT_SUPER,
    KeyMenu = GLFW_KEY_MENU,

};

enum class KeyAction : int {
    Release = GLFW_RELEASE,
    Press = GLFW_PRESS,
    Repeat = GLFW_REPEAT,
};

enum Modifier : int {
    MOD_ALT = GLFW_MOD_ALT,
    MOD_CONTROL = GLFW_MOD_CONTROL,
    MOD_SHIFT = GLFW_MOD_SHIFT,
    MOD_SUPER = GLFW_MOD_SUPER,
    MOD_CAPS_LOCK = GLFW_MOD_CAPS_LOCK,
    MOD_NUM_LOCK = GLFW_MOD_NUM_LOCK,
};

struct KeyEvent {
    Key key;
    KeyAction action;
    Modifier modifiers;
};

enum class MouseButton : int {
    MouseButton1 = GLFW_MOUSE_BUTTON_1,
    MouseButton2 = GLFW_MOUSE_BUTTON_2,
    MouseButton3 = GLFW_MOUSE_BUTTON_3,
    MouseButton4 = GLFW_MOUSE_BUTTON_4,
    MouseButton5 = GLFW_MOUSE_BUTTON_5,
    MouseButton6 = GLFW_MOUSE_BUTTON_6,
    MouseButton7 = GLFW_MOUSE_BUTTON_7,
    MouseButton8 = GLFW_MOUSE_BUTTON_8,
    MouseButtonLast = GLFW_MOUSE_BUTTON_LAST,
    MouseButtonLeft = GLFW_MOUSE_BUTTON_LEFT,
    MouseButtonRight = GLFW_MOUSE_BUTTON_RIGHT,
    MouseButtonMiddle = GLFW_MOUSE_BUTTON_MIDDLE,
};

enum class MouseAction : int {
    Release = GLFW_RELEASE,
    Press = GLFW_PRESS,
    // Repeat = GLFW_REPEAT,
};

struct MouseEvent {
    MouseButton button;
    MouseAction action;
    Modifier modifiers;
};

class GLFWApplication {
public:
    GLFWApplication() = default;

    bool init(int width, int height)
    {
        glfwSetErrorCallback(glfw_error_callback);

        if (!glfwInit()) {
            return false;
        }

        if (!pre_init()) {
            return false;
        }

        m_window = glfwCreateWindow(width, height, m_name.c_str(), NULL, NULL);

        if (!m_window) {
            return false;
        }

        glfwMakeContextCurrent(m_window);

        glfwSetWindowUserPointer(m_window, this);

        glfwSetKeyCallback(m_window, glfw_key_callback);
        glfwSetScrollCallback(m_window, glfw_scroll_callback);
        glfwSetCursorPosCallback(m_window, glfw_cursor_callback);
        glfwSetWindowSizeCallback(m_window, glfw_window_size_callback);
        glfwSetMouseButtonCallback(m_window, glfw_mouse_button_callback);
        // glfwSetCursorEnterCallback();
        // glfwSetWindowCloseCallback();

        if (glewInit() != GLEW_OK) {
            return false;
        }

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(gl_error_callback, this);

        return post_init();
    }

    ~GLFWApplication() { glfwTerminate(); }

    ivec2 window_size()
    {
        ivec2 size;
        glfwGetWindowSize(m_window, &size.x, &size.y);
        return size;
    }

    dvec2 mouse_pos()
    {
        dvec2 mouse;
        glfwGetCursorPos(m_window, &mouse.x, &mouse.y);
        return mouse;
    }

protected:
    // runs just after glfwInit() and before the window is created
    // usefull to give advanced window hints to glfw.
    virtual bool pre_init() { return true; }

    // runs after every other initialization
    // opengl functions can be called.
    virtual bool post_init() { return true; }

    // main function of the application
    virtual void run() = 0;

    virtual void key_event(KeyEvent event) {}

    virtual void mouse_event(MouseEvent event) {}

    virtual void cursor_event(dvec2 pos) {}

    virtual void scroll_event(dvec2 off) {}

    virtual void resize_event(ivec2 size) {}

private:
    static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        GLFWApplication* this_ptr = static_cast<GLFWApplication*>(glfwGetWindowUserPointer(window));
        auto event = KeyEvent { static_cast<Key>(key), static_cast<KeyAction>(action), static_cast<Modifier>(mods) };
        this_ptr->key_event(event);
    }

    static void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        GLFWApplication* this_ptr = static_cast<GLFWApplication*>(glfwGetWindowUserPointer(window));
        this_ptr->scroll_event(dvec2 { xoffset, yoffset });
    }

    static void glfw_cursor_callback(GLFWwindow* window, double xpos, double ypos)
    {
        GLFWApplication* this_ptr = static_cast<GLFWApplication*>(glfwGetWindowUserPointer(window));
        this_ptr->cursor_event(dvec2 { xpos, ypos });
    }

    static void glfw_window_size_callback(GLFWwindow* window, int width, int height)
    {
        GLFWApplication* this_ptr = static_cast<GLFWApplication*>(glfwGetWindowUserPointer(window));
        this_ptr->resize_event(ivec2 { width, height });
    }

    static void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) 
    {
        GLFWApplication* this_ptr = static_cast<GLFWApplication*>(glfwGetWindowUserPointer(window));
        MouseEvent event = MouseEvent { static_cast<MouseButton>(button), static_cast<MouseAction>(action), static_cast<Modifier>(mods) };
        this_ptr->mouse_event(event);
    }

    static void glfw_error_callback(int error, const char* description)
    {
        fprintf(stderr, "GLFW Error: %s\n", description);
    }

    static void APIENTRY gl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
        const GLchar* message, const void* this_ptr)
    {
        fprintf(stderr, "OpenGL Info: %s\n", message);
    }

protected:
    std::string m_name { "GLFW App" };
    GLFWwindow* m_window { nullptr };
};

} // namespace mygl
