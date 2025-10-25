using CommunityToolkit.Mvvm.ComponentModel;
using System.Collections.ObjectModel;
using CommunityToolkit.Mvvm.Input;

namespace dashb.ViewModels;

public interface IShowDialog
{
    InputVm ShowAddServer(NtripServerVm server, RelayCommand addServerCmd);
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
        _addServerFromInput = new(AddServerFromInput);
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
        _inputVm = _showDialog.ShowAddServer(_serverToAdd, _addServerFromInput);
    }

    private InputVm? _inputVm;
    private readonly RelayCommand _addServerFromInput;
    private void AddServerFromInput()
    {
        Servers.Add(_serverToAdd!);
        _inputVm?.Close();
    }
}
