using Avalonia;
using Avalonia.Controls;
using System;
using System.Linq;

namespace school_calc.Views;

public class CirclePanel : Panel
{
    protected override Size MeasureOverride(Size availableSize)
    {
        Size desiredSize = new(0, 0);
        foreach (var child in Children)
        {
            child.Measure(availableSize);
            desiredSize = new(desiredSize.Width + child.DesiredSize.Width,
                              desiredSize.Height + child.DesiredSize.Height);
        }
        return desiredSize;
    }

    protected override Size ArrangeOverride(Size finalSize)
    {
        Point center = new(finalSize.Width / 2, finalSize.Height / 2);
        double radius = Math.Min(center.X - Children.Max(c => c.DesiredSize.Width),
                                 center.Y - Children.Max(c => c.DesiredSize.Height));
        for (int i = 0; i < Children.Count; i++)
        {
            double angle = 2 * Math.PI * i / Children.Count;
            double x = center.X + radius * Math.Cos(angle) - Children[i].DesiredSize.Width / 2;
            double y = center.Y + radius * Math.Sin(angle) - Children[i].DesiredSize.Height / 2;
            Children[i].Arrange(new Rect(new Point(x, y), Children[i].DesiredSize));
        }
        return finalSize;
    }
}
