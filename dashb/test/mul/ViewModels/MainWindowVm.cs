using Avalonia.Data;
using Avalonia.Data.Converters;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using System;
using System.Collections.ObjectModel;
using System.Globalization;

namespace mul.ViewModels;

public enum Op
{
    Add,
    Sub,
    Mul,
    Div
}

public partial class MainWindowVm : ObservableObject
{
    [ObservableProperty]
    private ObservableCollection<OperationVm> _operations = [];

    public MainWindowVm()
    {
        for (int i = 0; i < 25; i++)
        {
            Operations.Add(new OperationVm());
        }
        CheckCmd = new(Check);
    }

    [ObservableProperty]
    private string _errorMsg = "Check";

    public RelayCommand CheckCmd { get; set; }

    private void Check()
    {
        int err_cnt = 0;
        foreach (var op in Operations)
        {
            if (op.Result == op.ExpectedResult)
            {
                op.Error = Verdict.Pass;
            }
            else if (op.Result == int.MinValue)
            {
                err_cnt++;
                op.Error = Verdict.None;
            }
            else
            {
                err_cnt++;
                op.Error = Verdict.Fail;
            }
        }

        ErrorMsg = $"Errors={err_cnt}";
    }
}

public enum Verdict
{
    None,
    Fail,
    Pass
}

public partial class OperationVm : ObservableObject
{
    [ObservableProperty]
    private int _left;

    [ObservableProperty]
    private int _right;

    [ObservableProperty]
    private int _result = int.MinValue;

    [ObservableProperty]
    private string _operation = string.Empty;

    private readonly Op _op;
    public int ExpectedResult;

    [ObservableProperty]
    private Verdict _error = Verdict.None;

    public OperationVm()
    {
        Random rnd = new();

        _op = (Op)rnd.Next(0, 2);
        Operation = _op switch
        {
            Op.Add => "+",
            Op.Sub => "-",
            Op.Mul => "\u00D7",
            Op.Div => "\u00F7",
            _ => string.Empty
        };

        switch (_op)
        {
            case Op.Add:
                Left = rnd.Next(200, 1000);
                Right = rnd.Next(200, 1000);
                ExpectedResult = Left + Right;
                break;

            case Op.Sub:
                Left = rnd.Next(200, 1000);
                Right = rnd.Next(200, 1000);
                if (Left < Right)
                {
                    (Left, Right) = (Right, Left);
                }
                ExpectedResult = Left - Right;
                break;

            case Op.Mul:
                Left = rnd.Next(2, 16);
                Right = rnd.Next(2, 16);
                ExpectedResult = Left * Right;
                break;

            case Op.Div:
                Left = rnd.Next(2, 16);
                Right = rnd.Next(2, 16);
                ExpectedResult = Left * Right;
                Left = ExpectedResult;
                ExpectedResult = Left / Right;
                break;
        }
    }
}


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
