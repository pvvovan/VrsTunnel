using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;

namespace vm.ViewModels;

public partial class InputVm : ObservableObject
{
    public IWnd? Wnd;

    public void User_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
    {
        _CanOkExecute = User is not null && User.Name.Length > 0;
        OkCmd?.NotifyCanExecuteChanged();
    }

    public void Close()
    {
        Wnd?.Close();
    }

    [ObservableProperty]
    public partial UserVm? User { get; set; }

    [ObservableProperty]
    public partial RelayCommand? OkCmd { get; set; }

    private bool _CanOkExecute = false;
    public bool CanOkExecute()
    {
        return _CanOkExecute;
    }
}
