#include "RemoteControl.h"
#include "Logger.h"
#include "TVSet.h"
#include <algorithm>
#include <sstream>

RemoteControl::RemoteControl(TVSet& tv, std::istream& input, std::ostream& output)
	: m_tv(tv)
	, m_input(input)
	, m_output(output)
	, m_actionMap{
		{ "TurnOn", [this](auto& stream) { return TurnOn(stream); } },
		{ "TurnOff", [this](auto& stream) { return TurnOff(stream); } },
		{ "Info", [this](auto& stream) { return Info(stream); } },
		{ "SelectChannel", [this](auto& stream) { return SelectChannel(stream); } },
		{ "SelectPreviousChannel", [this](auto& stream) { return SelectPreviousChannel(stream); } },
		{ "SetChannelName", [this](auto& stream) { return SetChannelName(stream); } },
		{ "DeleteChannelName", [this](auto& stream) { return DeleteChannelName(stream); } },
		{ "GetChannelName", [this](auto& stream) { return GetChannelName(stream); } },
		{ "GetChannelByName", [this](auto& stream) { return GetChannelByName(stream); } },
	}
{
}

bool RemoteControl::HandleCommand()
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
	{
		return action->second(stream);
	}

	m_output << "ERROR\n";
	Logger::Debug("Unknown Command");
	return true;
}

bool RemoteControl::TurnOn(std::istream&)
{
	m_tv.TurnOn();
	m_output << "TV is turned on\n";
	return false;
}

bool RemoteControl::TurnOff(std::istream&)
{
	m_tv.TurnOff();
	m_output << "TV is turned off\n";
	return false;
}

bool RemoteControl::Info(std::istream&)
{
	if (!m_tv.IsTurnedOn())
	{
		m_output << "TV is turned off\n";
		return false;
	}

	m_output << "TV is turned on\n";
	m_output << "Channel is: " << m_tv.GetChannel() << "\n";

	auto names = GetSortedChannelNames();
	for (const auto& [num, name] : names)
	{
		m_output << num << " - " << name << "\n";
	}

	return false;
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

	std::ranges::sort(result, [](const auto& a, const auto& b) {
		return a.first < b.first;
	});

	return result;
}

bool RemoteControl::SelectChannel(std::istream& args)
{
	std::string arg;
	if (!(args >> arg))
	{
		Logger::Error(m_output, "Arguments reading error");
		return true;
	}

	try
	{
		size_t pos;
		const int channel = std::stoi(arg, &pos);

		if (pos == arg.length())
		{
			if (!m_tv.SelectChannelByNumber(channel))
			{
				m_output << "Channel switched to: " << channel << "\n";
				return false;
			}
			Logger::Error(m_output, "Channel switch error");
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

	if (!m_tv.SelectChannelByName(fullName))
	{
		m_output << "Channel switched to: " << fullName << "\n";
		return false;
	}
	Logger::Error(m_output, "Channel switch error");
	return true;
}

bool RemoteControl::SelectPreviousChannel(std::istream&)
{
	if (!m_tv.SelectPreviousChannel())
	{
		m_output << "Switched to previous channel\n";
		return false;
	}
	Logger::Error(m_output, "SelectPreviousChannel failed");
	return true;
}

bool RemoteControl::SetChannelName(std::istream& args)
{
	int channel;
	if (!(args >> channel))
	{
		Logger::Error(m_output, "Arguments reading error");
		return true;
	}

	std::string name;
	std::getline(args >> std::ws, name);

	if (name.empty())
	{
		Logger::Error(m_output, "Name is empty");
		return true;
	}

	if (!m_tv.SetChannelName(channel, name))
	{
		m_output << "Channel name set: " << channel << " - " << name << "\n";
		return false;
	}
	Logger::Error(m_output, "Channel set error");
	return true;
}

bool RemoteControl::DeleteChannelName(std::istream& args)
{
	std::string name;
	std::getline(args >> std::ws, name);

	if (name.empty())
	{
		Logger::Error(m_output, "Channel name not found");
		return true;
	}

	if (!m_tv.DeleteChannelName(name))
	{
		m_output << "Channel name deleted: " << name << "\n";
		return false;
	}
	Logger::Error(m_output, "Channel deleting error");
	return true;
}

bool RemoteControl::GetChannelName(std::istream& args)
{
	int channel;
	if (!(args >> channel))
	{
		Logger::Error(m_output, "Arguments reading error");
		return true;
	}

	if (const auto channelName = m_tv.GetChannelName(channel))
	{
		m_output << "Channel " << channel << " name: " << *channelName << "\n";
		return false;
	}
	Logger::Error(m_output, "Channel get error");
	return true;
}

bool RemoteControl::GetChannelByName(std::istream& args)
{
	std::string name;
	std::getline(args >> std::ws, name);

	if (name.empty())
	{
		Logger::Error(m_output, "Channel name not found");
		return true;
	}

	if (const auto channelNum = m_tv.GetChannelByName(name))
	{
		m_output << "Channel for name " << name << ": " << *channelNum << "\n";
		return false;
	}
	Logger::Error(m_output, "Channel name not found");
	return true;
}