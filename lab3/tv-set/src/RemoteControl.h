#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

class TVSet;

class RemoteControl
{
public:
	RemoteControl(TVSet& tv, std::istream& input, std::ostream& output);
	[[nodiscard]] bool HandleCommand() const;

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

	[[nodiscard]] ChannelList GetSortedChannelNames() const;

	using Handler = std::function<bool(std::istream&)>;
	using ActionMap = std::map<std::string, Handler>;

	// TODO провисший указатель
	TVSet& m_tv;
	std::istream& m_input;
	// TODO ф-ция для вывода ошибок
	std::ostream& m_output;
	const ActionMap m_actionMap;
};