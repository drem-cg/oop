#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <map>
#include <vector>
#include <utility>

class TVSet;

class RemoteControl
{
public:
	using Handler = std::function<bool(std::istream&)>;
	using ActionMap = std::map<std::string, Handler>;
	using NamedChannel = std::pair<int, std::string>;
	using ChannelList = std::vector<NamedChannel>;

	RemoteControl(TVSet& tv, std::istream& input, std::ostream& output);

	bool HandleCommand() const;

private:
	bool TurnOn(std::istream& args) const;
	bool TurnOff(std::istream& args) const;
	bool Info(std::istream& args) const;
	bool SelectChannel(std::istream& args) const;
	bool SelectPreviousChannel(std::istream& args) const;
	bool SetChannelName(std::istream& args) const;
	bool DeleteChannelName(std::istream& args) const;
	bool GetChannelName(std::istream& args) const;
	bool GetChannelByName(std::istream& args) const;

	[[nodiscard]] ChannelList GetSortedChannelNames() const;

	TVSet& m_tv;
	std::istream& m_input;
	std::ostream& m_output;
	const ActionMap m_actionMap;
};