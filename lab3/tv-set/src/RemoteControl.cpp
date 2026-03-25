#include "RemoteControl.h"
#include "TVSet.h"
#include <algorithm>
#include <sstream>

CRemoteControl::CRemoteControl(CTVSet& tv, std::istream& input,
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

bool CRemoteControl::HandleCommand() const
{
	std::string line;
	if (!std::getline(m_input, line))
		return false;

	std::istringstream stream(line);
	std::string command;
	stream >> command;

	const auto action = m_actionMap.find(command);
	if (action != m_actionMap.end())
		return action->second(stream);

	return false;
}

bool CRemoteControl::TurnOn(std::istream&) const
{
	m_tv.TurnOn();
	m_output << "TV is turned on\n";
	return true;
}

bool CRemoteControl::TurnOff(std::istream&) const
{
	m_tv.TurnOff();
	m_output << "TV is turned off\n";
	return true;
}

bool CRemoteControl::Info(std::istream&) const
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

CRemoteControl::ChannelList CRemoteControl::GetSortedChannelNames() const
{
	ChannelList result;

	for (int ch = 1; ch <= 99; ++ch)
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

bool CRemoteControl::SelectChannel(std::istream& args) const
{
	std::string arg;
	if (!(args >> arg))
	{
		m_output << "ERROR\n";
		return true;
	}

	try
	{
		size_t pos;
		const int channel = std::stoi(arg, &pos);

		if (pos == arg.length())
		{
			if (m_tv.SelectChannelByNumber(channel))
				m_output << "Channel switched to: " << channel << "\n";
			else
				m_output << "ERROR\n";
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
		m_output << "ERROR\n";

	return true;
}

bool CRemoteControl::SelectPreviousChannel(std::istream&) const
{
	if (m_tv.SelectPreviousChannel())
		m_output << "Switched to previous channel\n";
	else
		m_output << "ERROR\n";

	return true;
}

bool CRemoteControl::SetChannelName(std::istream& args) const
{
	int channel;
	if (!(args >> channel))
	{
		m_output << "ERROR\n";
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
		m_output << "ERROR\n";

	return true;
}

bool CRemoteControl::DeleteChannelName(std::istream& args) const
{
	std::string name;
	std::getline(args >> std::ws, name);

	if (name.empty())
	{
		m_output << "ERROR\n";
		return true;
	}

	if (m_tv.DeleteChannelName(name))
		m_output << "Channel name deleted: " << name << "\n";
	else
		m_output << "ERROR\n";

	return true;
}

bool CRemoteControl::GetChannelName(std::istream& args) const
{
	int channel;
	if (!(args >> channel))
	{
		m_output << "ERROR\n";
		return true;
	}

	if (const auto nameOpt = m_tv.GetChannelName(channel))
		m_output << "Channel " << channel << " name: " << *nameOpt << "\n";
	else
		m_output << "ERROR\n";

	return true;
}

bool CRemoteControl::GetChannelByName(std::istream& args) const
{
	std::string name;
	std::getline(args >> std::ws, name);

	if (name.empty())
	{
		m_output << "ERROR\n";
		return true;
	}

	if (const auto chOpt = m_tv.GetChannelByName(name))
		m_output << "Channel for name " << name << ": " << *chOpt << "\n";
	else
		m_output << "ERROR\n";

	return true;
}