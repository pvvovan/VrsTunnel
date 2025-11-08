using CommunityToolkit.Mvvm.Input;
using dashb.Models;
using dashb.ViewModels;

namespace TestVm;

public class UnitTestVm
{
    [Fact]
    public void Test_NtripServerVm()
    {
        NtripServer server = new("Server 1", "Pw1", Guid.NewGuid(), null);
        RelayCommand<NtripServerVm> stubCmd = new(p => { });
        NtripServerVm serverVm = new(stubCmd, server);
        Assert.Equal(serverVm.Name, server.Name);
    }

    [Fact]
    public void Test_NtripClientVm()
    {
        NtripClient client = new("Client 1", "Pw1", Guid.NewGuid());
        RelayCommand<NtripClientVm> stubCmd = new(p => { });
        NtripClientVm clientVm = new(stubCmd, stubCmd, stubCmd, client);
        Assert.Equal(clientVm.Name, client.Name);
    }

    [Fact]
    public void Test_InputVm()
    {
        InputVm inputVm = new();
        inputVm.Close();
        inputVm.User_PropertyChanged(null, new System.ComponentModel.PropertyChangedEventArgs(""));
        Assert.False(inputVm.CanOkExecute());
    }

    [Fact]
    public void Test_MainWindowVm()
    {
        MainWindowVm mainVm = new(new DialogStub(), new dashb.DAL.JsonConfig());

        mainVm.AddServerCmd.Execute(null);
        Assert.Equal("inputName", mainVm.Servers[0].Name);

        mainVm.AddClientCmd.Execute(null);
        Assert.Equal("inputName", mainVm.Clients[0].Name);
    }
}

internal class DialogStub : IDialog
{
    public void Show(InputVm inputVm)
    {
        inputVm.User!.Name = "inputName";
        inputVm.User!.Password = "inputPass";
        inputVm.OkCmd?.Execute(null);
    }
}
