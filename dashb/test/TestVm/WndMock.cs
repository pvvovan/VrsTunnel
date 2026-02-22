using vm.ViewModels;

namespace test.TestVm;

internal class WndMock : IWnd
{
    public bool CloseCalled = false;
    public void Close()
    {
        CloseCalled = true;
    }
}
