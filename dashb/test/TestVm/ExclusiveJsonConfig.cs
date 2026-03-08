namespace test.TestVm;

public class ExclusiveJsonConfig : IDisposable
{
    private readonly Mutex _mut = new(false, "test.TestVm.ExclusiveJsonConfig");

    public ExclusiveJsonConfig()
    {
        for ( ; ; )
        {
            try
            {
                _mut.WaitOne();
                break;
            }
            catch (AbandonedMutexException) { }
        }
    }

    public void Dispose()
    {
        _mut.Dispose();
    }
}
