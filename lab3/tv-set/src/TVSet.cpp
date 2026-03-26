#include "TVSet.h"
#include <algorithm>
#include <cctype>

CTVSet::CTVSet()
	: m_isOn(false)
	, m_currentChannel(0)
	, m_lastChannel(0)
{
}

bool CTVSet::IsValidChannel(const int channel)
{
	return channel >= MIN_CHANNEL && channel <= MAX_CHANNEL;
}

bool CTVSet::IsTurnedOn() const
{
	return m_isOn;
}

void CTVSet::TurnOn()
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

void CTVSet::TurnOff()
{
	if (m_isOn)
	{
		m_isOn = false;
	}
}

int CTVSet::GetChannel() const
{
	if (m_isOn)
	{
		return m_currentChannel;
	}
	return 0;
}

bool CTVSet::SelectChannelByNumber(const int channel)
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

bool CTVSet::SelectChannelByName(const std::string& name)
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

bool CTVSet::SelectPreviousChannel()
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

bool CTVSet::SetChannelName(const int channel, const std::string& name)
{
	if (!CheckPoweredOn() || !IsValidChannel(channel))
	{
		return false;
	}

	const std::string normalizedName = NormalizeName(name);
	if (normalizedName.empty())
	{
		return false;
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

	return true;
}

bool CTVSet::DeleteChannelName(const std::string& name)
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

std::optional<std::string> CTVSet::GetChannelName(const int channel) const
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

std::optional<int> CTVSet::GetChannelByName(const std::string& name) const
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

std::string CTVSet::NormalizeName(const std::string& name)
{
	std::string result;
	bool isPrevSpace = true;
	for (const char c : name)
	{
		if (std::isspace(static_cast<unsigned char>(c)))
		{
			if (!isPrevSpace)
			{
				result += ' ';
				isPrevSpace = true;
			}
		}
		else
		{
			result += c;
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

bool CTVSet::CheckPoweredOn() const
{
	return m_isOn;
}