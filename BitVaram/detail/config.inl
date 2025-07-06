template<typename T>
T varam::Config::get(const std::string& key) const noexcept(false)
{
    if (this->vm.count(key)) {
        return this->vm[key].as<T>();
    }
    throw std::runtime_error("Missing option: " + key);
};

template<typename T>
void varam::Config::set(const std::string& key, const T& value) noexcept(false) {
    if (this->vm.count(key)) {
        this->vm.at(key).value() = value;
    }
    else {
        throw std::runtime_error("Missing option: " + key);
    }
}