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
        var (oldClients, oldServers) = await _config.LoadAsync().ConfigureAwait(false);
        var newClients = new Dictionary<NtripClientVm, Guid>();

        List<Models.NtripClient> cls = [];
        foreach (var cl in Clients)
        {
            var oldCl = (from oc in oldClients
                         where cl.Model != null && oc.Id == cl.Model.Id
                         select oc).FirstOrDefault();
            Guid clId;
            if (oldCl is null)
            {
                clId = Guid.NewGuid();
                newClients.Add(cl, clId);
            }
            else
            {
                clId = oldCl.Id;
            }
            cls.Add(new Models.NtripClient(cl.Name, cl.Password, clId));
        }

        List<Models.NtripServer> srs = [];
        foreach (var sv in Servers)
        {
            var oldSv = (from os in oldServers
                         where sv.Model != null && os.Id == sv.Model.Id
                         select os).FirstOrDefault();
            List<Guid> clIds = [];
            foreach (var assignCl in sv.Clients)
            {
                if (assignCl.Model is not null)
                {
                    clIds.Add(assignCl.Model.Id);
                }
                else
                {
                    clIds.Add(newClients[assignCl]);
                }
            }
            srs.Add(new Models.NtripServer(sv.Name,
                                           sv.Password,
                                           oldSv is not null ? oldSv.Id : Guid.NewGuid(),
                                           clIds));
        }

        await _config.StoreAsync(cls.AsQueryable(), srs.AsQueryable()).ConfigureAwait(false);
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
        Servers = [];

        var loadResult = _config.LoadAsync();
        loadResult.ConfigureAwait(true);
        loadResult.ContinueWith(cfg =>
        {
            var (cfgClients, cfgServers) = cfg.Result;

            foreach (var cl in cfgClients)
            {
                NtripClientVm clVm = new(new(RemoveClient), new(AssignClient), new(UnassignClient), cl)
                {
                    EditCmd = _editClientCmd
                };
                Clients.Add(clVm);
            }

            foreach (var sv in cfgServers)
            {
                NtripServerVm svVm = new(new(RemoveServer), sv)
                {
                    EditCmd = _editServerCmd
                };
                if (sv.Clients is not null)
                {
                    foreach (var assignedCl in sv.Clients)
                    {
                        svVm.Clients.Add((from cl in Clients
                                          where cl.Model is not null && cl.Model.Id == assignedCl
                                          select cl).First());
                    }
                }
                Servers.Add(svVm);
            }
        }).ConfigureAwait(true);
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
        _serverToAdd = new(new(RemoveServer), null);
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
        _editedServer = new(new(RemoveServer), null)
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
