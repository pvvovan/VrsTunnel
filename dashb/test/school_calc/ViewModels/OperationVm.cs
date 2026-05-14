using System;
using CommunityToolkit.Mvvm.ComponentModel;
using school_calc.Models;

namespace school_calc.ViewModels;

public partial class OperationVm : ViewModelBase
{
    private readonly Op _op;

    public int ExpectedResult;

    [ObservableProperty]
    public partial int Left { get; set; }

    [ObservableProperty]
    public partial int Right { get; set; }

    [ObservableProperty]
    public partial int Result { get; set; } = int.MinValue;

    [ObservableProperty]
    public partial string Operation { get; set; } = string.Empty;

    [ObservableProperty]
    public partial Verdict Error { get; set; } = Verdict.None;

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
