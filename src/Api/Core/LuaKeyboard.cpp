#include "LuaKeyboard.h"


LuaKeyboard::LuaKeyboard(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<SparkGlobals> globals)
{
    mSceneManager = sceneManager;
    mWindow = window;
    mGlobals = globals;
}

LuaKeyboard::~LuaKeyboard()
{

}

void LuaKeyboard::setup(sol::state& lua)
{
    /*
    * =========================================
    * Instantiate the keyboard table of utilities
    * =========================================
    */
    lua["spark"]["keyboard"] = lua.create_table();


    /*
    * =========================================
    * Key code bindings
    * TODO put this somewhere nicer
    * =========================================
    */

    // -- spark.keyboard.code.a 
    lua["spark"]["keyboard"]["code"] = lua.create_table_with(
        "a", 0,
        "b", 1,
        "c", 2,
        "d", 3,
        "e", 4,
        "f", 5,
        "g", 6,
        "h", 7,
        "i", 8,
        "j", 9,
        "k", 10,
        "l", 11,
        "m", 12,
        "n", 13,
        "o", 14,
        "p", 15,
        "q", 16,
        "r", 17,
        "s", 18,
        "t", 19,
        "u", 20,
        "v", 21,
        "w", 22,
        "x", 23,
        "y", 24,
        "z", 25,
        "num1", 26,
        "num2", 27,
        "num3", 28,
        "num4", 29,
        "num5", 30,
        "num6", 31,
        "num7", 32,
        "num8", 33,
        "num9", 34,
        "num0", 35,
        "enter", 36,
        "escape", 37,
        "backspace", 38,
        "tab", 39,
        "space", 40,
        "hyphen", 41,
        "equal", 42,
        "lbracket", 43,
        "rbracket", 44,
        "backslash", 45,
        "semicolon", 46,
        "apostrophe", 47,
        "grave", 48,
        "comma", 49,
        "period", 50,
        "slash", 51,
        "f1", 52,
        "f2", 53,
        "f3", 54,
        "f4", 55,
        "f5", 56,
        "f6", 57,
        "f7", 58,
        "f8", 59,
        "f9", 60,
        "f10", 61,
        "f11", 62,
        "f12", 63,
        "f13", 64,
        "f14", 65,
        "f15", 66,
        "f16", 67,
        "f17", 68,
        "f18", 69,
        "f19", 70,
        "f20", 71,
        "f21", 72,
        "f22", 73,
        "f23", 74,
        "f24", 75,
        "capslock", 76,
        "printscreen", 77,
        "scrolllock", 78,
        "pause", 79,
        "insert", 80,
        "home", 81,
        "pageup", 82,
        "delete", 83,
        "end", 84,
        "pagedown", 85,
        "right", 86,
        "left", 87,
        "down", 88,
        "up", 89,
        "numlock", 90,
        "numpaddivide", 91,
        "numpadmultiply", 92,
        "numpadminus", 93,
        "numpadplus", 94,
        "numpadequal", 95,
        "numpadenter", 96,
        "numpaddecimal", 97,
        "numpad1", 98,
        "numpad2", 99,
        "numpad3", 100,
        "numpad4", 101,
        "numpad5", 102,
        "numpad6", 103,
        "numpad7", 104,
        "numpad8", 105,
        "numpad9", 106,
        "numpad0", 107,
        "lcontrol", 127,
        "lshift", 128,
        "lalt", 129,
        "lsystem", 130,
        "rcontrol", 131,
        "rshift", 132,
        "ralt", 133,
        "rsystem", 134
    );


    /*
    * =========================================
    * Active check to see if a particular key is being pressed
    * =========================================
    */

    // -- spark.keyboard:down(3)
    lua["spark"]["keyboard"]["down"] = [this](sol::table keyboard, int code) {
        return (std::find(keysDown.begin(), keysDown.end(), code) != keysDown.end());
    };
}

void LuaKeyboard::keyDown(sol::state& lua, sf::Keyboard::Scancode code)
{
    lua["spark"]["keypressed"](sol::nil, code);
}

void LuaKeyboard::keyUp(sol::state&lua, sf::Keyboard::Scancode code)
{
    lua["spark"]["keyreleased"](sol::nil, code);
}