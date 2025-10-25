using CommunityToolkit.Mvvm.ComponentModel;
using System.Collections.ObjectModel;
using CommunityToolkit.Mvvm.Input;
using System.ComponentModel;

namespace dashb.ViewModels;

public interface IDialog
{
    void Show(InputVm inputVm);
}

public interface IWnd
{
    void Close();
}

public partial class MainWindowVm : ViewModelBase, INotifyPropertyChanged
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

    private void RemoveClient(object? param)
    {
        if (param is not null && param is NtripClientVm client)
        {
            Clients.Remove(client);
        }
    }

    [ObservableProperty]
    private RelayCommand _addClientCmd;

    private NtripClientVm? _clientToAdd;
    private void AddClient()
    {
        _clientToAdd = new(new(RemoveClient), new(AssignClient), new(UnassignClient));
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


    private NtripServerVm? _selectedServer;
    public NtripServerVm SelectedServer
    {
        get
        {
            return _selectedServer!;
        }
        set
        {
            _selectedServer = value;
            PropertyChanged!(this, new PropertyChangedEventArgs(nameof(SelectedServer)));
            AssignedClients = _selectedServer?.Clients ?? [];
        }
    }

    private ObservableCollection<NtripClientVm> _assignedClients;
    public ObservableCollection<NtripClientVm> AssignedClients
    {
        get
        {
            return _assignedClients;
        }
        set
        {
            _assignedClients = value;
            PropertyChanged!(this, new PropertyChangedEventArgs(nameof(AssignedClients)));
        }
    }

    private void AssignClient(object? param)
    {
        if (param is not null && param is NtripClientVm client)
        {
            SelectedServer?.Clients.Add(client);
        }
    }

    private void UnassignClient(object? param)
    {
        if (param is not null && param is NtripClientVm client)
        {
            SelectedServer?.Clients.Remove(client);
        }
    }

    public new event System.ComponentModel.PropertyChangedEventHandler? PropertyChanged = delegate { };
}
