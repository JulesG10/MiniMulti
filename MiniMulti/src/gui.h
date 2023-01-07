#pragma once
#include "stdafx.h"

class Text
{
public:
    Text(std::string, float, Vector2);
    ~Text();

    void draw(Color);

    char *get_text();

    Text *set_text(std::string);
    Text *set_position(Vector2);
    Text *set_size(float);
    Text *set_center(bool, bool);

private:
    void update();

    Vector2 m_position;
    Vector2 m_rndposition;
    std::string m_text;
    float m_size;
    bool m_cx, m_cy;
};

class Button
{
public:
    Button();
    ~Button();

    void draw(Color, Color);
    bool is_hover(Vector2);

    Button *set_position(Vector2);
    Button *set_text(std::string);
    Button *set_size(float);
    Button *set_margin(int);
    Button *set_center(bool, bool);

private:
    void update();

    float m_fontsize = 20;
    int m_margin = 5;

    bool m_cx, m_cy;
    Text *m_text;
    Rectangle m_rect;
    Vector2 m_position;
};

class Loader
{
public:
    Loader(float, Rectangle);
    ~Loader();

    void draw(Color, Color);
    Loader *update(float);

    float m_value;

private:
    Rectangle m_rect;
    Rectangle m_inner;
};

class Keyframe
{
public:
    Keyframe(float from, float to, float duration);
    ~Keyframe();

    float update();
    float get_value();
    void reset(float from, float to, float duration);
    bool is_finished();

private:
    float m_from;
    float m_to;

    float m_time;
    float m_duration;

    float m_factor;
    float m_value;
};

class Utility
{
public:
    Utility();
    ~Utility();

    static Utility *Instance();

    void set_rendersize(Vector2);
    Vector2 mouse();
    Vector2 center(Rectangle);
    Vector2 percent(Vector2);
    Vector2 screen_midle();

    Vector2 render_size();
    Vector2 window_size();
     
    std::unordered_map<std::string, std::string> parse_args(int argc, char** argv);
    int stop_char(std::string, char);
private:
    Vector2 m_render;
    Vector2 m_window;
};

#define $util Utility::Instance()