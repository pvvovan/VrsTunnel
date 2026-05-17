using Avalonia;
using Avalonia.Controls;
using System;
using System.Linq;

namespace school_calc.Views;

public class CirclePanel : Panel
{
    private static Rect Fit(Rect rect, Size area)
    {
        Point pos = rect.Position;
        if (pos.X < 0)
        {
            pos = new Point(0, pos.Y);
        }
        if (pos.Y < 0)
        {
            pos = new Point(pos.X, 0);
        }
        rect = new Rect(pos, new Point(pos.X + rect.Width, pos.Y + rect.Height));
        if (rect.BottomRight.X > area.Width)
        {
            pos = new Point(area.Width - rect.Width, pos.Y);
        }
        if (rect.BottomRight.Y > area.Height)
        {
            pos = new Point(pos.X, area.Height - rect.Height);
        }
        return new Rect(pos, new Point(pos.X + rect.Width, pos.Y + rect.Height));
    }

    protected override Size MeasureOverride(Size availableSize)
    {
        Size desiredSize = new(0, 0);
        foreach (var child in Children)
        {
            child.Measure(availableSize);
            desiredSize = new(desiredSize.Width + child.DesiredSize.Width / 2,
                              desiredSize.Height + child.DesiredSize.Height / 2);
        }
        return desiredSize;
    }

    protected override Size ArrangeOverride(Size finalSize)
    {
        Point center = new(finalSize.Width / 2 - Children[0].DesiredSize.Width / 2,
                           finalSize.Height / 2 - Children[0].DesiredSize.Height / 2);
        double radius = Math.Min(finalSize.Width - center.X - Children[0].DesiredSize.Width,
                                 finalSize.Height - center.Y - Children[0].DesiredSize.Height);
        for (int i = 0; i < Children.Count; i++)
        {
            double angle = 2 * Math.PI * i / Children.Count;
            double x = center.X + radius * Math.Cos(angle);
            double y = center.Y + radius * Math.Sin(angle);
            Children[i].Arrange(Fit(new Rect(new Point(x, y), Children[i].DesiredSize), finalSize));
        }
        return finalSize;
    }
}
