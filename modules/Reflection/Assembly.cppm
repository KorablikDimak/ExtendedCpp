module;

#include <map>
#include <mutex>
#include <typeindex>
#include <vector>

export module ExtendedCpp.Reflection.Assembly;

export namespace ExtendedCpp::Reflection
{
    class TypeInfo;

    class Assembly final
    {
    private:
        static std::map<std::type_index, TypeInfo>  _typeList;
        static std::mutex _listMutex;

    public:
        Assembly() = delete;

        static void AddType(const TypeInfo& typeInfo) noexcept
        {

        }

        static std::vector<TypeInfo> GetTypes() noexcept
        {

        }
    };
}