#pragma once

#include <optional>
#include <string>
#include <unordered_map>

class CTVSet
{
public:
	CTVSet();
	static constexpr int MIN_CHANNEL = 1;
	static constexpr int MAX_CHANNEL = 99;

	bool IsTurnedOn() const;
	void TurnOn();
	void TurnOff();
	int GetChannel() const;
	bool SelectChannelByNumber(int channel);
	bool SelectChannelByName(const std::string& name);
	bool SelectPreviousChannel();
	bool SetChannelName(int channel, const std::string& name);
	bool DeleteChannelName(const std::string& name);
	std::optional<std::string> GetChannelName(int channel) const;
	std::optional<int> GetChannelByName(const std::string& name) const;

private:
	bool m_isOn;
	int m_currentChannel;
	int m_lastChannel;
	static bool IsValidChannel(int channel);
	static std::string NormalizeName(const std::string& name);
	bool CheckPoweredOn() const;
	std::unordered_map<int, std::string> m_channelToName;
	std::unordered_map<std::string, int> m_nameToChannel;
};