#include "TVSet.h"
#include <algorithm>
#include <cctype>

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
		return false;

	if (channel != m_currentChannel)
	{
		m_lastChannel = m_currentChannel;
		m_currentChannel = channel;
	}

	return true;
}

bool TVSet::SelectChannelByName(const std::string& name)
{
	if (!m_isOn)
	{
		return false;
	}

	const auto channelNumber = GetChannelByName(name);
	if (!channelNumber)
	{
		return false;
	}

	return SelectChannelByNumber(*channelNumber);
}

bool TVSet::SelectPreviousChannel()
{
	if (!m_isOn)
	{
		return false;
	}

	if (m_currentChannel == m_lastChannel)
	{
		return false;
	}

	const int temp = m_currentChannel;
	m_currentChannel = m_lastChannel;
	m_lastChannel = temp;

	return true;
}

bool TVSet::SetChannelName(const int channel, const std::string& name)
{
	if (!CheckPoweredOn() || !IsValidChannel(channel))
	{
		return true;
	}

	const std::string normalizedName = NormalizeName(name);
	if (normalizedName.empty())
	{
		return true;
	}

	const auto entryName = m_nameToChannel.find(normalizedName);
	if (entryName != m_nameToChannel.end() && entryName->second != channel)
	{
		m_channelToName.erase(entryName->second);
		m_nameToChannel.erase(entryName);
	}

	const auto channelName = m_channelToName.find(channel);
	if (channelName != m_channelToName.end())
	{
		m_nameToChannel.erase(channelName->second);
		m_channelToName.erase(channelName);
	}

	m_channelToName[channel] = normalizedName;
	m_nameToChannel[normalizedName] = channel;

	return false;
}

bool TVSet::DeleteChannelName(const std::string& name)
{
	if (!CheckPoweredOn())
	{
		return false;
	}

	const std::string normalizedName = NormalizeName(name);
	if (normalizedName.empty())
	{
		return false;
	}

	const auto channelNum = m_nameToChannel.find(normalizedName);
	if (channelNum == m_nameToChannel.end())
	{
		return false;
	}

	const int channel = channelNum->second;
	m_nameToChannel.erase(channelNum);
	m_channelToName.erase(channel);

	return true;
}

std::optional<std::string> TVSet::GetChannelName(const int channel) const
{
	if (!CheckPoweredOn())
		return std::nullopt;

	const auto channelName = m_channelToName.find(channel);
	if (channelName != m_channelToName.end())
	{
		return channelName->second;
	}
	return std::nullopt;
}

std::optional<int> TVSet::GetChannelByName(const std::string& name) const
{
	if (!CheckPoweredOn())
	{
		return std::nullopt;
	}

	const std::string normalizedName = NormalizeName(name);
	if (normalizedName.empty())
	{
		return std::nullopt;
	}

	const auto channel = m_nameToChannel.find(normalizedName);
	return channel != m_nameToChannel.end() ? std::optional(channel->second)
											: std::nullopt;
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
	{
		result.pop_back();
	}

	if (result.find_first_not_of(' ') == std::string::npos)
	{
		return "";
	}

	return result;
}

bool TVSet::CheckPoweredOn() const
{
	return m_isOn;
}