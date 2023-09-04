#ifndef DS_BOUNDS_H
#define DS_BOUNDS_H

#include "DSGraphics.h"

enum DockPosH
{
    left,
    centre,
    right,
    noneH
};
enum DockPosV
{
    top,
    middle,
    bottom,
    noneV
};

class DSBounds
{
public:
    Rect boundsRect;

    DSBounds() { boundsRect = Rect(0, 0, 1, 1); }
    DSBounds(Rect boundsRect, DSBounds *coordinateSpace) { setBounds(boundsRect, coordinateSpace); }

    void setBounds(int x, int y, int w, int h) { setBounds(Rect(x, y, w, h)); }
    void setBounds(int x, int y, int w, int h, DSBounds *parent) { setBounds(Rect(x, y, w, h), parent); }
    void setBounds(Rect bounds) { this->boundsRect = bounds; }
    void setBounds(Rect bounds, DSBounds *parent)
    {
        this->boundsRect = bounds;
        coordinateSpace = parent;
    }
    void attachToSpace(DSBounds *parent) { coordinateSpace = parent; }
    Rect getBounds() { return this->boundsRect; }

    void translateBounds(Point p) { translateBounds(p.x, p.y); }
    void translateBounds(int x, int y)
    {
        boundsRect.x += x;
        boundsRect.y += y;
    }

    void setBoundsPosition(Point p) { setBoundsPosition(p.x, p.y); }
    void setBoundsPosition(int x, int y)
    {
        boundsRect.x = x;
        boundsRect.y = y;
    }
    Point getBoundsPosition() { return Point(boundsRect.x, boundsRect.y); }
    Point getCentre() { return Point(boundsRect.x + boundsRect.width / 2, boundsRect.y + boundsRect.height / 2); }

    void dock(DockPosH dpx, DockPosV dpy, uint8_t marginH = 0)
    {
        dockPosH = dpx;
        dockPosV = dpy;
        this->marginH = marginH;
    }

    Rect dockedBounds()
    {
        Rect r = getBoundsAsCoordinateSpace();
        if (coordinateSpace != nullptr)
        {
            if (dockPosH == DockPosH::left)
                r.x += marginH;
            else if (dockPosH == DockPosH::centre)
                r.x += coordinateSpace->boundsRect.width / 2 - boundsRect.width / 2;
            else if (dockPosH == DockPosH::right)
                r.x += coordinateSpace->boundsRect.width - boundsRect.width - marginH;

            if (dockPosV == DockPosV::top)
                r.y += 0;
            else if (dockPosV == DockPosV::middle)
                r.y += coordinateSpace->boundsRect.height / 2 - boundsRect.height / 2;
            else if (dockPosV == DockPosV::bottom)
                r.y += coordinateSpace->boundsRect.height - boundsRect.height;
        }
        return r;
    }

private:
    DSBounds *coordinateSpace = nullptr;
    DockPosH dockPosH = noneH;
    DockPosV dockPosV = noneV;
    uint8_t marginH = 0;

    Rect getBoundsAsCoordinateSpace()
    {
        Rect b = boundsRect;
        if (coordinateSpace != nullptr)
        {
            Rect dockedBounds = coordinateSpace->dockedBounds();
            b = Rect(
                boundsRect.x + dockedBounds.x, 
                boundsRect.y + dockedBounds.y, 
                boundsRect.width, 
                boundsRect.height);
        }

        return b;
    }
};

#endif