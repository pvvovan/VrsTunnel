using Avalonia.Controls;
using Avalonia.Data.Converters;
using Avalonia.Media;
using mul.ViewModels;
using System;
using System.Globalization;

namespace mul.Views;

public partial class MainWindow : Window
{
    public MainWindow()
    {
        InitializeComponent();
    }
}

public class MyBrushConverter : IValueConverter
{
    public object Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        Verdict verdict = (Verdict)value!;
        return verdict switch
        {
            Verdict.Fail => new SolidColorBrush(Colors.Red),
            Verdict.Pass => new SolidColorBrush(Colors.Green),
            _ => new SolidColorBrush(Colors.Gray)
        };
    }

    public object ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        return int.MinValue;
    }
}
