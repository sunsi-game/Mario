#pragma once

namespace KhyMario
{
    namespace MathUtil
    {
        inline bool AABBOverlap(
            float ax, float ay, float aw, float ah,
            float bx, float by, float bw, float bh)
        {
            return (ax < bx + bw) && (ax + aw > bx) &&
                (ay < by + bh) && (ay + ah > by);
        }
    }
}