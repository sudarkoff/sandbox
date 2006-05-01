template
<
    class T,
    template <class> class CreationPolicy = CreateUsingNew,
    template <class> class LifetimePolicy = DefaultLifetime,
    template <class> class ThreadingModel = SingleThreaded
>
class SingletonHolder {
public:
    static T& Instance();

private:
    // Helpers
    static void DestroySingleton();
    
    // Protection
    SingletonHolder();
    // ...

    //Data
    typedef ThreadingModel<T>::VolatileType InstanceType;
    static InstanceType* pInstance_;
    static bool destroyed_;
};
