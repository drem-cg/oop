#include "RemoteControl.h"
#include "TVSet.h"
#include <algorithm>
#include <sstream>

RemoteControl::RemoteControl(TVSet& tv, std::istream& input,
	std::ostream& output)
	: m_tv(tv)
	, m_input(input)
	, m_output(output)
	, m_actionMap{
		{ "TurnOn", [this](auto& stream) { return TurnOn(stream); } },
		{ "TurnOff", [this](auto& stream) { return TurnOff(stream); } },
		{ "Info", [this](auto& stream) { return Info(stream); } },
		{ "SelectChannel", [this](auto& stream) { return SelectChannel(stream); } },
		{ "SelectPreviousChannel",
			[this](auto& stream) { return SelectPreviousChannel(stream); } },
		{ "SetChannelName", [this](auto& stream) { return SetChannelName(stream); } },
		{ "DeleteChannelName",
			[this](auto& stream) { return DeleteChannelName(stream); } },
		{ "GetChannelName", [this](auto& stream) { return GetChannelName(stream); } },
		{ "GetChannelByName", [this](auto& stream) { return GetChannelByName(stream); } },
	}
{
}

bool RemoteControl::HandleCommand() const
{
	std::string line;
	if (!std::getline(m_input, line))
	{
		return false;
	}

	std::istringstream stream(line);
	std::string command;
	stream >> command;

	const auto action = m_actionMap.find(command);
	if (action != m_actionMap.end())
		return action->second(stream);

	return false;
}

bool RemoteControl::TurnOn(std::istream&) const
{
	m_tv.TurnOn();
	m_output << "TV is turned on\n";
	return true;
}

bool RemoteControl::TurnOff(std::istream&) const
{
	m_tv.TurnOff();
	m_output << "TV is turned off\n";
	return true;
}

bool RemoteControl::Info(std::istream&) const
{
	if (!m_tv.IsTurnedOn())
	{
		m_output << "TV is turned off\n";
		return true;
	}

	m_output << "TV is turned on\n";
	m_output << "Channel is: " << m_tv.GetChannel() << "\n";

	auto names = GetSortedChannelNames();
	for (const auto& [num, name] : names)
	{
		m_output << num << " - " << name << "\n";
	}

	return true;
}

RemoteControl::ChannelList RemoteControl::GetSortedChannelNames() const
{
	ChannelList result;

	for (int ch = TVSet::MIN_CHANNEL; ch <= TVSet::MAX_CHANNEL; ++ch)
	{
		if (auto nameOpt = m_tv.GetChannelName(ch))
		{
			result.emplace_back(ch, *nameOpt);
		}
	}

	std::ranges::sort(
		result, [](const auto& a, const auto& b) { return a.first < b.first; });

	return result;
}

bool RemoteControl::SelectChannel(std::istream& args) const
{
	std::string arg;
	if (!(args >> arg))
	{
		m_output << "Arguments reading error\n";
		return true;
	}

	try
	{
		size_t pos;
		const int channel = std::stoi(arg, &pos);

		if (pos == arg.length())
		{
			if (m_tv.SelectChannelByNumber(channel))
			{
				m_output << "Channel switched to: " << channel << "\n";
			}
			else
			{
				m_output << "Channel switch error\n";
			}
			return true;
		}
	}
	catch (...)
	{
	}
	std::string fullName = arg;
	std::string rest;
	while (args >> rest)
	{
		fullName += " " + rest;
	}

	if (m_tv.SelectChannelByName(fullName))
		m_output << "Channel switched to: " << fullName << "\n";
	else
		m_output << "Channel switch error\n";

	return true;
}
bool RemoteControl::SelectPreviousChannel(std::istream&) const
{
	if (m_tv.SelectPreviousChannel())
		m_output << "Switched to previous channel\n";
	else
		m_output << "Channel switch error\n";

	return true;
}

bool RemoteControl::SetChannelName(std::istream& args) const
{
	int channel;
	if (!(args >> channel))
	{
		m_output << "Arguments reading error\n";
		return true;
	}

	std::string name;
	std::getline(args >> std::ws, name);

	if (name.empty())
	{
		m_output << "ERROR\n";
		return true;
	}

	if (m_tv.SetChannelName(channel, name))
		m_output << "Channel name set: " << channel << " - " << name << "\n";
	else
		m_output << "Channel set error\n";

	return true;
}

bool RemoteControl::DeleteChannelName(std::istream& args) const
{
	std::string name;
	std::getline(args >> std::ws, name);

	if (name.empty())
	{
		m_output << "Error: channel name is not found\n";
		return true;
	}

	if (m_tv.DeleteChannelName(name))
	{
		m_output << "Channel name deleted: " << name << "\n";
	}
	else
	{
		m_output << "Error: channel deleting error\n";
	}

	return true;
}

bool RemoteControl::GetChannelName(std::istream& args) const
{
	int channel;
	if (!(args >> channel))
	{
		m_output << "Arguments reading error\n";
		return true;
	}

	if (const auto channelName = m_tv.GetChannelName(channel))
	{
		m_output << "Channel " << channel << " name: " << *channelName << "\n";
	}
	else
		m_output << "Channel get error\n";

	return true;
}

bool RemoteControl::GetChannelByName(std::istream& args) const
{
	std::string name;
	std::getline(args >> std::ws, name);

	if (name.empty())
	{
		m_output << "Error: channel name is not found\n";
		return true;
	}

	if (const auto channelName = m_tv.GetChannelByName(name))
	{
		m_output << "Channel for name " << name << ": " << *channelName << "\n";
	}
	else
	{
		m_output << "Error: channel name is not found\n";
	}

	return false;
}