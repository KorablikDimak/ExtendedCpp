#ifndef DI_Register_H
#define DI_Register_H

#include <DI/ServiceProvider.h>

namespace DI
{
    template<typename TTarget>
    class Register;

    template<typename TTarget, typename... TDependencies>
    class Register<TTarget(TDependencies...)>
    {
    private:
        ServiceProvider _serviceProvider;

    public:
        explicit Register(const ServiceProvider& serviceProvider)
        {
            _serviceProvider = serviceProvider;
        }

        auto Create() const
        {
            constexpr std::size_t N = sizeof...(TDependencies);

            if constexpr (N == 0)
                return TTarget();
            if constexpr (N == 1)
            {
                using TDependency0 = std::tuple_element_t<0, std::tuple<TDependencies...>>;
                return TTarget(_serviceProvider.GetService<TDependency0>());
            }
            if constexpr (N == 2)
            {
                using TDependency0 = std::tuple_element_t<0, std::tuple<TDependencies...>>;
                using TDependency1 = std::tuple_element_t<1, std::tuple<TDependencies...>>;
                return TTarget(_serviceProvider.GetService<TDependency0>(),
                    _serviceProvider.GetService<TDependency1>());
            }
            if constexpr (N == 3)
            {
                using TDependency0 = std::tuple_element_t<0, std::tuple<TDependencies...>>;
                using TDependency1 = std::tuple_element_t<1, std::tuple<TDependencies...>>;
                using TDependency2 = std::tuple_element_t<2, std::tuple<TDependencies...>>;
                return TTarget(_serviceProvider.GetService<TDependency0>(),
                    _serviceProvider.GetService<TDependency1>(),
                    _serviceProvider.GetService<TDependency2>());
            }
            if constexpr (N == 4)
            {
                using TDependency0 = std::tuple_element_t<0, std::tuple<TDependencies...>>;
                using TDependency1 = std::tuple_element_t<1, std::tuple<TDependencies...>>;
                using TDependency2 = std::tuple_element_t<2, std::tuple<TDependencies...>>;
                using TDependency3 = std::tuple_element_t<3, std::tuple<TDependencies...>>;
                return TTarget(_serviceProvider.GetService<TDependency0>(),
                    _serviceProvider.GetService<TDependency1>(),
                    _serviceProvider.GetService<TDependency2>(),
                    _serviceProvider.GetService<TDependency3>());
            }
            if constexpr (N == 5)
            {
                using TDependency0 = std::tuple_element_t<0, std::tuple<TDependencies...>>;
                using TDependency1 = std::tuple_element_t<1, std::tuple<TDependencies...>>;
                using TDependency2 = std::tuple_element_t<2, std::tuple<TDependencies...>>;
                using TDependency3 = std::tuple_element_t<3, std::tuple<TDependencies...>>;
                using TDependency4 = std::tuple_element_t<4, std::tuple<TDependencies...>>;
                return TTarget(_serviceProvider.GetService<TDependency0>(),
                    _serviceProvider.GetService<TDependency1>(),
                    _serviceProvider.GetService<TDependency2>(),
                    _serviceProvider.GetService<TDependency3>(),
                    _serviceProvider.GetService<TDependency4>());
            }
            if constexpr (N == 6)
            {
                using TDependency0 = std::tuple_element_t<0, std::tuple<TDependencies...>>;
                using TDependency1 = std::tuple_element_t<1, std::tuple<TDependencies...>>;
                using TDependency2 = std::tuple_element_t<2, std::tuple<TDependencies...>>;
                using TDependency3 = std::tuple_element_t<3, std::tuple<TDependencies...>>;
                using TDependency4 = std::tuple_element_t<4, std::tuple<TDependencies...>>;
                using TDependency5 = std::tuple_element_t<5, std::tuple<TDependencies...>>;
                return TTarget(_serviceProvider.GetService<TDependency0>(),
                    _serviceProvider.GetService<TDependency1>(),
                    _serviceProvider.GetService<TDependency2>(),
                    _serviceProvider.GetService<TDependency3>(),
                    _serviceProvider.GetService<TDependency4>(),
                    _serviceProvider.GetService<TDependency5>());
            }
            if constexpr (N == 7)
            {
                using TDependency0 = std::tuple_element_t<0, std::tuple<TDependencies...>>;
                using TDependency1 = std::tuple_element_t<1, std::tuple<TDependencies...>>;
                using TDependency2 = std::tuple_element_t<2, std::tuple<TDependencies...>>;
                using TDependency3 = std::tuple_element_t<3, std::tuple<TDependencies...>>;
                using TDependency4 = std::tuple_element_t<4, std::tuple<TDependencies...>>;
                using TDependency5 = std::tuple_element_t<5, std::tuple<TDependencies...>>;
                using TDependency6 = std::tuple_element_t<6, std::tuple<TDependencies...>>;
                return TTarget(_serviceProvider.GetService<TDependency0>(),
                    _serviceProvider.GetService<TDependency1>(),
                    _serviceProvider.GetService<TDependency2>(),
                    _serviceProvider.GetService<TDependency3>(),
                    _serviceProvider.GetService<TDependency4>(),
                    _serviceProvider.GetService<TDependency5>(),
                    _serviceProvider.GetService<TDependency6>());
            }
            if constexpr (N == 8)
            {
                using TDependency0 = std::tuple_element_t<0, std::tuple<TDependencies...>>;
                using TDependency1 = std::tuple_element_t<1, std::tuple<TDependencies...>>;
                using TDependency2 = std::tuple_element_t<2, std::tuple<TDependencies...>>;
                using TDependency3 = std::tuple_element_t<3, std::tuple<TDependencies...>>;
                using TDependency4 = std::tuple_element_t<4, std::tuple<TDependencies...>>;
                using TDependency5 = std::tuple_element_t<5, std::tuple<TDependencies...>>;
                using TDependency6 = std::tuple_element_t<6, std::tuple<TDependencies...>>;
                using TDependency7 = std::tuple_element_t<7, std::tuple<TDependencies...>>;
                return TTarget(_serviceProvider.GetService<TDependency0>(),
                    _serviceProvider.GetService<TDependency1>(),
                    _serviceProvider.GetService<TDependency2>(),
                    _serviceProvider.GetService<TDependency3>(),
                    _serviceProvider.GetService<TDependency4>(),
                    _serviceProvider.GetService<TDependency5>(),
                    _serviceProvider.GetService<TDependency6>(),
                    _serviceProvider.GetService<TDependency7>());
            }
            if constexpr (N == 9)
            {
                using TDependency0 = std::tuple_element_t<0, std::tuple<TDependencies...>>;
                using TDependency1 = std::tuple_element_t<1, std::tuple<TDependencies...>>;
                using TDependency2 = std::tuple_element_t<2, std::tuple<TDependencies...>>;
                using TDependency3 = std::tuple_element_t<3, std::tuple<TDependencies...>>;
                using TDependency4 = std::tuple_element_t<4, std::tuple<TDependencies...>>;
                using TDependency5 = std::tuple_element_t<5, std::tuple<TDependencies...>>;
                using TDependency6 = std::tuple_element_t<6, std::tuple<TDependencies...>>;
                using TDependency7 = std::tuple_element_t<7, std::tuple<TDependencies...>>;
                using TDependency8 = std::tuple_element_t<8, std::tuple<TDependencies...>>;
                return TTarget(_serviceProvider.GetService<TDependency0>(),
                    _serviceProvider.GetService<TDependency1>(),
                    _serviceProvider.GetService<TDependency2>(),
                    _serviceProvider.GetService<TDependency3>(),
                    _serviceProvider.GetService<TDependency4>(),
                    _serviceProvider.GetService<TDependency5>(),
                    _serviceProvider.GetService<TDependency6>(),
                    _serviceProvider.GetService<TDependency7>(),
                    _serviceProvider.GetService<TDependency8>());
            }
            if constexpr (N == 10)
            {
                using TDependency0 = std::tuple_element_t<0, std::tuple<TDependencies...>>;
                using TDependency1 = std::tuple_element_t<1, std::tuple<TDependencies...>>;
                using TDependency2 = std::tuple_element_t<2, std::tuple<TDependencies...>>;
                using TDependency3 = std::tuple_element_t<3, std::tuple<TDependencies...>>;
                using TDependency4 = std::tuple_element_t<4, std::tuple<TDependencies...>>;
                using TDependency5 = std::tuple_element_t<5, std::tuple<TDependencies...>>;
                using TDependency6 = std::tuple_element_t<6, std::tuple<TDependencies...>>;
                using TDependency7 = std::tuple_element_t<7, std::tuple<TDependencies...>>;
                using TDependency8 = std::tuple_element_t<8, std::tuple<TDependencies...>>;
                using TDependency9 = std::tuple_element_t<9, std::tuple<TDependencies...>>;
                return TTarget(_serviceProvider.GetService<TDependency0>(),
                    _serviceProvider.GetService<TDependency1>(),
                    _serviceProvider.GetService<TDependency2>(),
                    _serviceProvider.GetService<TDependency3>(),
                    _serviceProvider.GetService<TDependency4>(),
                    _serviceProvider.GetService<TDependency5>(),
                    _serviceProvider.GetService<TDependency6>(),
                    _serviceProvider.GetService<TDependency7>(),
                    _serviceProvider.GetService<TDependency8>(),
                    _serviceProvider.GetService<TDependency9>());
            }
            if constexpr (N == 11)
            {
                using TDependency0 = std::tuple_element_t<0, std::tuple<TDependencies...>>;
                using TDependency1 = std::tuple_element_t<1, std::tuple<TDependencies...>>;
                using TDependency2 = std::tuple_element_t<2, std::tuple<TDependencies...>>;
                using TDependency3 = std::tuple_element_t<3, std::tuple<TDependencies...>>;
                using TDependency4 = std::tuple_element_t<4, std::tuple<TDependencies...>>;
                using TDependency5 = std::tuple_element_t<5, std::tuple<TDependencies...>>;
                using TDependency6 = std::tuple_element_t<6, std::tuple<TDependencies...>>;
                using TDependency7 = std::tuple_element_t<7, std::tuple<TDependencies...>>;
                using TDependency8 = std::tuple_element_t<8, std::tuple<TDependencies...>>;
                using TDependency9 = std::tuple_element_t<9, std::tuple<TDependencies...>>;
                using TDependency10 = std::tuple_element_t<10, std::tuple<TDependencies...>>;
                return TTarget(_serviceProvider.GetService<TDependency0>(),
                    _serviceProvider.GetService<TDependency1>(),
                    _serviceProvider.GetService<TDependency2>(),
                    _serviceProvider.GetService<TDependency3>(),
                    _serviceProvider.GetService<TDependency4>(),
                    _serviceProvider.GetService<TDependency5>(),
                    _serviceProvider.GetService<TDependency6>(),
                    _serviceProvider.GetService<TDependency7>(),
                    _serviceProvider.GetService<TDependency8>(),
                    _serviceProvider.GetService<TDependency9>(),
                    _serviceProvider.GetService<TDependency10>());
            }
        }

