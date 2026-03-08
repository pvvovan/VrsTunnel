namespace test.TestVm;

public class ExclusiveJsonConfig : IDisposable
{
    private readonly Mutex _mut = new(false, "test.TestVm.ExclusiveJsonConfig");

    public ExclusiveJsonConfig()
    {
        _mut.WaitOne();
    }

    public void Dispose()
    {
        _mut.Dispose();
    }
}
