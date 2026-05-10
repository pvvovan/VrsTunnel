//using System.Collections.ObjectModel;
//using System.ComponentModel;
//using CommunityToolkit.Mvvm.Input;
//using vm.Models;
//using vm.ViewModels;

//namespace test.TestVm;

//#pragma warning disable CS9113 // Parameter exclusiveJsonConfig is unread.
//public class UnitTestVm(ExclusiveJsonConfig exclusiveJsonConfig) : IClassFixture<ExclusiveJsonConfig>
//#pragma warning restore CS9113 // Parameter exclusiveJsonConfig is unread.
//{
//    [Fact]
//    public void Test_NtripServerVm()
//    {
//        NtripServer server = new("Server 1", "Pw1", Guid.NewGuid(), null);
//        RelayCommand<NtripServerVm> stubCmd = new(p => { });
//        NtripServerVm serverVm = new(server);
//        Assert.Equal(serverVm.Name, server.Name);
//    }

//    [Fact]
//    public void Test_NtripClientVm()
//    {
//        NtripClient client = new("Client 1", "Pw1", Guid.NewGuid());
//        RelayCommand<NtripClientVm> stubCmd = new(p => { });
//        NtripClientVm clientVm = new(client);
//        Assert.Equal(clientVm.Name, client.Name);
//    }

//    [Fact]
//    public void Test_InputVm()
//    {
//        InputVm inputVm = new(new NtripServerVm(new NtripServer()), new RelayCommand<UserVm>((_)=>{}), ()=>{});
//        inputVm.Close();
//        inputVm.User_PropertyChanged(null, new PropertyChangedEventArgs(""));
//        Assert.False(inputVm.CanOkExecute());

//        var wnd = new WndMock()
//        {
//            CloseCalled = false
//        };
//        inputVm.Wnd = wnd;
//        inputVm.Close();
//        Assert.True(wnd.CloseCalled);
//    }

//    [Fact]
//    public async Task Test_MainWindowVm()
//    {
//        DialogStub dialogStub = new();
//        MainWindowVm mainVm = new(dialogStub, new vm.DAL.JsonConfig());
//        await Task.Delay(500);
//        mainVm.Servers.Clear();
//        mainVm.Clients.Clear();

//        dialogStub.InputName = "Client0";
//        mainVm.AddClientCommand.Execute(null);
//        await Task.Delay(100);

//        Assert.Single(mainVm.Clients);
//        Assert.Equal("Client0", mainVm.Clients[0].Name);
//        mainVm.Clients[0].RemoveCommand.Execute(new ReadOnlyCollection<object>([mainVm.Servers, mainVm.Clients]));

//        dialogStub.InputName = "inputName";
//        mainVm.AddServerCommand.Execute(null);
//        Assert.Equal("inputName", mainVm.Servers[0].Name);

//        mainVm.AddClientCommand.Execute(null);
//        Assert.Equal("inputName", mainVm.Clients[0].Name);

//        dialogStub.InputName = "Client1";
//        mainVm.AddClientCommand.Execute(null);
//        Assert.Equal("Client1", mainVm.Clients[1].Name);

//        mainVm.SelectedServer = mainVm.Servers[0];
//        mainVm.Clients[0].AssignCommand.Execute(mainVm.SelectedServer);
//        Assert.Single(mainVm.AssignedClients);
//        mainVm.Clients[0].UnassignCommand.Execute(mainVm.SelectedServer);
//        Assert.Empty(mainVm.AssignedClients);

//        mainVm.Clients[1].AssignCommand.Execute(mainVm.SelectedServer);
//        Assert.Single(mainVm.AssignedClients);
//        mainVm.Clients[1].RemoveCommand.Execute(new ReadOnlyCollection<object>([mainVm.Servers, mainVm.Clients]));
//        Assert.Empty(mainVm.AssignedClients);

//        dialogStub.InputName = "Server2";
//        mainVm.Servers[0].EditCmd?.Execute(mainVm.Servers[0]);
//        Assert.Equal("Server2", mainVm.Servers[0].Name);

//        dialogStub.InputName = "Client2";
//        mainVm.Clients[0].EditCmd?.Execute(mainVm.Clients[0]);
//        Assert.Equal("Client2", mainVm.Clients[0].Name);
//        mainVm.Clients[0].AssignCommand.Execute(mainVm.SelectedServer);
//        Assert.Equal("Client2", mainVm.AssignedClients[0].Name);

//        await mainVm.StoreConfig();
//        await Task.Delay(100);
//        mainVm = new(dialogStub, new vm.DAL.JsonConfig());
//        await Task.Delay(100);
//        await mainVm.StoreConfig();
//        await Task.Delay(100);

//        dialogStub = new();
//        mainVm = new(dialogStub, new vm.DAL.JsonConfig());
//        await Task.Delay(500);
//        dialogStub.InputName = "Client0";
//        mainVm.AddClientCommand.Execute(null);
//        await Task.Delay(100);
//        Assert.Equal("Client0", mainVm.Clients[1].Name);
//        Assert.Equal("Server2", mainVm.Servers[0].Name);
//        Assert.Equal("Client2", mainVm.Clients[0].Name);

//        mainVm.Clients[0].RemoveCommand.Execute(new ReadOnlyCollection<object>([mainVm.Servers, mainVm.Clients]));
//        mainVm.Clients[0].RemoveCommand.Execute(new ReadOnlyCollection<object>([mainVm.Servers, mainVm.Clients]));
//        mainVm.Servers[0].RemoveCommand.Execute(mainVm.Servers);
//        Assert.Empty(mainVm.Clients);
//        Assert.Empty(mainVm.Servers);
//        await mainVm.StoreConfig();
//        await Task.Delay(100);
//    }
//}
