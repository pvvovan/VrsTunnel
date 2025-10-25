using CommunityToolkit.Mvvm.ComponentModel;
using System.Collections.ObjectModel;
using CommunityToolkit.Mvvm.Input;

namespace dashb.ViewModels;

public interface IDialog
{
    void Show(InputVm inputVm);
}

public interface IWnd
{
    void Close();
}

public partial class MainWindowVm : ViewModelBase
{
    private readonly IDialog _dialog;

    public MainWindowVm(IDialog dialog)
    {
        AddServerCmd = new(AddServer);
        AddClientCmd = new(AddClient);
        _dialog = dialog;
    }


    private InputVm? _inputVm;


    [ObservableProperty]
    private ObservableCollection<NtripServerVm> _servers = [];

    private void RemoveServer(object? param)
    {
        if (param is not null && param is NtripServerVm server)
        {
            Servers.Remove(server);
        }
    }

    [ObservableProperty]
    private RelayCommand _addServerCmd;

    private NtripServerVm? _serverToAdd;
    private void AddServer()
    {
        _serverToAdd = new(new(RemoveServer));
        _inputVm = new InputVm()
        {
            User = _serverToAdd
        };
        _inputVm.User.PropertyChanged += _inputVm.User_PropertyChanged;
        _inputVm.OkCmd = new(AddServerFromInput, _inputVm.CanOkExecute);
        _dialog.Show(_inputVm);
    }

    private void AddServerFromInput()
    {
        _inputVm!.User!.PropertyChanged -= _inputVm.User_PropertyChanged;
        Servers.Add(_serverToAdd!);
        _inputVm?.Close();
    }


    [ObservableProperty]
    private ObservableCollection<NtripClientVm> _clients = [];

    [ObservableProperty]
    private RelayCommand _addClientCmd;

    private NtripClientVm? _clientToAdd;
    private void AddClient()
    {
        _clientToAdd = new();
        _inputVm = new InputVm()
        {
            User = _clientToAdd
        };
        _inputVm.User.PropertyChanged += _inputVm.User_PropertyChanged;
        _inputVm.OkCmd = new(AddClientFromInput, _inputVm.CanOkExecute);
        _dialog.Show(_inputVm);
    }

    private void AddClientFromInput()
    {
        _inputVm!.User!.PropertyChanged -= _inputVm.User_PropertyChanged;
        Clients.Add(_clientToAdd!);
        _inputVm?.Close();
    }
}
