using CommunityToolkit.Mvvm.ComponentModel;
using System.Collections.ObjectModel;
using CommunityToolkit.Mvvm.Input;

namespace dashb.ViewModels;

public interface IShowDialog
{
    void ShowAddServer(InputVm inputVm);
    void ShowAddClient();
}

public interface IWnd
{
    void Close();
}

public partial class MainWindowVm : ViewModelBase
{
    private readonly IShowDialog _showDialog;

    public MainWindowVm(IShowDialog showDialog)
    {
        AddServerCmd = new(AddServer);
        _showDialog = showDialog;
    }

    [ObservableProperty]
    private ObservableCollection<NtripClientVm> _clients = [];

    [ObservableProperty]
    private ObservableCollection<NtripServerVm> _servers = [];

    [ObservableProperty]
    private RelayCommand _addServerCmd;

    private NtripServerVm? _serverToAdd;
    private void AddServer()
    {
        _serverToAdd = new();
        _inputVm = new InputVm()
        {
            User = _serverToAdd
        };
        _inputVm.User.PropertyChanged += _inputVm.User_PropertyChanged;
        _inputVm.OkCmd = new(AddServerFromInput, _inputVm.CanOkExecute);
        _showDialog.ShowAddServer(_inputVm);
    }

    private InputVm? _inputVm;
    private void AddServerFromInput()
    {
        _inputVm!.User!.PropertyChanged -= _inputVm.User_PropertyChanged;
        Servers.Add(_serverToAdd!);
        _inputVm?.Close();
    }
}
