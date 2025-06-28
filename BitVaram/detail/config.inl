template<typename T>
T varam::Config::get(const std::string& key) const
{
    if (this->vm.count(key)) {
        return this->vm[key].as<T>();
    }
    throw std::runtime_error("Missing option: " + key);
};