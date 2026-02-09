#include "Pipe.h"

Pipe::Pipe(const Vector2& startPos)
    : super("P", startPos, 2, 2)
{
    SetColor(KhyMario::Color::Green);
}

Pipe::Pipe(const Vector2& startPos, int w, int h)
    : super("P", startPos, w, h)
{
    SetColor(KhyMario::Color::Green);
}
