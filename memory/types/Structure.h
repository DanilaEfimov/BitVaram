#pragma once

#include "IValue.h"

#include <string>
#include <memory>
#include <unordered_map>

typename std::unique_ptr<IValue> StructureFieldValue;

struct StructureField 
{
	const std::string name;
	std::unique_ptr<IValue> value;

	StructureField(const std::string& name, std::unique_ptr<IValue>&& value);
	virtual ~StructureField() = default;
};

class Structure : public IValue
{
	const std::string typeName;
	std::unordered_map<std::string, StructureField> fields;

public:
	Structure(std::string name);
	~Structure() = default;

	void set(StructureField field);
	const StructureField const* get(const std::string& fieldName) const;
};

