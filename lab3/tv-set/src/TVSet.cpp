#include "TVSet.h"
#include <algorithm>

TVSet::TVSet()
	: m_isOn(false)
	, m_currentChannel(0)
	, m_lastChannel(0)
{
}

bool TVSet::IsValidChannel(const int channel)
{
	return channel >= MIN_CHANNEL && channel <= MAX_CHANNEL;
}

bool TVSet::IsTurnedOn() const
{
	return m_isOn;
}

void TVSet::TurnOn()
{
	if (!m_isOn)
	{
		m_isOn = true;
		if (m_currentChannel == 0)
		{
			m_currentChannel = 1;
			m_lastChannel = 1;
		}
	}
}

void TVSet::TurnOff()
{
	if (m_isOn)
	{
		m_isOn = false;
	}
}

int TVSet::GetChannel() const
{
	if (m_isOn)
	{
		return m_currentChannel;
	}
	return 0;
}

bool TVSet::SelectChannelByNumber(const int channel)
{
	if (!m_isOn || !IsValidChannel(channel))
		return true;

	if (channel != m_currentChannel)
	{
		m_lastChannel = m_currentChannel;
		m_currentChannel = channel;
	}

	return false;
}

bool TVSet::SelectChannelByName(const std::string& name)
{
	if (!m_isOn)
		return true;

	const auto channelNumber = GetChannelByName(name);
	if (!channelNumber)
		return true;

	return SelectChannelByNumber(*channelNumber);
}

bool TVSet::SelectPreviousChannel()
{
	if (!m_isOn)
		return true;

	if (m_currentChannel == m_lastChannel || m_lastChannel == 0)
		return true;

	std::swap(m_currentChannel, m_lastChannel);
	return false;
}

bool TVSet::SetChannelName(const int channel, const std::string& name)
{
	if (!CheckPoweredOn() || !IsValidChannel(channel))
		return true;

	const std::string normalizedName = NormalizeName(name);
	if (normalizedName.empty())
		return true;

	const auto entryByName = m_nameToChannel.find(normalizedName);
	if (entryByName != m_nameToChannel.end() && entryByName->second != channel)
	{
		m_channelToName.erase(entryByName->second);
		m_nameToChannel.erase(entryByName);
	}

	const auto entryByChannel = m_channelToName.find(channel);
	if (entryByChannel != m_channelToName.end())
	{
		m_nameToChannel.erase(entryByChannel->second);
		m_channelToName.erase(entryByChannel);
	}

	m_channelToName[channel] = normalizedName;
	m_nameToChannel[normalizedName] = channel;

	return false;
}

bool TVSet::DeleteChannelName(const std::string& name)
{
	if (!CheckPoweredOn())
		return true;

	const std::string normalizedName = NormalizeName(name);
	if (normalizedName.empty())
		return true;

	const auto channelNum = m_nameToChannel.find(normalizedName);
	if (channelNum == m_nameToChannel.end())
		return true;

	const int channel = channelNum->second;
	m_nameToChannel.erase(channelNum);
	m_channelToName.erase(channel);

	return false;
}

std::optional<std::string> TVSet::GetChannelName(const int channel) const
{
	if (!CheckPoweredOn())
		return std::nullopt;

	const auto channelName = m_channelToName.find(channel);
	if (channelName != m_channelToName.end())
		return channelName->second;

	return std::nullopt;
}

std::optional<int> TVSet::GetChannelByName(const std::string& name) const
{
	if (!CheckPoweredOn())
		return std::nullopt;

	const std::string normalizedName = NormalizeName(name);
	if (normalizedName.empty())
		return std::nullopt;

	const auto channel = m_nameToChannel.find(normalizedName);
	if (channel != m_nameToChannel.end())
		return channel->second;

	return std::nullopt;
}

std::string TVSet::NormalizeName(const std::string& name)
{
	std::string result;
	bool isPrevSpace = true;
	for (const char ch : name)
	{
		if (ch == ' ')
		{
			if (!isPrevSpace)
			{
				result += ' ';
				isPrevSpace = true;
			}
		}
		else
		{
			result += ch;
			isPrevSpace = false;
		}
	}

	if (!result.empty() && result.back() == ' ')
		result.pop_back();

	if (result.find_first_not_of(' ') == std::string::npos)
		return "";

	return result;
}

bool TVSet::CheckPoweredOn() const
{
	return m_isOn;
}