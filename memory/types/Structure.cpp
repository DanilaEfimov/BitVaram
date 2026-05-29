#include "Structure.h"

StructureField::StructureField(const std::string& name, std::unique_ptr<IValue>&& value)
    : name(name), value(std::move(value))
{}

Structure::Structure(std::string name)
    : typeName(name)
{}

void Structure::set(StructureField field)
{
    this->fields.emplace(field.name, std::move(field));
}

const StructureField const* Structure::get(const std::string& fieldName) const
{
    auto it = fields.find(fieldName);

    if (it == fields.end())
        return nullptr;

    return &it->second;
}
