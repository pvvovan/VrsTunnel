using CommunityToolkit.Mvvm.ComponentModel;
using System.Collections.ObjectModel;
using CommunityToolkit.Mvvm.Input;
using System.ComponentModel;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System;

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
    private readonly Models.IConfig _config;
    public async Task StoreConfig()
    {
        var oldClients = _config.Load().clients;
        List<Models.NtripClient> cls = [];
        foreach (var cl in Clients)
        {
            var oldCl = (from oc in oldClients
                        where (cl.Model != null && oc.Id == cl.Model.Id)
                        select oc).FirstOrDefault();
            cls.Add(new Models.NtripClient()
            {
                Name = cl.Name,
                Password = cl.Password,
                Id = oldCl is not null ? oldCl.Id : Guid.NewGuid()
            });
        }

        List<Models.NtripServer> srs = [];
        foreach (var sv in Servers)
        {
            srs.Add(new Models.NtripServer()
            {
                Name = sv.Name,
                Password = sv.Password,
                Id = Guid.NewGuid()
            });
        }
        await _config.Store(cls.AsQueryable(), srs.AsQueryable());
    }



    private readonly IDialog _dialog;

    public MainWindowVm(IDialog dialog, Models.IConfig config)
    {
        AddServerCmd = new(AddServer);
        AddClientCmd = new(AddClient);
        _editClientCmd = new(EditClient);
        _editServerCmd = new(EditServer);
        _dialog = dialog;
        _config = config;

        Clients = [];
        foreach (var cl in _config.Load().clients)
        {
            NtripClientVm clVm = new(new(RemoveClient), new(AssignClient), new(UnassignClient), cl);
            clVm.EditCmd = _editClientCmd;
            Clients.Add(clVm);
        }
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
        _inputVm.User.EditCmd = _editServerCmd;
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
            foreach (var srv in Servers)
            {
                while (srv.Clients.Contains(client))
                {
                    srv.Clients.Remove(client);
                }
            }
        }
    }

    [ObservableProperty]
    private RelayCommand _addClientCmd;

    private NtripClientVm? _clientToAdd;
    private void AddClient()
    {
        _clientToAdd = new(new(RemoveClient), new(AssignClient), new(UnassignClient), null);
        _inputVm = new InputVm()
        {
            User = _clientToAdd
        };
        _inputVm.User.EditCmd = _editClientCmd;
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

    private ObservableCollection<NtripClientVm> _assignedClients = [];
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
            if (SelectedServer?.Clients.Contains(client) == false)
            {
                SelectedServer?.Clients.Add(client);
            }
        }
    }

    private void UnassignClient(object? param)
    {
        if (param is not null && param is NtripClientVm client)
        {
            SelectedServer?.Clients.Remove(client);
        }
    }

    private NtripClientVm? _clientToEdit;
    private NtripClientVm? _editedClient;
    private readonly RelayCommand<UserVm> _editClientCmd;
    private void EditClient(object? param)
    {
        _clientToEdit = (NtripClientVm)param!;
        _editedClient = new(new(RemoveClient), new(AssignClient), new(UnassignClient), null)
        {
            Name = _clientToEdit.Name,
            Password = _clientToEdit.Password
        };
        _inputVm = new InputVm()
        {
            User = _editedClient
        };
        _inputVm.User.EditCmd = _editClientCmd;
        _inputVm.User.PropertyChanged += _inputVm.User_PropertyChanged;
        _inputVm.OkCmd = new(EditClientFromInput, _inputVm.CanOkExecute);
        _dialog.Show(_inputVm);
    }

    private void EditClientFromInput()
    {
        _clientToEdit!.Name = _editedClient!.Name;
        _clientToEdit!.Password = _editedClient!.Password;
        _inputVm!.User!.PropertyChanged -= _inputVm.User_PropertyChanged;
        _inputVm?.Close();
    }


    private NtripServerVm? _serverToEdit;
    private NtripServerVm? _editedServer;
    private readonly RelayCommand<UserVm> _editServerCmd;
    private void EditServer(object? param)
    {
        _serverToEdit = (NtripServerVm)param!;
        _editedServer = new(new(RemoveServer))
        {
            Name = _serverToEdit.Name,
            Password = _serverToEdit.Password
        };
        _inputVm = new InputVm()
        {
            User = _editedServer
        };
        _inputVm.User.EditCmd = _editServerCmd;
        _inputVm.User.PropertyChanged += _inputVm.User_PropertyChanged;
        _inputVm.OkCmd = new(EditServerFromInput, _inputVm.CanOkExecute);
        _dialog.Show(_inputVm);
    }

    private void EditServerFromInput()
    {
        _serverToEdit!.Name = _editedServer!.Name;
        _serverToEdit!.Password = _editedServer!.Password;
        _inputVm!.User!.PropertyChanged -= _inputVm.User_PropertyChanged;
        _inputVm?.Close();
    }

    public new event PropertyChangedEventHandler? PropertyChanged = delegate { };
}
