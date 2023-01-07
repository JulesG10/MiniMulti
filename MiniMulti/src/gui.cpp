#include "gui.h"

Text::Text(std::string text, float size, Vector2 pos)
{
    this->m_text = text;
    this->m_size = size;
    this->m_position = pos;
    this->m_cx = false;
    this->m_cy = false;
    this->update();
}

Text::~Text()
{
}

void Text::draw(Color color)
{
    DrawText(this->m_text.c_str(), this->m_rndposition.x, this->m_rndposition.y, this->m_size, color);
}

void Text::update()
{
    if (this->m_cx)
    {
        this->m_rndposition.x = this->m_position.x - (MeasureText(this->m_text.c_str(), this->m_size) / 2.f);
    }
    else
    {
        this->m_rndposition.x = this->m_position.x;
    }

    if (this->m_cy)
    {
        this->m_rndposition.y = this->m_position.y - (this->m_size / 2.f);
    }
    else
    {
        this->m_rndposition.y = this->m_position.y;
    }
}

Text *Text::set_text(std::string text)
{
    this->m_text = text;
    this->update();

    return this;
}

Text *Text::set_position(Vector2 position)
{
    this->m_position = position;
    this->update();

    return this;
}

Text *Text::set_size(float size)
{
    this->m_size = size;
    this->update();

    return this;
}

Text *Text::set_center(bool x, bool y)
{
    this->m_cx = x;
    this->m_cy = y;
    this->update();

    return this;
}

char *Text::get_text()
{
    return (char *)this->m_text.c_str();
}

Button::Button()
{
    this->m_text = new Text("", this->m_fontsize, {0, 0});
    this->update();
}

Button::~Button()
{
}

void Button::draw(Color fg, Color bg)
{
    DrawRectangleRec(this->m_rect, bg);
    this->m_text->draw(fg);
    DrawRectangleLinesEx(this->m_rect, 1.f, fg);
}

bool Button::is_hover(Vector2 mouse)
{
    return CheckCollisionPointRec(mouse, this->m_rect);
}

Button *Button::set_size(float size)
{
    this->m_fontsize = size;
    this->m_text->set_size(size);
    this->update();

    return this;
}

Button *Button::set_margin(int margin)
{
    this->m_margin = margin;
    this->update();

    return this;
}

Button *Button::set_center(bool cx, bool cy)
{
    this->m_cx = cx;
    this->m_cy = cy;
    this->update();
    return this;
}

Button *Button::set_text(std::string text)
{
    this->m_text->set_text(text);
    this->update();

    return this;
}

void Button::update()
{
    float twidth = MeasureText(this->m_text->get_text(), this->m_fontsize);
    this->m_rect = {
        this->m_position.x,
        this->m_position.y,
        twidth + this->m_margin * 2,
        this->m_fontsize + this->m_margin * 2};

    if (this->m_cx)
    {
        this->m_rect.x -= this->m_rect.width / 2.f;
    }
    if (this->m_cy)
    {
        this->m_rect.y -= this->m_rect.height / 2.f;
    }
    this->m_text->set_position({this->m_rect.x + this->m_margin, this->m_rect.y + this->m_margin});
}

Button *Button::set_position(Vector2 pos)
{
    this->m_position = pos;
    this->m_text->set_position(pos);
    this->update();
    return this;
}

Keyframe::Keyframe(float from, float to, float duration_s)
{
    this->m_from = from;
    this->m_to = to;
    this->m_duration = duration_s;
    this->m_value = from;
    this->m_time = 0.f;

    this->m_factor = this->m_from < this->m_to ? -1 : 1;
}

Keyframe::~Keyframe()
{
}

float Keyframe::update()
{
    if (this->m_time < this->m_duration && this->m_value < this->m_to)
    {
        this->m_time += GetFrameTime();
        this->m_value += ((this->m_time * this->m_to) / this->m_duration) * this->m_factor;
        if (this->m_value > this->m_to)
        {
            this->m_value = this->m_to;
        }
    }
    return this->m_value;
}

float Keyframe::get_value()
{
    return this->m_value;
}

void Keyframe::reset(float from, float to, float duration)
{
    this->m_from = from;
    this->m_to = to;
    this->m_duration = duration;
    this->m_value = from;
    this->m_time = 0.f;

    this->m_factor = this->m_from < this->m_to ? -1 : 1;
}

bool Keyframe::is_finished()
{
    return (this->m_time >= this->m_duration) || (this->m_value >= this->m_to);
}

Utility::Utility()
{
}

Utility::~Utility()
{
}

Utility *Utility::Instance()
{
    static Utility *util = new Utility();
    return util;
}

void Utility::set_rendersize(Vector2 render)
{
    this->m_render = render;
    this->m_window = {
        (float)GetRenderWidth(),
        (float)GetRenderHeight(),
    };
}

Vector2 Utility::mouse()
{
    return Vector2Divide(Vector2Multiply(this->m_render, GetMousePosition()), this->m_window);
}

Vector2 Utility::center(Rectangle obj)
{
    Vector2 size = {obj.width, obj.height};
    Vector2 pos = {obj.x, obj.y};
    return Vector2Add(pos, Vector2Divide(Vector2Subtract(this->m_render, size), {2.f, 2.f}));
}

Vector2 Utility::percent(Vector2 per)
{
    return Vector2Divide(Vector2Multiply(per, this->m_render), {100.f, 100.f});
}

Vector2 Utility::screen_midle()
{
    return Vector2Divide(this->m_window, { 2.f, 2.f });
}

Vector2 Utility::render_size()
{
    return this->m_render;
}

Vector2 Utility::window_size()
{
    return this->m_window;
}

std::unordered_map<std::string, std::string> Utility::parse_args(int argc, char **argv)
{
    std::unordered_map<std::string, std::string> map;

    for (size_t i = 0; i < argc; i++)
    {
        std::string arg(argv[i]);
        std::string param = "", value = "";
        bool change = false;
        for (size_t i = 0; i < arg.length(); i++)
        {
            if(arg[i] == '=')
            {
                change = true;
            }else if(change)
            {
                value += arg[i];
            }else{
                param += arg[i];
            }
        }
        map[param] = value;
    }

    return map;
}

int Utility::stop_char(std::string str, char c)
{
    int i = 0;
    while (i < str.length())
    {
        if (str[i] == c)
        {
            return i;
        }
        i++;
    }
    return i;
}

Loader::Loader(float start, Rectangle rect)
{
    this->m_value = start;
    this->m_rect = rect;
    this->m_inner = rect;

    this->update(0);
}

Loader::~Loader()
{
}

void Loader::draw(Color border, Color loader)
{
    DrawRectangleRec(this->m_inner, loader);
    DrawRectangleLinesEx(this->m_rect, 1, border);
}

Loader *Loader::update(float value)
{
    this->m_value += value;
    this->m_inner.width = (this->m_rect.width * this->m_value) / 100.f;
    return this;
}
