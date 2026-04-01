#pragma once

#include <optional>
#include <string>
#include <unordered_map>

class TVSet
{
public:
	// TODO: разобраться со стрелкой в draw io и зависисостями между классами
	static constexpr int MIN_CHANNEL = 1;
	static constexpr int MAX_CHANNEL = 99;

	TVSet();

	void TurnOn();
	void TurnOff();
	[[nodiscard]] bool IsTurnedOn() const;
	[[nodiscard]] int GetChannel() const;
	bool SelectChannelByNumber(int channel);
	bool SelectChannelByName(const std::string& name);
	bool SelectPreviousChannel();
	bool SetChannelName(int channel, const std::string& name);
	bool DeleteChannelName(const std::string& name);
	std::optional<std::string> GetChannelName(int channel) const;
	std::optional<int> GetChannelByName(const std::string& name) const;

private:
	[[nodiscard]] static std::string NormalizeName(const std::string& name);
	[[nodiscard]] bool CheckPoweredOn() const;
	[[nodiscard]] static bool IsValidChannel(int channel);

	bool m_isOn;
	int m_currentChannel;
	int m_lastChannel;

	std::unordered_map<int, std::string> m_channelToName;
	std::unordered_map<std::string, int> m_nameToChannel;
};