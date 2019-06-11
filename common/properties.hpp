#pragma once

// (c) Copyright Aleksey Sivkov 2018

#include <string>
#include <vector>
#include <map>
#include <QColor>

namespace jcw
{
    template <typename T>
    class PreloadedStringsMap : public std::map<std::string, T>
    {
    public:
        PreloadedStringsMap(std::vector<std::string> names);
        void clear();
        PreloadedStringsMap& operator=(PreloadedStringsMap& names);

    private:
        const std::vector<std::string> preloadedNames_;
    };

    class FileProperties : public PreloadedStringsMap<std::string>
    {
    public:
        FileProperties(bool isReadyToPlay = false);

        void markAsReadyToPlay();
        FileProperties operator=(const FileProperties& names);
    };

    template <typename T>
    PreloadedStringsMap<T>::PreloadedStringsMap(std::vector<std::string> names)
        : preloadedNames_{ names }
    {
        clear();
    }

    template <typename T>
    void PreloadedStringsMap<T>::clear()
    {
        std::map<std::string, T>::clear();
        for (auto& name : preloadedNames_)
        {
            this->operator[](name) = T();
        }
    }

    template <typename T>
    PreloadedStringsMap<T>& PreloadedStringsMap<T>::operator=(
        PreloadedStringsMap<T>& names)
    {
        *((std::vector<std::string>*)this) =
            *((std::vector<std::string>*)(&names));
        return *this;
    }

} // namespace jcw