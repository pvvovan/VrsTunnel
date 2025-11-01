using CommunityToolkit.Mvvm.Input;
using dashb.Models;
using dashb.ViewModels;

namespace test;

public class UnitTest1
{
    [Fact]
    public void Test_NtripServerVm()
    {
        NtripClient client = new("Client 1", "Pw1", Guid.NewGuid());
        RelayCommand<NtripClientVm> stubCmd = new(p => { });
        NtripClientVm clientVm = new(stubCmd, stubCmd, stubCmd, client);
        Assert.Equal(clientVm.Name, client.Name);
    }
}
