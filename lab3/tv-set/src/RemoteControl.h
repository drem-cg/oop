#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

class CTVSet;

class CRemoteControl
{
public:
	CRemoteControl(CTVSet& tv, std::istream& input, std::ostream& output);
	bool HandleCommand() const;

private:
	using ChannelList = std::vector<std::pair<int, std::string>>;
	bool TurnOn(std::istream& args) const;
	bool TurnOff(std::istream& args) const;
	bool Info(std::istream& args) const;
	bool SelectChannel(std::istream& args) const;
	bool SelectPreviousChannel(std::istream& args) const;
	bool SetChannelName(std::istream& args) const;
	bool DeleteChannelName(std::istream& args) const;
	bool GetChannelName(std::istream& args) const;
	bool GetChannelByName(std::istream& args) const;

	ChannelList GetSortedChannelNames() const;

	using Handler = std::function<bool(std::istream&)>;
	using ActionMap = std::map<std::string, Handler>;

	CTVSet& m_tv;
	std::istream& m_input;
	std::ostream& m_output;
	const ActionMap m_actionMap;
};