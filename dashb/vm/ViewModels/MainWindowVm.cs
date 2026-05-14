using Avalonia.Threading;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using System.Collections.ObjectModel;
using vm.Models;

namespace vm.ViewModels;

public partial class MainWindowVm : ObservableObject
{
    private readonly Dispatcher? _dispatcher;
    private InputVm? _inputVm;
    private readonly IConfig _config;
    private readonly IDialog _dialog;
    private readonly Action<Task<(IQueryable<NtripClient> clients, IQueryable<NtripServer> servers)>> _loadAction;

    public async Task StoreConfig(string file)
    {
        var cls = Clients.Select(c => new NtripClient()
        {
            Name = c.Name,
            PasswordHash = c.PasswordHash,
            Id = c.Model.Id
        });
        var srs = Servers.Select(s => new NtripServer()
        {
            Name = s.Name,
            PasswordHash = s.PasswordHash,
            Id = s.Model is null ? Guid.NewGuid() : s.Model.Id,
            Clients = [..s.Clients.Select(c => c.Model.Id)]
        });
        await _config.StoreAsync(cls.AsQueryable(), srs.AsQueryable(), file);
    }

    public MainWindowVm(IDialog dialog, IConfig config, Dispatcher? dispatcher = null)
    {
        _dispatcher = dispatcher;
        _editClientCmd = new(EditClient);
        _editServerCmd = new(EditServer);
        _dialog = dialog;
        _config = config;
        Clients = [];
        Servers = [];

        _loadAction = cfg =>
        {
            var (cfgClients, cfgServers) = cfg.Result;
            _dispatcher?.Invoke(() =>
            {
                Clients = [.. cfgClients.Select(c => new NtripClientVm(c) { EditCmd = _editClientCmd })];
                Servers.Clear();
                foreach (NtripServer server in cfgServers)
                {
                    NtripServerVm serverVm = new(server)
                    {
                        EditCmd = _editServerCmd,
                    };
                    foreach (Guid clientId in server.Clients)
                    {
                        Clients.First(c => c.Model.Id == clientId).AssignCommand.Execute(serverVm);
                    }
                    Servers.Add(serverVm);
                }
            });
        };

        _config.LoadAsync().ContinueWith(_loadAction);
    }

    [ObservableProperty]
    public partial ObservableCollection<NtripServerVm> Servers { get; set; }

    private NtripServerVm? _serverToAdd;

    [RelayCommand]
    private void CreateServer()
    {
        _serverToAdd = new();
        _inputVm = new InputVm(_serverToAdd, _editServerCmd, AddServerFromInput);
        _dialog.Show(_inputVm);
    }

    private void AddServerFromInput()
    {
        Servers.Add(_serverToAdd!);
        _inputVm?.Close();
    }

    [ObservableProperty]
    public partial ObservableCollection<NtripClientVm> Clients { get; set; }

    private NtripClientVm? _clientToAdd;

    [RelayCommand]
    private void CreateClient()
    {
        _clientToAdd = new();
        _inputVm = new InputVm(_clientToAdd, _editClientCmd, AddClientFromInput);
        _dialog.Show(_inputVm);
    }

    private void AddClientFromInput()
    {
        Clients.Add(_clientToAdd!);
        _inputVm?.Close();
    }

    public object? SelectedNode
    {
        get => field;
        set
        {
            SetProperty(ref field, value);
            Clients.ToList().ForEach(c => c.CanAssign = SelectedNode is NtripServerVm);
        }
    }

    private NtripClientVm? _clientToEdit;
    private NtripClientVm? _editedClient;
    private readonly RelayCommand<UserVm> _editClientCmd;
    private void EditClient(object? param)
    {
        _clientToEdit = (NtripClientVm)param!;
        _editedClient = new()
        {
            Name = _clientToEdit.Name,
            PasswordHash = _clientToEdit.PasswordHash
        };
        _inputVm = new InputVm(_editedClient, _editClientCmd, EditClientFromInput);
        _dialog.Show(_inputVm);
    }

    private void EditClientFromInput()
    {
        _clientToEdit!.Name = _editedClient!.Name;
        _clientToEdit!.PasswordHash = _editedClient!.PasswordHash;
        _inputVm?.Close();
    }

    private NtripServerVm? _serverToEdit;
    private NtripServerVm? _editedServer;
    private readonly RelayCommand<UserVm> _editServerCmd;
    private void EditServer(object? param)
    {
        _serverToEdit = (NtripServerVm)param!;
        _editedServer = new()
        {
            Name = _serverToEdit.Name,
            PasswordHash = _serverToEdit.PasswordHash
        };
        _inputVm = new InputVm(_editedServer, _editServerCmd, EditServerFromInput);
        _dialog.Show(_inputVm);
    }

    private void EditServerFromInput()
    {
        _serverToEdit!.Name = _editedServer!.Name;
        _serverToEdit!.PasswordHash = _editedServer!.PasswordHash;
        _inputVm?.Close();
    }

    [RelayCommand]
    private async Task Save()
    {
        string file = await _dialog.Save();
        await StoreConfig(file);
    }

    [RelayCommand]
    private async Task Open()
    {
        string file = await _dialog.Open();
        await _config.LoadAsync(file).ContinueWith(_loadAction);
    }
}
