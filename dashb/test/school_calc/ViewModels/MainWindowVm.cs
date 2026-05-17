using System.Collections.ObjectModel;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using school_calc.Models;

namespace school_calc.ViewModels;

public partial class MainWindowVm : ViewModelBase
{
    [RelayCommand]
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

    [ObservableProperty]
    public partial ObservableCollection<OperationVm> Operations { get; set; } = [];

    [ObservableProperty]
    public partial string ErrorMsg { get; set; } = "Check";

    public MainWindowVm()
    {
        for (int i = 0; i < 24; i++)
        {
            Operations.Add(new OperationVm());
        }
    }
}
