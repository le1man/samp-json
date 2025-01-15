#include <Server/Components/Pawn/pawn.hpp>
#include <Server/Components/Pawn/Impl/pawn_natives.hpp>
#include <Server/Components/Pawn/Impl/pawn_impl.hpp>

#include <sdk.hpp>

#include <string>
#include <vector>

#include "json_spirit/json_spirit.h"
#include "settings.h"

struct t_JsonData
{
    unsigned id;
    json_spirit::Object object;
};
std::vector<t_JsonData> json_data;
unsigned json_data_id = 0;

class SampJson final : public IComponent,
                       public PawnEventHandler
{
private:
    ICore *core_ = nullptr;
    IPawnComponent *pawn_ = nullptr;

public:
    PROVIDE_UID(0x151CF6FD9750FEC4);

    SampJson()
    {
    }
    ~SampJson()
    {
        if (pawn_)
        {
            pawn_->getEventDispatcher().removeEventHandler(this);
        }
        if (core_)
        {
        }
    }

    void onAmxLoad(IPawnScript &script) override
    {
        pawn_natives::AmxLoad(script.GetAMX());
    }

    void onAmxUnload(IPawnScript &script) override
    {
    }

    StringView componentName() const override
    {
        return "SA:MP JSON";
    }

    SemanticVersion componentVersion() const override
    {
        return SemanticVersion(1, 2, 0, 0);
    }

    void onLoad(ICore *c) override
    {
        core_ = c;

        setAmxLookups(core_);

        // core_->printLn("SampJson: succesfully loaded!");
        core_->printLn(" -----------------------------------------------------------------");
        core_->printLn(" SA:MP JSON plugin version %s successfully loaded!", PLUGIN_VERSION);
        core_->printLn(" Developer: Manuel Schnitzer");
        core_->printLn(" -----------------------------------------------------------------");
    }

    void onInit(IComponentList *components) override
    {
        pawn_ = components->queryComponent<IPawnComponent>();

        if (pawn_)
        {
            setAmxFunctions(pawn_->getAmxFunctions());
            setAmxLookups(components);
            pawn_->getEventDispatcher().addEventHandler(this);
        }
    }

    void onReady() override
    {
        // Fire events here at earliest.
    }

    void onFree(IComponent *component) override
    {
        if (component == pawn_)
        {
            pawn_ = nullptr;
            setAmxFunctions();
            setAmxLookups();
        }
        core_->printLn(" * SA:MP JSON plugin was unloaded.");
    }

    void free() override
    {
        // Deletes the component.
        delete this;
    }

    void reset() override
    {
        // Resets data when the mode changes.
    }
};

COMPONENT_ENTRY_POINT()
{
    return new SampJson();
}

SCRIPT_API(json_decode, int(std::string const &json_string))
{
    json_spirit::Value value;
    auto success = json_spirit::read(json_string, value);

    if (success == false)
        return 0;

    ++json_data_id;
    json_data.push_back({json_data_id, value.get_obj()});

    return json_data_id;
}

SCRIPT_API(json_get_int, int(int const data_id, std::string const &key))
{
    for (auto &x : json_data)
    {
        if (x.id == data_id)
        {
            for (auto &i : x.object)
            {
                if (i.name_ == key && i.value_.type() == json_spirit::int_type)
                {
                    return i.value_.get_int();
                }
            }
        }
    }
    return -1;
}

SCRIPT_API(json_get_string, bool(int const data_id, std::string const &key, OutputOnlyString &value))
{
    for (auto &x : json_data)
    {
        if (x.id == data_id)
        {
            for (auto &i : x.object)
            {
                if (i.name_ == key && i.value_.type() == json_spirit::str_type)
                {
                    std::string str;
                    str.assign(i.value_.get_str().c_str());
                    value = str;

                    return true;
                }
            }
        }
    }
    return false;
}

SCRIPT_API(json_get_float, float(int const data_id, std::string const &key))
{
    for (auto &x : json_data)
    {
        if (x.id == data_id)
        {
            for (auto &i : x.object)
            {
                if (i.name_ == key && i.value_.type() == json_spirit::real_type)
                {
                    return (float)i.value_.get_real();
                }
            }
        }
    }

    return -1.0f;
}

SCRIPT_API(json_delete, bool(int const data_id))
{
    int index = 0;
    for (auto &i : json_data)
    {
        if (i.id == data_id)
        {
            json_data.erase(json_data.begin() + index);
            return true;
        }

        ++index;
    }
    return false;
}