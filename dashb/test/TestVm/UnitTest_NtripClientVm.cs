using System.Collections.ObjectModel;
using System.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using vm.Models;
using vm.ViewModels;

namespace test.TestVm;

#pragma warning disable CS9113 // Parameter exclusiveJsonConfig is unread.
public class UnitTestVm(ExclusiveJsonConfig exclusiveJsonConfig) : IClassFixture<ExclusiveJsonConfig>
#pragma warning restore CS9113 // Parameter exclusiveJsonConfig is unread.
{
   [Fact]
   public void Test_NtripServerVm()
   {
       NtripServer server = new()
       {
           Name = "Server 1",
           PasswordHash = "Pw1",
           Id = Guid.NewGuid(),
       };
       RelayCommand<NtripServerVm> stubCmd = new(p => { });
       NtripServerVm serverVm = new(server);
       Assert.Equal(serverVm.Name, server.Name);
   }

   [Fact]
   public void Test_NtripClientVm()
   {
       NtripClient client = new()
       {
            Name = "Client 1",
            PasswordHash = "Pw1",
            Id = Guid.NewGuid(),
       };
       RelayCommand<NtripClientVm> stubCmd = new(p => { });
       NtripClientVm clientVm = new(client);
       Assert.Equal(clientVm.Name, client.Name);
   }

   [Fact]
   public void Test_InputVm()
   {
       InputVm inputVm = new(new NtripServerVm(new NtripServer()), new RelayCommand<UserVm>((_)=>{}), ()=>{});
       inputVm.Close();
       inputVm.User_PropertyChanged(null, new PropertyChangedEventArgs(""));
       Assert.False(inputVm.CanOkExecute());

       var wnd = new WndMock()
       {
           CloseCalled = false
       };
       inputVm.Wnd = wnd;
       inputVm.Close();
       Assert.True(wnd.CloseCalled);
   }

   [Fact]
   public async Task Test_MainWindowVm()
   {
       DialogStub dialogStub = new();
       MainWindowVm mainVm = new(dialogStub, new vm.DAL.JsonConfig(), null);
       await Task.Delay(500);
       mainVm.Servers.Clear();
       mainVm.Clients.Clear();

       dialogStub.InputName = "Client0";
       mainVm.CreateClientCommand.Execute(null);
       await Task.Delay(100);

       Assert.Single(mainVm.Clients);
       Assert.Equal("Client0", mainVm.Clients[0].Name);
       mainVm.Clients[0].RemoveCommand.Execute(new ReadOnlyCollection<object>([mainVm.Servers, mainVm.Clients]));

       dialogStub.InputName = "inputName";
       mainVm.CreateServerCommand.Execute(null);
       Assert.Equal("inputName", mainVm.Servers[0].Name);

       mainVm.CreateClientCommand.Execute(null);
       Assert.Equal("inputName", mainVm.Clients[0].Name);

       dialogStub.InputName = "Client1";
       mainVm.CreateClientCommand.Execute(null);
       Assert.Equal("Client1", mainVm.Clients[1].Name);

       dialogStub.InputName = "Server2";
       mainVm.Servers[0].EditCmd?.Execute(mainVm.Servers[0]);
       Assert.Equal("Server2", mainVm.Servers[0].Name);
   }
}
