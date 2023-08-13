#ifndef DS_GRAPHICS_H
#define DS_GRAPHICS_H

struct Point
{
    int x, y;
    Point() {}
    Point(int x, int y) : x(x), y(y) {}
    Point operator+(const Point &other) const
    {
        return Point(x + other.x, y + other.y);
    }
};

struct Rect
{
    int x, y, width, height;
    Rect() {}
    Rect(int x, int y, int width, int height) : x(x), y(y), width(width), height(height){};

    Point getCentre()
    {
        return Point(x + width / 2, y + height / 2);
    }

    int getCentreX() { return x + width / 2; }
    int getCentreY() { return y + height / 2; }

    Point position()
    {
        return Point(x, y);
    }
};

struct Line
{
    Point start;
    Point end;
    Line() {}
    Line(Point lineStart, Point lineEnd) : start(lineStart), end(lineEnd) {}
};

struct PolyLine
{
    uint8_t count;
    uint8_t lineCount;
    uint8_t index = 0;
    Point *points;
    PolyLine(uint8_t numPoints) : count(numPoints), lineCount(numPoints - 1)
    {
        points = new Point[numPoints];
    }

    void add(int x, int y)
    {        
        addAtIndex(index, x, y);
        index++;
        if (index > count - 1)
            index = 0;
    }

    void addAtIndex(uint8_t i, int x, int y)
    {
        if (i > count - 1 || i < 0)
            return;

        if (i == 0)
        {
            points[i].x = x;
            points[i].y = y;
        }
        else
        {
            points[i].x = x + points[i - 1].x;
            points[i].y = y + points[i - 1].y;
        }
    }

    Line lineAtIndex(uint8_t i)
    {        
        return Line(points[i], points[i + 1]);
    }

    void reset() {
        index = 0;
    }
};

void test () {    
}



#endif