        auto CreateRequired() const
        {
            constexpr std::size_t N = sizeof...(TDependencies);

            if constexpr (N == 0)
                return TTarget();
            if constexpr (N == 1)
            {
                using TDependency0 = std::tuple_element_t<0, std::tuple<TDependencies...>>;
                return TTarget(_serviceProvider.GetServiceRequired<TDependency0>());
            }
            if constexpr (N == 2)
            {
                using TDependency0 = std::tuple_element_t<0, std::tuple<TDependencies...>>;
                using TDependency1 = std::tuple_element_t<1, std::tuple<TDependencies...>>;
                return TTarget(_serviceProvider.GetServiceRequired<TDependency0>(),
                    _serviceProvider.GetServiceRequired<TDependency1>());
            }
            if constexpr (N == 3)
            {
                using TDependency0 = std::tuple_element_t<0, std::tuple<TDependencies...>>;
                using TDependency1 = std::tuple_element_t<1, std::tuple<TDependencies...>>;
                using TDependency2 = std::tuple_element_t<2, std::tuple<TDependencies...>>;
                return TTarget(_serviceProvider.GetServiceRequired<TDependency0>(),
                    _serviceProvider.GetServiceRequired<TDependency1>(),
                    _serviceProvider.GetServiceRequired<TDependency2>());
            }
            if constexpr (N == 4)
            {
                using TDependency0 = std::tuple_element_t<0, std::tuple<TDependencies...>>;
                using TDependency1 = std::tuple_element_t<1, std::tuple<TDependencies...>>;
                using TDependency2 = std::tuple_element_t<2, std::tuple<TDependencies...>>;
                using TDependency3 = std::tuple_element_t<3, std::tuple<TDependencies...>>;
                return TTarget(_serviceProvider.GetServiceRequired<TDependency0>(),
                    _serviceProvider.GetServiceRequired<TDependency1>(),
                    _serviceProvider.GetServiceRequired<TDependency2>(),
                    _serviceProvider.GetServiceRequired<TDependency3>());
            }
            if constexpr (N == 5)
            {
                using TDependency0 = std::tuple_element_t<0, std::tuple<TDependencies...>>;
                using TDependency1 = std::tuple_element_t<1, std::tuple<TDependencies...>>;
                using TDependency2 = std::tuple_element_t<2, std::tuple<TDependencies...>>;
                using TDependency3 = std::tuple_element_t<3, std::tuple<TDependencies...>>;
                using TDependency4 = std::tuple_element_t<4, std::tuple<TDependencies...>>;
                return TTarget(_serviceProvider.GetServiceRequired<TDependency0>(),
                    _serviceProvider.GetServiceRequired<TDependency1>(),
                    _serviceProvider.GetServiceRequired<TDependency2>(),
                    _serviceProvider.GetServiceRequired<TDependency3>(),
                    _serviceProvider.GetServiceRequired<TDependency4>());
            }
            if constexpr (N == 6)
            {
                using TDependency0 = std::tuple_element_t<0, std::tuple<TDependencies...>>;
                using TDependency1 = std::tuple_element_t<1, std::tuple<TDependencies...>>;
                using TDependency2 = std::tuple_element_t<2, std::tuple<TDependencies...>>;
                using TDependency3 = std::tuple_element_t<3, std::tuple<TDependencies...>>;
                using TDependency4 = std::tuple_element_t<4, std::tuple<TDependencies...>>;
                using TDependency5 = std::tuple_element_t<5, std::tuple<TDependencies...>>;
                return TTarget(_serviceProvider.GetServiceRequired<TDependency0>(),
                    _serviceProvider.GetServiceRequired<TDependency1>(),
                    _serviceProvider.GetServiceRequired<TDependency2>(),
                    _serviceProvider.GetServiceRequired<TDependency3>(),
                    _serviceProvider.GetServiceRequired<TDependency4>(),
                    _serviceProvider.GetServiceRequired<TDependency5>());
            }
            if constexpr (N == 7)
            {
                using TDependency0 = std::tuple_element_t<0, std::tuple<TDependencies...>>;
                using TDependency1 = std::tuple_element_t<1, std::tuple<TDependencies...>>;
                using TDependency2 = std::tuple_element_t<2, std::tuple<TDependencies...>>;
                using TDependency3 = std::tuple_element_t<3, std::tuple<TDependencies...>>;
                using TDependency4 = std::tuple_element_t<4, std::tuple<TDependencies...>>;
                using TDependency5 = std::tuple_element_t<5, std::tuple<TDependencies...>>;
                using TDependency6 = std::tuple_element_t<6, std::tuple<TDependencies...>>;
                return TTarget(_serviceProvider.GetServiceRequired<TDependency0>(),
                    _serviceProvider.GetServiceRequired<TDependency1>(),
                    _serviceProvider.GetServiceRequired<TDependency2>(),
                    _serviceProvider.GetServiceRequired<TDependency3>(),
                    _serviceProvider.GetServiceRequired<TDependency4>(),
                    _serviceProvider.GetServiceRequired<TDependency5>(),
                    _serviceProvider.GetServiceRequired<TDependency6>());
            }
            if constexpr (N == 8)
            {
                using TDependency0 = std::tuple_element_t<0, std::tuple<TDependencies...>>;
                using TDependency1 = std::tuple_element_t<1, std::tuple<TDependencies...>>;
                using TDependency2 = std::tuple_element_t<2, std::tuple<TDependencies...>>;
                using TDependency3 = std::tuple_element_t<3, std::tuple<TDependencies...>>;
                using TDependency4 = std::tuple_element_t<4, std::tuple<TDependencies...>>;
                using TDependency5 = std::tuple_element_t<5, std::tuple<TDependencies...>>;
                using TDependency6 = std::tuple_element_t<6, std::tuple<TDependencies...>>;
                using TDependency7 = std::tuple_element_t<7, std::tuple<TDependencies...>>;
                return TTarget(_serviceProvider.GetServiceRequired<TDependency0>(),
                    _serviceProvider.GetServiceRequired<TDependency1>(),
                    _serviceProvider.GetServiceRequired<TDependency2>(),
                    _serviceProvider.GetServiceRequired<TDependency3>(),
                    _serviceProvider.GetServiceRequired<TDependency4>(),
                    _serviceProvider.GetServiceRequired<TDependency5>(),
                    _serviceProvider.GetServiceRequired<TDependency6>(),
                    _serviceProvider.GetServiceRequired<TDependency7>());
            }
            if constexpr (N == 9)
            {
                using TDependency0 = std::tuple_element_t<0, std::tuple<TDependencies...>>;
                using TDependency1 = std::tuple_element_t<1, std::tuple<TDependencies...>>;
                using TDependency2 = std::tuple_element_t<2, std::tuple<TDependencies...>>;
                using TDependency3 = std::tuple_element_t<3, std::tuple<TDependencies...>>;
                using TDependency4 = std::tuple_element_t<4, std::tuple<TDependencies...>>;
                using TDependency5 = std::tuple_element_t<5, std::tuple<TDependencies...>>;
                using TDependency6 = std::tuple_element_t<6, std::tuple<TDependencies...>>;
                using TDependency7 = std::tuple_element_t<7, std::tuple<TDependencies...>>;
                using TDependency8 = std::tuple_element_t<8, std::tuple<TDependencies...>>;
                return TTarget(_serviceProvider.GetServiceRequired<TDependency0>(),
                    _serviceProvider.GetServiceRequired<TDependency1>(),
                    _serviceProvider.GetServiceRequired<TDependency2>(),
                    _serviceProvider.GetServiceRequired<TDependency3>(),
                    _serviceProvider.GetServiceRequired<TDependency4>(),
                    _serviceProvider.GetServiceRequired<TDependency5>(),
                    _serviceProvider.GetServiceRequired<TDependency6>(),
                    _serviceProvider.GetServiceRequired<TDependency7>(),
                    _serviceProvider.GetServiceRequired<TDependency8>());
            }
            if constexpr (N == 10)
            {
                using TDependency0 = std::tuple_element_t<0, std::tuple<TDependencies...>>;
                using TDependency1 = std::tuple_element_t<1, std::tuple<TDependencies...>>;
                using TDependency2 = std::tuple_element_t<2, std::tuple<TDependencies...>>;
                using TDependency3 = std::tuple_element_t<3, std::tuple<TDependencies...>>;
                using TDependency4 = std::tuple_element_t<4, std::tuple<TDependencies...>>;
                using TDependency5 = std::tuple_element_t<5, std::tuple<TDependencies...>>;
                using TDependency6 = std::tuple_element_t<6, std::tuple<TDependencies...>>;
                using TDependency7 = std::tuple_element_t<7, std::tuple<TDependencies...>>;
                using TDependency8 = std::tuple_element_t<8, std::tuple<TDependencies...>>;
                using TDependency9 = std::tuple_element_t<9, std::tuple<TDependencies...>>;
                return TTarget(_serviceProvider.GetServiceRequired<TDependency0>(),
                    _serviceProvider.GetServiceRequired<TDependency1>(),
                    _serviceProvider.GetServiceRequired<TDependency2>(),
                    _serviceProvider.GetServiceRequired<TDependency3>(),
                    _serviceProvider.GetServiceRequired<TDependency4>(),
                    _serviceProvider.GetServiceRequired<TDependency5>(),
                    _serviceProvider.GetServiceRequired<TDependency6>(),
                    _serviceProvider.GetServiceRequired<TDependency7>(),
                    _serviceProvider.GetServiceRequired<TDependency8>(),
                    _serviceProvider.GetServiceRequired<TDependency9>());
            }
            if constexpr (N == 11)
            {
                using TDependency0 = std::tuple_element_t<0, std::tuple<TDependencies...>>;
                using TDependency1 = std::tuple_element_t<1, std::tuple<TDependencies...>>;
                using TDependency2 = std::tuple_element_t<2, std::tuple<TDependencies...>>;
                using TDependency3 = std::tuple_element_t<3, std::tuple<TDependencies...>>;
                using TDependency4 = std::tuple_element_t<4, std::tuple<TDependencies...>>;
                using TDependency5 = std::tuple_element_t<5, std::tuple<TDependencies...>>;
                using TDependency6 = std::tuple_element_t<6, std::tuple<TDependencies...>>;
                using TDependency7 = std::tuple_element_t<7, std::tuple<TDependencies...>>;
                using TDependency8 = std::tuple_element_t<8, std::tuple<TDependencies...>>;
                using TDependency9 = std::tuple_element_t<9, std::tuple<TDependencies...>>;
                using TDependency10 = std::tuple_element_t<10, std::tuple<TDependencies...>>;
                return TTarget(_serviceProvider.GetServiceRequired<TDependency0>(),
                    _serviceProvider.GetServiceRequired<TDependency1>(),
                    _serviceProvider.GetServiceRequired<TDependency2>(),
                    _serviceProvider.GetServiceRequired<TDependency3>(),
                    _serviceProvider.GetServiceRequired<TDependency4>(),
                    _serviceProvider.GetServiceRequired<TDependency5>(),
                    _serviceProvider.GetServiceRequired<TDependency6>(),
                    _serviceProvider.GetServiceRequired<TDependency7>(),
                    _serviceProvider.GetServiceRequired<TDependency8>(),
                    _serviceProvider.GetServiceRequired<TDependency9>(),
                    _serviceProvider.GetServiceRequired<TDependency10>());
            }
        }
    };
}

#endif