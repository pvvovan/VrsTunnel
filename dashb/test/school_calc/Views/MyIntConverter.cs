using Avalonia.Data.Converters;
using System;
using System.Globalization;

namespace school_calc.Views;

public class MyIntConverter : IValueConverter
{
    public object Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        int num = (int)value!;
        return num != int.MinValue ? num.ToString() : string.Empty;
    }

    public object ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        string? strValue = value as string;
        if (Int32.TryParse(strValue, out int num))
        {
            return num;
        }
        return int.MinValue;
    }
}
