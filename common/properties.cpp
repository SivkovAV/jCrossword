// (c) Copyright Aleksey Sivkov 2018

#include <properties.hpp>
#include <QDateTime>

using namespace jcw;

FileProperties::FileProperties(bool isReadyToPlay)
    : PreloadedStringsMap<std::string>({
        "id", "author", "creating_date", "gui_name", "is_ready_to_play" })
{
    const auto datetime = QDateTime::currentDateTime().toString();
    (*this)["creating_date"] = datetime.toStdString();

    if (isReadyToPlay)
    {
        markAsReadyToPlay();
    }
}

void FileProperties::markAsReadyToPlay()
{
    (*this)["is_ready_to_play"] = "1";
}

FileProperties FileProperties::operator=(const FileProperties& names)
{
    *((std::map<std::string, std::string>*)this) =
        *((std::map<std::string, std::string>*)(&names));
    return *this;
}